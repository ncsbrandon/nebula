/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

#include "le_wifiAp_interface.h"
#include "le_wifiAp_messages.h"
#include "le_wifiAp_service.h"


//--------------------------------------------------------------------------------------------------
// Generic Client Types, Variables and Functions
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Client Thread Objects
 *
 * This object is used to contain thread specific data for each IPC client.
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    le_msg_SessionRef_t sessionRef;     ///< Client Session Reference
    int                 clientCount;    ///< Number of clients sharing this thread
    le_wifiAp_DisconnectHandler_t disconnectHandler; ///< Disconnect handler for this thread
    void*               contextPtr;     ///< Context for disconnect handler
}
_ClientThreadData_t;

//--------------------------------------------------------------------------------------------------
/**
 * Static pool for client threads.
 */
//--------------------------------------------------------------------------------------------------
LE_MEM_DEFINE_STATIC_POOL(le_wifiAp_ClientThreadData,
                          LE_CDATA_COMPONENT_COUNT,
                          sizeof(_ClientThreadData_t));


//--------------------------------------------------------------------------------------------------
/**
 * The memory pool for client thread objects
 */
//--------------------------------------------------------------------------------------------------
static le_mem_PoolRef_t _ClientThreadDataPool;


//--------------------------------------------------------------------------------------------------
/**
 * Key under which the pointer to the Thread Object (_ClientThreadData_t) will be kept in
 * thread-local storage.  This allows a thread to quickly get a pointer to its own Thread Object.
 */
//--------------------------------------------------------------------------------------------------
static pthread_key_t _ThreadDataKey;


//--------------------------------------------------------------------------------------------------
/**
 * This global flag is shared by all client threads, and is used to indicate whether the common
 * data has been initialized.
 *
 * @warning Use InitMutex, defined below, to protect accesses to this data.
 */
//--------------------------------------------------------------------------------------------------
static bool CommonDataInitialized = false;


//--------------------------------------------------------------------------------------------------
/**
 * Mutex and associated macros for use with the above CommonDataInitialized.
 */
//--------------------------------------------------------------------------------------------------
extern le_mutex_Ref_t le_ifgen_InitMutexRef;

/// Locks the mutex.
#define LOCK_INIT    le_mutex_Lock(le_ifgen_InitMutexRef);

/// Unlocks the mutex.
#define UNLOCK_INIT  le_mutex_Unlock(le_ifgen_InitMutexRef);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize thread specific data, and connect to the service for the current thread.
 *
 * @return
 *  - LE_OK if the client connected successfully to the service.
 *  - LE_UNAVAILABLE if the server is not currently offering the service to which the client is
 *    bound.
 *  - LE_NOT_PERMITTED if the client interface is not bound to any service (doesn't have a binding).
 *  - LE_COMM_ERROR if the Service Directory cannot be reached.
 */
//--------------------------------------------------------------------------------------------------
static le_result_t InitClientForThread
(
    bool isBlocking
)
{
    // Open a session.
    le_msg_SessionRef_t sessionRef;
    le_msg_ProtocolRef_t protocolRef;

    protocolRef = le_msg_GetProtocolRef(PROTOCOL_ID_STR, sizeof(_Message_t));
    sessionRef = le_msg_CreateSession(protocolRef, SERVICE_INSTANCE_NAME);
    le_result_t result = ifgen_le_wifiAp_OpenSession(sessionRef, isBlocking);
    if (result != LE_OK)
    {
        LE_DEBUG("Could not connect to '%s' service", SERVICE_INSTANCE_NAME);

        return result;
    }

    // Store the client sessionRef in thread-local storage, since each thread requires
    // its own sessionRef.
    _ClientThreadData_t* clientThreadPtr = le_mem_Alloc(_ClientThreadDataPool);
    memset(clientThreadPtr, 0, sizeof(_ClientThreadData_t));
    clientThreadPtr->sessionRef = sessionRef;
    if (pthread_setspecific(_ThreadDataKey, clientThreadPtr) != 0)
    {
        LE_FATAL("pthread_setspecific() failed!");
    }

    // This is the first client for the current thread
    clientThreadPtr->clientCount = 1;

    return LE_OK;
}


//--------------------------------------------------------------------------------------------------
/**
 * Get a pointer to the client thread data for the current thread.
 *
 * If the current thread does not have client data, then NULL is returned
 */
//--------------------------------------------------------------------------------------------------
static _ClientThreadData_t* GetClientThreadDataPtr
(
    void
)
{
    return pthread_getspecific(_ThreadDataKey);
}


//--------------------------------------------------------------------------------------------------
/**
 * Return the sessionRef for the current thread.
 *
 * If the current thread does not have a session ref, then this is a fatal error.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((unused)) static le_msg_SessionRef_t GetCurrentSessionRef
(
    void
)
{
    if (ifgen_le_wifiAp_HasLocalBinding())
    {
        return NULL;
    }
    else
    {
        _ClientThreadData_t* clientThreadPtr = GetClientThreadDataPtr();

        // If the thread specific data is NULL, then the session ref has not been created.
        LE_FATAL_IF(clientThreadPtr==NULL,
                    "le_wifiAp_ConnectService() not called for current thread");

        return clientThreadPtr->sessionRef;
    }
}


//--------------------------------------------------------------------------------------------------
/**
 * Init data that is common across all threads.
 */
//--------------------------------------------------------------------------------------------------
static void InitCommonData(void)
{
    // Perform common initialization across all instances of this API.
    ifgen_le_wifiAp_InitCommonData();

    // Allocate the client thread pool
    _ClientThreadDataPool = le_mem_InitStaticPool(le_wifiAp_ClientThreadData,
                                                  LE_CDATA_COMPONENT_COUNT,
                                                  sizeof(_ClientThreadData_t));

    // Create the thread-local data key to be used to store a pointer to each thread object.
    LE_ASSERT(pthread_key_create(&_ThreadDataKey, NULL) == 0);
}


//--------------------------------------------------------------------------------------------------
/**
 * Connect to the service, using either blocking or non-blocking calls.
 *
 * This function implements the details of the public ConnectService functions.
 *
 * @return
 *  - LE_OK if the client connected successfully to the service.
 *  - LE_UNAVAILABLE if the server is not currently offering the service to which the client is
 *    bound.
 *  - LE_NOT_PERMITTED if the client interface is not bound to any service (doesn't have a binding).
 *  - LE_COMM_ERROR if the Service Directory cannot be reached.
 */
//--------------------------------------------------------------------------------------------------
static le_result_t DoConnectService
(
    bool isBlocking
)
{
    // If this is the first time the function is called, init the client common data.
    LOCK_INIT
    if ( ! CommonDataInitialized )
    {
        InitCommonData();
        CommonDataInitialized = true;
    }
    UNLOCK_INIT

    _ClientThreadData_t* clientThreadPtr = GetClientThreadDataPtr();

    // If the thread specific data is NULL, then there is no current client session.
    if (clientThreadPtr == NULL)
    {
        le_result_t result;

        result = InitClientForThread(isBlocking);
        if ( result != LE_OK )
        {
            // Note that the blocking call will always return LE_OK
            return result;
        }

        LE_DEBUG("======= Starting client for '%s' service ========", SERVICE_INSTANCE_NAME);
    }
    else
    {
        // Keep track of the number of clients for the current thread.  There is only one
        // connection per thread, and it is shared by all clients.
        clientThreadPtr->clientCount++;
        LE_DEBUG("======= Starting another client for '%s' service ========",
                 SERVICE_INSTANCE_NAME);
    }

    return LE_OK;
}


//--------------------------------------------------------------------------------------------------
/**
 *
 * Connect the current client thread to the service providing this API. Block until the service is
 * available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void le_wifiAp_ConnectService
(
    void
)
{
    if (!ifgen_le_wifiAp_HasLocalBinding())
    {
        // Connect to the service; block until connected.
        DoConnectService(true);
    }
}

//--------------------------------------------------------------------------------------------------
/**
 *
 * Try to connect the current client thread to the service providing this API. Return with an error
 * if the service is not available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 *
 * @return
 *  - LE_OK if the client connected successfully to the service.
 *  - LE_UNAVAILABLE if the server is not currently offering the service to which the client is
 *    bound.
 *  - LE_NOT_PERMITTED if the client interface is not bound to any service (doesn't have a binding).
 *  - LE_COMM_ERROR if the Service Directory cannot be reached.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_wifiAp_TryConnectService
(
    void
)
{
    if (ifgen_le_wifiAp_HasLocalBinding())
    {
        return LE_OK;
    }
    else
    {
        // Connect to the service; return with an error if not connected.
        return DoConnectService(false);
    }
}

//--------------------------------------------------------------------------------------------------
// Session close handler.
//
// Dispatches session close notifications to the registered client handler function (if any)
//--------------------------------------------------------------------------------------------------
static void SessionCloseHandler
(
    le_msg_SessionRef_t sessionRef,
    void *contextPtr
)
{
    _ClientThreadData_t* clientThreadPtr = contextPtr;

    le_msg_DeleteSession( clientThreadPtr->sessionRef );

    // Need to delete the thread specific data, since it is no longer valid.  If a new
    // client session is started, new thread specific data will be allocated.
    le_mem_Release(clientThreadPtr);
    if (pthread_setspecific(_ThreadDataKey, NULL) != 0)
    {
        LE_FATAL("pthread_setspecific() failed!");
    }

    LE_DEBUG("======= '%s' service spontaneously disconnected ========", SERVICE_INSTANCE_NAME);

    if (clientThreadPtr->disconnectHandler)
    {
        clientThreadPtr->disconnectHandler(clientThreadPtr->contextPtr);
    }

    LE_FATAL("Component for le_wifiAp disconnected\n");
}

//--------------------------------------------------------------------------------------------------
/**
 * Set handler called when server disconnection is detected.
 *
 * When a server connection is lost, call this handler then exit with LE_FATAL.  If a program wants
 * to continue without exiting, it should call longjmp() from inside the handler.
 */
//--------------------------------------------------------------------------------------------------
void le_wifiAp_SetServerDisconnectHandler
(
    le_wifiAp_DisconnectHandler_t disconnectHandler,
    void *contextPtr
)
{
    if (ifgen_le_wifiAp_HasLocalBinding())
    {
        // Local bindings don't disconnect
        return;
    }

    _ClientThreadData_t* clientThreadPtr = GetClientThreadDataPtr();

    if (NULL == clientThreadPtr)
    {
        LE_CRIT("Trying to set disconnect handler for non-existent client session for '%s' service",
                SERVICE_INSTANCE_NAME);
    }
    else
    {
        clientThreadPtr->disconnectHandler = disconnectHandler;
        clientThreadPtr->contextPtr = contextPtr;

        if (disconnectHandler)
        {
            le_msg_SetSessionCloseHandler(clientThreadPtr->sessionRef,
                                          SessionCloseHandler,
                                          clientThreadPtr);
        }
    }
}


//--------------------------------------------------------------------------------------------------
/**
 *
 * Disconnect the current client thread from the service providing this API.
 *
 * Normally, this function doesn't need to be called. After this function is called, there's no
 * longer a connection to the service, and the functions in this API can't be used. For details, see
 * @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void le_wifiAp_DisconnectService
(
    void
)
{
    if (ifgen_le_wifiAp_HasLocalBinding())
    {
        // Cannot disconnect local bindings
        return;
    }

    _ClientThreadData_t* clientThreadPtr = GetClientThreadDataPtr();

    // If the thread specific data is NULL, then there is no current client session.
    if (clientThreadPtr == NULL)
    {
        LE_CRIT("Trying to stop non-existent client session for '%s' service",
                SERVICE_INSTANCE_NAME);
    }
    else
    {
        // This is the last client for this thread, so close the session.
        if ( clientThreadPtr->clientCount == 1 )
        {
            le_msg_DeleteSession( clientThreadPtr->sessionRef );

            // Need to delete the thread specific data, since it is no longer valid.  If a new
            // client session is started, new thread specific data will be allocated.
            le_mem_Release(clientThreadPtr);
            if (pthread_setspecific(_ThreadDataKey, NULL) != 0)
            {
                LE_FATAL("pthread_setspecific() failed!");
            }

            LE_DEBUG("======= Stopping client for '%s' service ========", SERVICE_INSTANCE_NAME);
        }
        else
        {
            // There is one less client sharing this thread's connection.
            clientThreadPtr->clientCount--;

            LE_DEBUG("======= Stopping another client for '%s' service ========",
                     SERVICE_INSTANCE_NAME);
        }
    }
}

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_wifiAp_NewEvent'
 *
 * These events provide information on WiFi Access Point
 *
 */
//--------------------------------------------------------------------------------------------------
le_wifiAp_NewEventHandlerRef_t le_wifiAp_AddNewEventHandler
(
    le_wifiAp_EventHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
)
{
    return ifgen_le_wifiAp_AddNewEventHandler(
        GetCurrentSessionRef(),
        handlerPtr,
        contextPtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_wifiAp_NewEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_wifiAp_RemoveNewEventHandler
(
    le_wifiAp_NewEventHandlerRef_t handlerRef
        ///< [IN]
)
{
     ifgen_le_wifiAp_RemoveNewEventHandler(
        GetCurrentSessionRef(),
        handlerRef
    );
}

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
le_result_t le_wifiAp_Start
(
    void
)
{
    return ifgen_le_wifiAp_Start(
        GetCurrentSessionRef()
    );
}

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
le_result_t le_wifiAp_Stop
(
    void
)
{
    return ifgen_le_wifiAp_Stop(
        GetCurrentSessionRef()
    );
}

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
le_result_t le_wifiAp_SetSsid
(
    const uint8_t* ssidPtr,
        ///< [IN] The SSID to set as a octet array.
    size_t ssidSize
        ///< [IN]
)
{
    return ifgen_le_wifiAp_SetSsid(
        GetCurrentSessionRef(),
        ssidPtr,
        ssidSize
    );
}

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
le_result_t le_wifiAp_SetSecurityProtocol
(
    le_wifiAp_SecurityProtocol_t securityProtocol
        ///< [IN] The security protocol to use.
)
{
    return ifgen_le_wifiAp_SetSecurityProtocol(
        GetCurrentSessionRef(),
        securityProtocol
    );
}

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
le_result_t le_wifiAp_SetPassPhrase
(
    const char* LE_NONNULL passPhrase
        ///< [IN] pass-phrase for PSK
)
{
    return ifgen_le_wifiAp_SetPassPhrase(
        GetCurrentSessionRef(),
        passPhrase
    );
}

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
le_result_t le_wifiAp_SetPreSharedKey
(
    const char* LE_NONNULL preSharedKey
        ///< [IN] PSK. Note the difference between PSK and Pass Phrase.
)
{
    return ifgen_le_wifiAp_SetPreSharedKey(
        GetCurrentSessionRef(),
        preSharedKey
    );
}

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
le_result_t le_wifiAp_SetDiscoverable
(
    bool discoverable
        ///< [IN] If TRUE the Access Point shows up on scans, else it is hidden.
)
{
    return ifgen_le_wifiAp_SetDiscoverable(
        GetCurrentSessionRef(),
        discoverable
    );
}

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
le_result_t le_wifiAp_SetIeeeStandard
(
    le_wifiAp_IeeeStdBitMask_t stdMask
        ///< [IN] Bit mask for the IEEE standard.
)
{
    return ifgen_le_wifiAp_SetIeeeStandard(
        GetCurrentSessionRef(),
        stdMask
    );
}

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
le_result_t le_wifiAp_GetIeeeStandard
(
    le_wifiAp_IeeeStdBitMask_t* stdMaskPtrPtr
        ///< [OUT] Bit mask for the IEEE standard.
)
{
    return ifgen_le_wifiAp_GetIeeeStandard(
        GetCurrentSessionRef(),
        stdMaskPtrPtr
    );
}

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
le_result_t le_wifiAp_SetChannel
(
    uint16_t channelNumber
        ///< [IN] the channel number.
)
{
    return ifgen_le_wifiAp_SetChannel(
        GetCurrentSessionRef(),
        channelNumber
    );
}

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
le_result_t le_wifiAp_SetCountryCode
(
    const char* LE_NONNULL countryCode
        ///< [IN] the country code.
)
{
    return ifgen_le_wifiAp_SetCountryCode(
        GetCurrentSessionRef(),
        countryCode
    );
}

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
le_result_t le_wifiAp_SetMaxNumberOfClients
(
    int8_t maxNumberOfClient
        ///< [IN] the maximum number of clients
)
{
    return ifgen_le_wifiAp_SetMaxNumberOfClients(
        GetCurrentSessionRef(),
        maxNumberOfClient
    );
}

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
le_result_t le_wifiAp_SetIpRange
(
    const char* LE_NONNULL ipAp,
        ///< [IN] the IP address of the Access Point.
    const char* LE_NONNULL ipStart,
        ///< [IN] the start IP address of the Access Point.
    const char* LE_NONNULL ipStop
        ///< [IN] the stop IP address of the Access Point.
)
{
    return ifgen_le_wifiAp_SetIpRange(
        GetCurrentSessionRef(),
        ipAp,
        ipStart,
        ipStop
    );
}
