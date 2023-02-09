
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */
#ifndef LE_WIFIAP_COMMON_H_INCLUDE_GUARD
#define LE_WIFIAP_COMMON_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_wifiDefs_common.h"

#define IFGEN_LE_WIFIAP_PROTOCOL_ID "36c307882cc1b538442afcf212938aee"
#define IFGEN_LE_WIFIAP_MSG_SIZE 77



//--------------------------------------------------------------------------------------------------
/**
 * WiFi Access Point Events.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_WIFIAP_EVENT_CLIENT_CONNECTED = 0,
        ///< Client connected to WiFi Access Point.
    LE_WIFIAP_EVENT_CLIENT_DISCONNECTED = 1
        ///< Client disconnected from WiFi Access Point.
}
le_wifiAp_Event_t;


//--------------------------------------------------------------------------------------------------
/**
 * WiFi Access Point Security levels.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_WIFIAP_SECURITY_NONE = 0,
        ///< WiFi Access Point is open and has no password.
    LE_WIFIAP_SECURITY_WPA2 = 1
        ///< WiFi Access Point has WPA2 activated.
}
le_wifiAp_SecurityProtocol_t;


//--------------------------------------------------------------------------------------------------
/**
 * IEEE 802.11 standards Bit Mask
 */
//--------------------------------------------------------------------------------------------------/// IEEE 802.11a (5 GHz) Bit Mask.
#define LE_WIFIAP_BITMASK_IEEE_STD_A 0x1/// IEEE 802.11b (2.4 GHz) Bit Mask.
#define LE_WIFIAP_BITMASK_IEEE_STD_B 0x2/// IEEE 802.11g (2.4 GHz) Bit Mask.
#define LE_WIFIAP_BITMASK_IEEE_STD_G 0x4/// IEEE 802.11ad (60 GHz) Bit Mask.
#define LE_WIFIAP_BITMASK_IEEE_STD_AD 0x8/// IEEE 802.11d Bit Mask. This advertises the country code.
#define LE_WIFIAP_BITMASK_IEEE_STD_D 0x10/// IEEE 802.11h Bit Mask. This enables radar detection.
#define LE_WIFIAP_BITMASK_IEEE_STD_H 0x20/// IEEE 802.11n (HT) Bit Mask.
#define LE_WIFIAP_BITMASK_IEEE_STD_N 0x40/// IEEE 802.11ac (VHT) Bit Mask.
#define LE_WIFIAP_BITMASK_IEEE_STD_AC 0x80/// IEEE 802.11ax (HE) Bit Mask.
#define LE_WIFIAP_BITMASK_IEEE_STD_AX 0x100/// IEEE 802.11w Bit Mask.
#define LE_WIFIAP_BITMASK_IEEE_STD_W 0x200
typedef uint32_t le_wifiAp_IeeeStdBitMask_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_wifiAp_NewEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_wifiAp_NewEventHandler* le_wifiAp_NewEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for WiFi Access Point changes
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_wifiAp_EventHandlerFunc_t)
(
        le_wifiAp_Event_t event,
        ///< Handles the wifi events
        void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Get if this client bound locally.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_le_wifiAp_HasLocalBinding
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Init data that is common across all threads
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_wifiAp_InitCommonData
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Perform common initialization and open a session
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_OpenSession
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    bool isBlocking
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_wifiAp_NewEvent'
 *
 * These events provide information on WiFi Access Point
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_wifiAp_NewEventHandlerRef_t ifgen_le_wifiAp_AddNewEventHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_wifiAp_EventHandlerFunc_t handlerPtr,
        ///< [IN]
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_wifiAp_NewEvent'
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_wifiAp_RemoveNewEventHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_wifiAp_NewEventHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function starts the WIFI Access Point.
 * @note that all settings, if to be used, such as security, username, password must set prior to
 * starting the Access Point.
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_Start
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * This function stops the WIFI Access Point.
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_Stop
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Service set identification (SSID) of the AccessPoint
 * Default value is "LEGATO Access Point"
 * @note that the SSID does not have to be human readable ASCII values, but often has.
 *
 * @return
 *      - LE_BAD_PARAMETER if some parameter is invalid.
 *      - LE_OK if the function succeeded.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_SetSsid
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        const uint8_t* ssidPtr,
        ///< [IN] The SSID to set as a octet array.
        size_t ssidSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Security protocol to use.
 * Default value is SECURITY_WPA2.
 * @note that the SSID does not have to be human readable ASCII values, but often has.
 *
 * @return
 *      - LE_BAD_PARAMETER if some parameter is invalid.
 *      - LE_OK if the function succeeded.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_SetSecurityProtocol
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_wifiAp_SecurityProtocol_t securityProtocol
        ///< [IN] The security protocol to use.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the passphrase used to generate the PSK.
 *
 * @note If the PSK is to be set directly, please use le_wifiAp_SetPreSharedKey()
 *
 * @return
 *      - LE_BAD_PARAMETER if parameter is invalid.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_SetPassPhrase
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        const char* LE_NONNULL passPhrase
        ///< [IN] pass-phrase for PSK
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Pre Shared Key, PSK.
 * There is no default value, since le_wifiAp_SetPassPhrase is used as default.
 * @note the difference between le_wifiAp_SetPassPhrase() and this function
 *
 * @return
 *      - LE_BAD_PARAMETER if parameter is invalid.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_SetPreSharedKey
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        const char* LE_NONNULL preSharedKey
        ///< [IN] PSK. Note the difference between PSK and Pass Phrase.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set if the Access Point should announce its presence.
 * Default value is TRUE.
 * If the value is set to FALSE, the Access Point will be hidden.
 *
 * @return LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_SetDiscoverable
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        bool discoverable
        ///< [IN] If TRUE the Access Point shows up on scans, else it is hidden.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set which IEEE standard to use.
 * Default hardware mode is IEEE 802.11g.
 *
 * @return
 *      - LE_BAD_PARAMETER if invalid IEEE standard is set.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_SetIeeeStandard
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_wifiAp_IeeeStdBitMask_t stdMask
        ///< [IN] Bit mask for the IEEE standard.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get which IEEE standard was set.
 * Default hardware mode is IEEE 802.11g.
 *
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_GetIeeeStandard
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_wifiAp_IeeeStdBitMask_t* stdMaskPtrPtr
        ///< [OUT] Bit mask for the IEEE standard.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set which WiFi Channel to use.
 * Default value is 7.
 * Some legal restrictions might apply for your region.
 * The channel number must be between 1 and 14 for IEEE 802.11b/g.
 * The channel number must be between 7 and 196 for IEEE 802.11a.
 * The channel number must be between 1 and 6 for IEEE 802.11ad.
 * @return
 *      - LE_OUT_OF_RANGE if requested channel number is out of range.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_SetChannel
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        uint16_t channelNumber
        ///< [IN] the channel number.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set what country code to use for regulatory domain.
 * ISO/IEC 3166-1 Alpha-2 code is used.
 * Default country code is US.
 * @return
 *      - LE_FAULT if the function failed.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_SetCountryCode
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        const char* LE_NONNULL countryCode
        ///< [IN] the country code.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set number of maximally allowed clients to connect to the Access Point at the same time.
 *
 * @return
 *      - LE_OUT_OF_RANGE if requested number of users exceeds the capabilities of the Access Point.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_SetMaxNumberOfClients
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        int8_t maxNumberOfClient
        ///< [IN] the maximum number of clients
);

//--------------------------------------------------------------------------------------------------
/**
 * Defines the IP adresses range for the host AP.
 *
 * @return
 *      - LE_BAD_PARAMETER if at least one of the given IP addresses is invalid.
 *      - LE_FAULT if a system call failed.
 *      - LE_OK if the function succeeded.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_wifiAp_SetIpRange
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        const char* LE_NONNULL ipAp,
        ///< [IN] the IP address of the Access Point.
        const char* LE_NONNULL ipStart,
        ///< [IN] the start IP address of the Access Point.
        const char* LE_NONNULL ipStop
        ///< [IN] the stop IP address of the Access Point.
);

#endif // LE_WIFIAP_COMMON_H_INCLUDE_GUARD
