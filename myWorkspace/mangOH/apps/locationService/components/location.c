//--------------------------------------------------------------------------------------------------
/**
 * Implementation of the Brooklyn position sensor interface to the Data Hub.
 *
 * Copyright (C) Sierra Wireless Inc.
 */
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"
#include "periodicSensor.h"
#include <stdio.h>
#include <time.h>

#define HACCURACY_GOOD_GPS    150
#define HACCURACY_WIFI_ONLY   500
#define PSENSOR_ENABLE        "coordinates/enable"
#define PSENSOR_PERIOD        "coordinates/period"
#define DEFAULT_PERIOD        30
#define FIX_TYPE_THIS_PERIOD  "FixTypeThisPeriod/value"
#define HAVE_FIX              "fix/value"


typedef enum {GPS, WIFI} Loc_t;

typedef struct ScanReading
{
    double lat;
    double lon;
    double hAccuracy;
    double alt;
    double vAccuracy;
} Scan_t ;

// Hacking - assuming mangOH Yellow and the Cypress chip as wlan1 - TODO: add TI Wifi on wlan0?
//static const char *interfacePtr = "wlan1";
// Legato WIFI is broken so we need to create a fake access point to do a scan 
static const char *ssidPtr = "mangOH";
static le_wifiClient_AccessPointRef_t  createdAccessPoint;
static bool  WifiStarted = false;

static struct
{
    ma_combainLocation_LocReqHandleRef_t combainHandle;
    bool waitingForWifiResults;
    bool waitingForCombainResults;
    le_wifiClient_NewEventHandlerRef_t wifiHandler;
} State;

// > HACCURACY_GOOD_GPS - save the last GPS scan to send if Wifi scan fails
static Scan_t SavedGpsScan;
static Scan_t *GpsScan = NULL;

static psensor_Ref_t saved_ref = NULL;

uint64_t GetCurrentTimestamp(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t utcMilliSec = (uint64_t)(tv.tv_sec) * 1000 + (uint64_t)(tv.tv_usec) / 1000;
    return utcMilliSec;
}

static bool MacAddrStringToBinary(const char* s, uint8_t *b)
{
    size_t inputOffset = 0;
    const size_t macStrLen = (6 * 2) + (6 - 1);
    while (inputOffset < macStrLen)
    {
        if (inputOffset % 3 == 2)
        {
            if (s[inputOffset] != ':')
            {
                return false;
            }
        }
        else
        {
            uint8_t nibble;
            if (s[inputOffset] >= '0' && s[inputOffset] <= '9')
            {
                nibble = s[inputOffset] - '0';
            }
            else if (s[inputOffset] >= 'a' && s[inputOffset] <= 'f')
            {
                nibble = 10 + s[inputOffset] - 'a';
            }
            else if (s[inputOffset] >= 'A' && s[inputOffset] <= 'F')
            {
                nibble = 10 + s[inputOffset] - 'A';
            }
            else
            {
                return false;
            }

            if (inputOffset % 3 == 0)
            {
                b[inputOffset / 3] = (nibble << 4);
            }
            else
            {
                b[inputOffset / 3] |= (nibble << 0);
            }
        }

        inputOffset++;
    }

    if (s[inputOffset] != '\0')
    {
        return false;
    }

    return true;
}

static void PackJson
(
    Loc_t loc,
    Scan_t *scanp,
    char *jsonp,
    int jsonl
)
{
    uint64_t ts = 0;
    le_result_t res;
    int len;
    uint16_t hours;
    uint16_t minutes;
    uint16_t seconds;
    uint16_t millis;
    uint16_t years;
    uint16_t months;
    uint16_t days;
    time_t rawtime;
    struct tm * timeinfo;

    res = le_pos_GetTime(&hours, &minutes, &seconds, &millis);
    if (res == LE_OK){
        res = le_pos_GetDate(&years, &months, &days);
        if (res == LE_OK){
            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            timeinfo->tm_year = years - 1900;
            timeinfo->tm_mon = months - 1;
            timeinfo->tm_mday = days;
            timeinfo->tm_hour = hours;
            timeinfo->tm_min = minutes;
            timeinfo->tm_sec = seconds;

            time_t converted = mktime(timeinfo);
            ts = (uint64_t)converted * 1000;
        } else {
            LE_ERROR("Could not get position date: %s", LE_RESULT_TXT(res));
            ts = GetCurrentTimestamp();
        }
    } else {
        LE_ERROR("Could not get position time: %s", LE_RESULT_TXT(res));
        ts = GetCurrentTimestamp();
    }

    if (loc == GPS)
        len = snprintf(jsonp, jsonl,
                       "{ \"lat\": %lf, \"lon\": %lf, \"hAcc\": %lf,"
                       " \"alt\": %lf, \"vAcc\": %lf, \"fixType\" : \"GNSS\", \"ts\" : %ju}",
                       scanp->lat, scanp->lon, scanp->hAccuracy,
                       scanp->alt, scanp->vAccuracy, (uintmax_t)ts);
    // For Wifi we keep the same JSON format as GNSS - i.e. alt & vAcc are 0
    // TODO: go to jansson directly and send NULL
    else if (loc == WIFI)
        len = snprintf(jsonp, jsonl,
                       "{ \"lat\": %lf, \"lon\": %lf, \"hAcc\": %lf,"
                       " \"alt\": %lf, \"vAcc\": %lf, \"fixType\" : \"WIFI\", \"ts\" : %ju}",
                       scanp->lat, scanp->lon, scanp->hAccuracy,
                       (double) 0, (double) 0, (uintmax_t)ts);

    if (len >= jsonl)
        LE_FATAL("JSON string (len %d) is longer than buffer (size %zu).", len, jsonl);
}

static void UseGpsScan(void)
{
    char json[256];

    if (GpsScan != NULL)
    {
        PackJson(GPS, &SavedGpsScan, json, sizeof(json));
        LE_INFO("Sending dhub json: %s", json);
        psensor_PushJson(saved_ref, 0 /* now */, json);
        GpsScan = NULL;
        dhubIO_PushString(FIX_TYPE_THIS_PERIOD, DHUBIO_NOW, "Weak GNSS");
        dhubIO_PushBoolean(HAVE_FIX, DHUBIO_NOW, true);
    }
    dhubIO_PushString(FIX_TYPE_THIS_PERIOD, DHUBIO_NOW, "NONE");
    dhubIO_PushBoolean(HAVE_FIX, DHUBIO_NOW, false);
}

static void LocationResultHandler(
    ma_combainLocation_LocReqHandleRef_t handle, ma_combainLocation_Result_t result, void *context)
{
    char json[256];

    if (State.waitingForCombainResults != true)
    {
        LE_FATAL("Serious error from Combain service");
        exit(1);
    }

    State.waitingForCombainResults = false;
    switch (result)
    {
    case MA_COMBAINLOCATION_RESULT_SUCCESS:
    {
        Scan_t scan;


        const le_result_t res = ma_combainLocation_GetSuccessResponse(
            handle, &scan.lat, &scan.lon, &scan.hAccuracy);
        if (res != LE_OK)
        {
            LE_INFO(
                "Received result notification of type success response, but couldn't fetch the result\n");
        }

        // Add hard-coded geo-fencing for Vancouver SunRun - TODO - fix with values from dhub
        else if (scan.lat < 49.0f || scan.lat >= 50.0f ||
                 scan.lon > -122.0f || scan.lon <= -124.0f)
        {
            UseGpsScan();
        }

        else
        {
            LE_INFO("Location: latitude=%f, longitude=%f, accuracy=%f meters\n",
                    scan.lat, scan.lon, scan.hAccuracy);
            PackJson(WIFI, &scan, json, sizeof(json));
            LE_INFO("Sending dhub json: %s", json);
            psensor_PushJson(saved_ref, 0 /* now */, json);
            saved_ref = NULL;
            dhubIO_PushString(FIX_TYPE_THIS_PERIOD, DHUBIO_NOW, "Wifi");
            dhubIO_PushBoolean(HAVE_FIX, DHUBIO_NOW, true);
        }
        return;
    }

    case MA_COMBAINLOCATION_RESULT_ERROR:
    {
        char firstDomain[65];
        char firstReason[65];
        char firstMessage[129];
        uint16_t code;
        char message[129];

        const le_result_t res = ma_combainLocation_GetErrorResponse(
            handle,
            firstDomain,
            sizeof(firstDomain) - 1,
            firstReason,
            sizeof(firstReason) - 1,
            firstMessage,
            sizeof(firstMessage) - 1,
            &code,
            message,
            sizeof(message) - 1);
        if (res != LE_OK)
        {
            LE_INFO(
                "Received result notification of type success response, but couldn't fetch the result\n");
        }
        else
        {
            LE_INFO("Received an error response.\n");
            LE_INFO("  firstDomain: %s\n", firstDomain);
            LE_INFO("  firstReason: %s\n", firstReason);
            LE_INFO("  firstMessage: %s\n", firstMessage);
            LE_INFO("  code: %d\n", code);
            LE_INFO("  message: %s\n", message);
        }

        break;
    }

    case MA_COMBAINLOCATION_RESULT_RESPONSE_PARSE_FAILURE:
    {
        char rawResult[257];
        const le_result_t res = ma_combainLocation_GetParseFailureResult(
            handle, rawResult, sizeof(rawResult) - 1);
        if (res != LE_OK)
        {
            LE_INFO(
                "Received result notification of type success response, but couldn't fetch the result\n");
        }
        else
        {
            LE_INFO("Received a result which couldn't be parsed \"%s\"\n", rawResult);
        }
        break;
    }

    case MA_COMBAINLOCATION_RESULT_COMMUNICATION_FAILURE:
        LE_INFO("Couldn't communicate with Combain server\n");
        break;

    default:
        LE_INFO("Received unhandled result type (%d)\n", result);
    }

    UseGpsScan();
}

static bool TrySubmitRequest(void)
{
    if (!State.waitingForWifiResults && State.waitingForCombainResults == false)
    {
        LE_INFO("Attempting to submit location request");
        const le_result_t res = ma_combainLocation_SubmitLocationRequest(
            State.combainHandle, LocationResultHandler, NULL);

         if (res == LE_OK) {
             State.waitingForCombainResults = true;
             LE_INFO("Submitted request handle: %d", (uint32_t) State.combainHandle);
             return true;
         }
        
         LE_INFO("Failed to submit location request\n");
    }

    ma_combainLocation_DestroyLocationRequest(State.combainHandle);
    LE_INFO("Cannot submit WIFI location request to Combain as previous in transit");
    dhubIO_PushString(FIX_TYPE_THIS_PERIOD, DHUBIO_NOW, "NONE");
    dhubIO_PushBoolean(HAVE_FIX, DHUBIO_NOW, false);
    return false;
}

static void WifiEventHandler(le_wifiClient_Event_t event, void *context)
{
    LE_INFO("Called WifiEventHandler() with event=%d", event);
    switch (event)
    {
        case LE_WIFICLIENT_EVENT_SCAN_DONE:
            if (State.waitingForWifiResults)
            {
                State.waitingForWifiResults = false;
                State.combainHandle = ma_combainLocation_CreateLocationRequest();
                LE_INFO("Create request handle: %d", (uint32_t) State.combainHandle);
    
                le_wifiClient_AccessPointRef_t ap = le_wifiClient_GetFirstAccessPoint();
                while (ap != NULL)
                {
                    // Add one to the length so that the SSID is guaranteed to have a final zero
                    // byte. This allows us to (incorrectly) treat the SSID as though it's a C
                    // string and print it out. It's possible that some of the string may not get
                    // printed if there are unprintable characters or embedded nulls in the SSID.
                    uint8_t ssid[32 + 1] = {0};
                    size_t ssidLen = sizeof(ssid)- 1;
                    char bssid[(2 * 6) + (6 - 1) + 1]; // "nn:nn:nn:nn:nn:nn\0"
                    int16_t signalStrength;
                    le_result_t res;
                    uint8_t bssidBytes[6];
                    res = le_wifiClient_GetSsid(ap, ssid, &ssidLen);
                    if (res != LE_OK)
                    {
                        LE_INFO("Failed while fetching WiFi SSID\n");
                    }
    
                    res = le_wifiClient_GetBssid(ap, bssid, sizeof(bssid) - 1);
                    if (res != LE_OK)
                    {
                        LE_INFO("Failed while fetching WiFi BSSID\n");
                    }
    
                    signalStrength = le_wifiClient_GetSignalStrength(ap);
                    if (signalStrength == LE_WIFICLIENT_NO_SIGNAL_STRENGTH)
                    {
                        LE_INFO("Failed while fetching WiFi signal strength\n");
                    }

                    if (!MacAddrStringToBinary(bssid, bssidBytes))
                    {
                        LE_INFO("WiFi scan contained invalid bssid=\"%s\"\n", bssid);
                    }

                    res = ma_combainLocation_AppendWifiAccessPoint(
                    State.combainHandle, bssidBytes, 6, ssid, ssidLen, signalStrength);
                    LE_INFO("Submitted AccessPoint: %d ssid: %s", (uint32_t) State.combainHandle,
                        (char *) ssid);

                    if (res != LE_OK)
                    {
                        LE_INFO("Failed to append a WiFi scan result to combain request\n");
                    }

                    ap = le_wifiClient_GetNextAccessPoint();
                }
                /* if (WifiStarted)
                {
                    le_wifiClient_Stop();
                    WifiStarted = false;
                } */

                TrySubmitRequest();
            }
            break;

        default:
        {
            LE_INFO("WiFi scan failed\n");
            /*if (WifiStarted)
            {
                le_wifiClient_Stop();
                WifiStarted = false;
            }*/
            ma_combainLocation_DestroyLocationRequest(State.combainHandle);
            State.waitingForWifiResults = false;
            UseGpsScan();
            break;
        }
    }
}

static void Sample
(
    psensor_Ref_t ref,
    void *context
)
{
    Scan_t scan;
    char json[256];

    int32_t lat;
    int32_t lon;
    int32_t hAccuracy;
    int32_t alt;
    int32_t vAccuracy;

    le_result_t posRes = le_pos_Get3DLocation(&lat, &lon, &hAccuracy, &alt, &vAccuracy);

    if (posRes == LE_OK)
        LE_INFO("le_pos_Get3DLocation returned: lat = %d lon: %d hAccuracy: %d alt: %d vAccuracy: %d",
           lat, lon, hAccuracy, alt, vAccuracy); 
    else
        LE_INFO("le_pos_Get3DLocation FAILED: %s", LE_RESULT_TXT(posRes));

    scan.lat = (double) lat / 1000000.0 ;
    scan.lon = (double) lon / 1000000.0;
    scan.hAccuracy = (double) hAccuracy;
    scan.alt = (double) alt / 1000.0;
    scan.vAccuracy = (double) vAccuracy;

    /* LE_INFO("CVT double: lat = %f lon: %f hAccuracy: %f alt: %f vAccuracy: %f",
       scan.lat, scan.lon, scan.hAccuracy, scan.alt, scan.vAccuracy); */

    // No GPS or low accuracy GPS try Wifi Scan & Combain translation
    if (posRes != LE_OK || scan.hAccuracy > HACCURACY_GOOD_GPS) {
        le_result_t startRes;

        // If Combain is down no use doing a Wifi scan
        if (!ma_combainLocation_ServiceAvailable())
        {
            LE_INFO("Combain Service is not available");
            dhubIO_PushString(FIX_TYPE_THIS_PERIOD, DHUBIO_NOW, "NONE");
            dhubIO_PushBoolean(HAVE_FIX, DHUBIO_NOW, false);
            return;
        }

        if (scan.hAccuracy < HACCURACY_WIFI_ONLY)
        {
            SavedGpsScan = scan;
            GpsScan = &SavedGpsScan;
        }    

        if (!State.waitingForWifiResults && State.waitingForCombainResults == false) {
            if (!createdAccessPoint)
            {
                createdAccessPoint = le_wifiClient_Create((const uint8_t *)ssidPtr, strlen(ssidPtr));
                if (NULL == createdAccessPoint)
                {
                    LE_INFO("le_wifiClient_Create returns NULL.\n");
                    exit(1);
                }

                startRes = le_wifiClient_SetInterface(createdAccessPoint, "wlan1");
                if (LE_OK != startRes)
                {
                    LE_FATAL("ERROR: le_wifiClient_SetInterface returns %d.\n", startRes);
                    exit(1);
                }
            }

            if (WifiStarted == false) {
                startRes = le_wifiClient_Start();

                if (startRes != LE_OK && startRes != LE_BUSY) {
                    LE_INFO("Couldn't start the WiFi service error code: %s", LE_RESULT_TXT(startRes));
		    WifiStarted = false;
                }
                else
		    WifiStarted = true;
            }

            if (WifiStarted == true) {
                saved_ref = ref;
                le_wifiClient_Scan();
                State.waitingForWifiResults = true;
            }
        }
        else {
            LE_INFO("le_wifiClient_Scan still RUNNING");
            dhubIO_PushString(FIX_TYPE_THIS_PERIOD, DHUBIO_NOW, "Waiting for Wifi");
            dhubIO_PushBoolean(HAVE_FIX, DHUBIO_NOW, false);
        }
    }

    // Good GPS
    else if (posRes == LE_OK && scan.hAccuracy <= HACCURACY_GOOD_GPS)
    {
        PackJson(GPS, &scan, json, sizeof(json));
        LE_INFO("Sending dhub json: %s", json);
        psensor_PushJson(ref, 0 /* now */, json);
        GpsScan = NULL;
        dhubIO_PushString(FIX_TYPE_THIS_PERIOD, DHUBIO_NOW, "Good GNSS");
        dhubIO_PushBoolean(HAVE_FIX, DHUBIO_NOW, true);

        // Kill any errant WIFI scan or Combain requests as we got GPS
        if (State.waitingForWifiResults || State.waitingForCombainResults)
        {
            ma_combainLocation_DestroyLocationRequest(State.combainHandle);
            State.waitingForWifiResults = false;
        }
    }

    else {
        LE_ERROR("NO RESULTS TO REPORT");
        dhubIO_PushString(FIX_TYPE_THIS_PERIOD, DHUBIO_NOW, "NONE");
        dhubIO_PushBoolean(HAVE_FIX, DHUBIO_NOW, false);
    }
}


COMPONENT_INIT
{
    // Activate the positioning service.
    le_posCtrl_ActivationRef_t posCtrlRef = le_posCtrl_Request();
    LE_FATAL_IF(posCtrlRef == NULL, "Couldn't activate positioning service");

    State.wifiHandler = le_wifiClient_AddNewEventHandler(WifiEventHandler, NULL);

    // Use the periodic sensor component from the Data Hub to implement the timer and Data Hub
    // interface.  We'll provide samples as JSON structures.
    psensor_Create("coordinates", DHUBIO_DATA_TYPE_JSON, "", Sample, NULL);

    // Make sure the periodic sensor is enabled and defaults to 30 secs.
    dhubIO_SetBooleanDefault(PSENSOR_ENABLE, true);
    dhubIO_SetNumericDefault(PSENSOR_PERIOD, DEFAULT_PERIOD);

    dhubIO_SetJsonExample("coordinates/value", "{\"lat\":0.1,\"lon\":0.2,"
                          "\"alt\":0.3,\"hAcc\":0.4,\"vAcc\":0.5,\"fixType\":\"GNSS\"}");

    LE_ASSERT(LE_OK == dhubIO_CreateInput(FIX_TYPE_THIS_PERIOD, DHUBIO_DATA_TYPE_STRING, ""));
    LE_ASSERT(LE_OK == dhubIO_CreateInput(HAVE_FIX, DHUBIO_DATA_TYPE_BOOLEAN, ""));
}

