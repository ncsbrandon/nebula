

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_WIFICLIENT_INTERFACE_H_INCLUDE_GUARD
#define LE_WIFICLIENT_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_wifiDefs_server.h"

// Internal includes for this interface
#include "le_wifiClient_common.h"
//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_wifiClient_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_wifiClient_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_wifiClient_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Reference type for AccessPoint that is returned by the WiFi Scan.
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * WiFi Client Events.
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * WiFi Client disconnect cause type enum.
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * WiFi Client Security Protocol for connection
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Handler for WiFi Client changes
 * @deprecated le_wifiClient_AddNewEventHandler() will be removed in near future.
 * It is replaced by le_wifiClient_AddConnectionEventHandler().
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_wifiClient_NewEvent'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * WiFi event indication structure. The disconnectionCause only applies to event EVENT_DISCONNECTED.
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Handler for WiFi Client connection changes
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_wifiClient_ConnectionEvent'
 */
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_wifiClient_NewEvent'
 *
 * This event provide information on WiFi Client event changes.
 * NewEvent will be deprecated.
 *
 */
//--------------------------------------------------------------------------------------------------
le_wifiClient_NewEventHandlerRef_t le_wifiClient_AddNewEventHandler
(
    le_wifiClient_NewEventHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_wifiClient_NewEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_wifiClient_RemoveNewEventHandler
(
    le_wifiClient_NewEventHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_wifiClient_ConnectionEvent'
 *
 * This event provide information on WiFi Client connection event changes.
 *
 */
//--------------------------------------------------------------------------------------------------
le_wifiClient_ConnectionEventHandlerRef_t le_wifiClient_AddConnectionEventHandler
(
    le_wifiClient_ConnectionEventHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_wifiClient_ConnectionEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_wifiClient_RemoveConnectionEventHandler
(
    le_wifiClient_ConnectionEventHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Start the WIFI device.
 *
 * @return
 *      - LE_OK     Function succeeded.
 *      - LE_FAULT  Function failed.
 *      - LE_BUSY   The WIFI device is already started.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_Start
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Stop the WIFI device.
 *
 * @return
 *      - LE_OK        Function succeeded.
 *      - LE_FAULT     Function failed.
 *      - LE_DUPLICATE The WIFI device is already stopped.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_Stop
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Start Scanning for WiFi Access points
 * Will result in event LE_WIFICLIENT_EVENT_SCAN_DONE when the scan results are available.
 *
 * @return
 *      - LE_OK     Function succeeded.
 *      - LE_FAULT  Function failed.
 *      - LE_BUSY   Scan already running.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_Scan
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the first WiFi Access Point found.
 *
 * @return
 *      - WiFi  Access Point reference if ok.
 *      - NULL  If no Access Point reference available.
 */
//--------------------------------------------------------------------------------------------------
le_wifiClient_AccessPointRef_t le_wifiClient_GetFirstAccessPoint
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the next WiFi Access Point.
 * Will return the Access Points in the order of found.
 * This function must be called in the same context as the GetFirstAccessPoint
 *
 * @return
 *      - WiFi  Access Point reference if ok.
 *      - NULL  If no Access Point reference available.
 */
//--------------------------------------------------------------------------------------------------
le_wifiClient_AccessPointRef_t le_wifiClient_GetNextAccessPoint
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the signal strength of the AccessPoint
 *
 * @return
 *      - Signal strength in dBm. Example -30 = -30dBm
 *      - If no signal available it will return LE_WIFICLIENT_NO_SIGNAL_STRENGTH
 *
 * @note The function returns the signal strength as reported at the time of the scan.
 */
//--------------------------------------------------------------------------------------------------
int16_t le_wifiClient_GetSignalStrength
(
    le_wifiClient_AccessPointRef_t accessPointRef
        ///< [IN] WiFi Access Point reference.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the Basic Service set identifier (BSSID) of the AccessPoint
 *
 * @return
 *         LE_OK            Function succeeded.
 *         LE_FAULT         Function failed.
 *         LE_BAD_PARAMETER Invalid parameter.
 *         LE_OVERFLOW      bssid buffer is too small to contain the BSSID.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_GetBssid
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    char* bssid,
        ///< [OUT] The BSSID
    size_t bssidSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the Service set identification (SSID) of the AccessPoint
 *
 * @return
 *        LE_OK            Function succeeded.
 *        LE_FAULT         Function failed.
 *        LE_BAD_PARAMETER Invalid parameter.
 *        LE_OVERFLOW      ssid buffer is too small to contain the SSID.
 *
 * @note The SSID does not have to be human readable ASCII values, but often is.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_GetSsid
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    uint8_t* ssidPtr,
        ///< [OUT] The SSID returned as a octet array.
    size_t* ssidSizePtr
        ///< [INOUT]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the currently selected connection to be established
 *
 * @return
 *      - LE_OK upon successful retrieval of the selected SSID to be connected
 *      - LE_FAULT upon failure to retrieve it
 */
//--------------------------------------------------------------------------------------------------
void le_wifiClient_GetCurrentConnection
(
    le_wifiClient_AccessPointRef_t* apRefPtr
        ///< [OUT] currently selected connection's AP reference
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the passphrase used to generate the PSK.
 *
 * @return
 *      - LE_OK            Function succeeded.
 *      - LE_FAULT         Function failed.
 *      - LE_BAD_PARAMETER Invalid parameter.
 *
 * @note The difference between le_wifiClient_SetPreSharedKey() and this function
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_SetPassphrase
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    const char* LE_NONNULL PassPhrase
        ///< [IN] pass-phrase for PSK
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the Pre Shared Key, PSK.
 *
 * @return
 *      - LE_OK             Function succeeded.
 *      - LE_FAULT          Function failed.
 *      - LE_BAD_PARAMETER  Invalid parameter.
 *
 * @note This is one way to authenticate against the access point. The other one is provided by the
 * le_wifiClient_SetPassPhrase() function. Both ways are exclusive and are effective only when used
 * with WPA-personal authentication.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_SetPreSharedKey
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    const char* LE_NONNULL PreSharedKey
        ///< [IN] PSK. Note the difference between PSK and
        ///< Pass Phrase.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the security protocol for connection
 *
 * @return
 *      - LE_OK             Function succeeded.
 *      - LE_FAULT          Function failed.
 *      - LE_BAD_PARAMETER  Invalid parameter.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_SetSecurityProtocol
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    le_wifiClient_SecurityProtocol_t securityProtocol
        ///< [IN] Security Mode
);



//--------------------------------------------------------------------------------------------------
/**
 * WPA-Enterprise requires a username and password to authenticate.
 * This function sets these parameters.
 *
 * @return
 *      - LE_OK             Function succeeded.
 *      - LE_FAULT          Function failed.
 *      - LE_BAD_PARAMETER  Invalid parameter.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_SetUserCredentials
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    const char* LE_NONNULL userName,
        ///< [IN] UserName used for WPA-Enterprise.
    const char* LE_NONNULL password
        ///< [IN] Password used for WPA-Enterprise.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the WEP key (WEP)
 *
 * @return
 *      - LE_OK     Function succeeded.
 *      - LE_FAULT  Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_SetWepKey
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    const char* LE_NONNULL wepKey
        ///< [IN] The WEP key
);



//--------------------------------------------------------------------------------------------------
/**
 * This function specifies whether the target Access Point is hiding its presence from clients or
 * not. When an Access Point is hidden, it cannot be discovered by a scan process.
 *
 * @return
 *      - LE_OK             Function succeeded.
 *      - LE_BAD_PARAMETER  Invalid parameter.
 *
 * @note By default, this attribute is not set which means that the client is unable to connect to
 * a hidden access point. When enabled, the client will be able to connect to the access point
 * whether it is hidden or not.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_SetHiddenNetworkAttribute
(
    le_wifiClient_AccessPointRef_t accessPointRef,
        ///< [IN] WiFi Access Point reference.
    bool hidden
        ///< [IN] If TRUE, the WIFI client will be able to connect to a hidden access point.
);



//--------------------------------------------------------------------------------------------------
/**
 * This function creates a reference to an Access Point given its SSID.
 * If an Access Point is hidden, it will not show up in the scan. So, its SSID must be known
 * in advance in order to create a reference.
 *
 * @return
 *      - AccessPoint reference to the current Access Point.
 *
 * @note This function fails if called while scan is running.
 */
//--------------------------------------------------------------------------------------------------
le_wifiClient_AccessPointRef_t le_wifiClient_Create
(
    const uint8_t* SsidPtr,
        ///< [IN] The SSID as a octet array.
    size_t SsidSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Deletes an accessPointRef.
 *
 * @return
 *      - LE_OK             Function succeeded.
 *      - LE_BAD_PARAMETER  Invalid parameter.
 *      - LE_BUSY           Function called during scan.
 *
 * @note The handle becomes invalid after it has been deleted.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_Delete
(
    le_wifiClient_AccessPointRef_t accessPointRef
        ///< [IN] WiFi Access Point reference.
);



//--------------------------------------------------------------------------------------------------
/**
 * Connect to the WiFi Access Point.
 * All authentication must be set prior to calling this function.
 *
 * @return
 *      - LE_OK             Function succeeded.
 *      - LE_BAD_PARAMETER  Invalid parameter.
 *
 * @note For PSK credentials see le_wifiClient_SetPassphrase() or le_wifiClient_SetPreSharedKey() .
 * @note For WPA-Enterprise credentials see le_wifiClient_SetUserCredentials()
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_Connect
(
    le_wifiClient_AccessPointRef_t accessPointRef
        ///< [IN] WiFi Access Point reference.
);



//--------------------------------------------------------------------------------------------------
/**
 * Disconnect from the current connected WiFi Access Point.
 *
 * @return
 *      - LE_OK     Function succeeded.
 *      - LE_FAULT  Function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_Disconnect
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Load the given SSID's configurations as it is selected as the connection to be established,
 * after creating for it an AP reference
 *
 * @return
 *      - LE_OK     Function succeeded.
 *      - LE_FAULT  Function failed.
 *      - LE_BAD_PARAMETER  Invalid parameter.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_LoadSsid
(
    const uint8_t* ssidPtr,
        ///< [IN] SSID which configs are to be installed
    size_t ssidSize,
        ///< [IN]
    le_wifiClient_AccessPointRef_t* apRefPtr
        ///< [OUT] reference to be created
);



//--------------------------------------------------------------------------------------------------
/**
 * Configure the given SSID to use WEP and the given WEP key in the respective input argument.
 * The WEP key is a mandatory input to be provided.
 *
 * @return
 *      - LE_OK     Succeeded to configure the givwn WEP key for the given SSID.
 *      - LE_FAULT  Failed to configure.
 *      - LE_BAD_PARAMETER  Invalid parameter.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_ConfigureWep
(
    const uint8_t* ssidPtr,
        ///< [IN] SSID which configs are to be installed
    size_t ssidSize,
        ///< [IN]
    const uint8_t* wepKeyPtr,
        ///< [IN] WEP key used for this SSID
    size_t wepKeySize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Configure the given SSID to use PSK and the given pass-phrase or pre-shared key in the
 * respective input arguments. The protocol input is mandatory and has to be set to either
 * LE_WIFICLIENT_SECURITY_WPA_PSK_PERSONAL or LE_WIFICLIENT_SECURITY_WPA2_PSK_PERSONAL.
 * Besides, it's mandatory to have at least one of the pass-phrase and pre-shared key supplied. If
 * both are provided as input, the pass-phrase has precedence and will be used. But it fails to
 * authenticate, a second attempt using the provided pre-shared key will not be done.
 *
 * @return
 *      - LE_OK     Succeeded to configure the given pass-phrase or pre-shared key for the given
 *                  SSID.
 *      - LE_FAULT  Failed to configure.
 *      - LE_BAD_PARAMETER  Invalid parameter.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_ConfigurePsk
(
    const uint8_t* ssidPtr,
        ///< [IN] SSID which configs are to be installed
    size_t ssidSize,
        ///< [IN]
    le_wifiClient_SecurityProtocol_t protocol,
        ///< [IN] security protocol WPA-PSK or WPA2-PSK
    const uint8_t* passPhrasePtr,
        ///< [IN] pass-phrase used for this SSID
    size_t passPhraseSize,
        ///< [IN]
    const uint8_t* pskPtr,
        ///< [IN] pre-shared key used for this SSID
    size_t pskSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Configure the given SSID to use EAP and the given EAP username and password in the respective
 * input arguments. The protocol input is mandatory and has to be set to either
 * LE_WIFICLIENT_SECURITY_WPA_EAP_PEAP0_ENTERPRISE or
 * LE_WIFICLIENT_SECURITY_WPA2_EAP_PEAP0_ENTERPRISE. Besides, both the username and password inputs
 * are mandatory.
 *
 * @return
 *      - LE_OK     Succeeded to configure the given EAP username and password for the given
 *                  SSID.
 *      - LE_FAULT  Failed to configure.
 *      - LE_BAD_PARAMETER  Invalid parameter.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_ConfigureEap
(
    const uint8_t* ssidPtr,
        ///< [IN] SSID which configs are to be installed
    size_t ssidSize,
        ///< [IN]
    le_wifiClient_SecurityProtocol_t protocol,
        ///< [IN] security protocol WPA-EAP or WPA2-EAP
    const uint8_t* usernamePtr,
        ///< [IN] EAP username used for this SSID
    size_t usernameSize,
        ///< [IN]
    const uint8_t* passwordPtr,
        ///< [IN] EAP password used for this SSID
    size_t passwordSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove and clear Wifi's security configurations to use with the given SSID from the config tree
 * and secured store. This includes the security protocol and all the username, password,
 * passphrase, pre-shared key, key, etc., previously configured via le_wifiClient_Configure APIs for
 * WEP, PSK and EAP.
 *
 * @return
 *      - LE_OK upon success to deconfigure the given SSID's configured user credentials;
 *        LE_FAULT otherwise
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiClient_RemoveSsidSecurityConfigs
(
    const uint8_t* ssidPtr,
        ///< [IN]  SSID which user credentials to be deconfigured
    size_t ssidSize
        ///< [IN]
);


#endif // LE_WIFICLIENT_INTERFACE_H_INCLUDE_GUARD
