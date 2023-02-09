/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

#include "query_interface.h"
#include "query_messages.h"
#include "query_service.h"


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
    query_DisconnectHandler_t disconnectHandler; ///< Disconnect handler for this thread
    void*               contextPtr;     ///< Context for disconnect handler
}
_ClientThreadData_t;

//--------------------------------------------------------------------------------------------------
/**
 * Static pool for client threads.
 */
//--------------------------------------------------------------------------------------------------
LE_MEM_DEFINE_STATIC_POOL(query_ClientThreadData,
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
    le_result_t result = ifgen_query_OpenSession(sessionRef, isBlocking);
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
    if (ifgen_query_HasLocalBinding())
    {
        return NULL;
    }
    else
    {
        _ClientThreadData_t* clientThreadPtr = GetClientThreadDataPtr();

        // If the thread specific data is NULL, then the session ref has not been created.
        LE_FATAL_IF(clientThreadPtr==NULL,
                    "query_ConnectService() not called for current thread");

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
    ifgen_query_InitCommonData();

    // Allocate the client thread pool
    _ClientThreadDataPool = le_mem_InitStaticPool(query_ClientThreadData,
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
void query_ConnectService
(
    void
)
{
    if (!ifgen_query_HasLocalBinding())
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
le_result_t query_TryConnectService
(
    void
)
{
    if (ifgen_query_HasLocalBinding())
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

    LE_FATAL("Component for query disconnected\n");
}

//--------------------------------------------------------------------------------------------------
/**
 * Set handler called when server disconnection is detected.
 *
 * When a server connection is lost, call this handler then exit with LE_FATAL.  If a program wants
 * to continue without exiting, it should call longjmp() from inside the handler.
 */
//--------------------------------------------------------------------------------------------------
void query_SetServerDisconnectHandler
(
    query_DisconnectHandler_t disconnectHandler,
    void *contextPtr
)
{
    if (ifgen_query_HasLocalBinding())
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
void query_DisconnectService
(
    void
)
{
    if (ifgen_query_HasLocalBinding())
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
 * Read data out of a buffer.  Data is written to a given file descriptor in JSON-encoded format
 * as an array of objects containing a timestamp and a value (or just a timestamp for triggers).
 * E.g.,
 *
 * @code
 * [{"t":1537483647.125,"v":true},{"t":1537483657.128,"v":true}]
 * @endcode
 *
 * @return
 *  - LE_OK if the read operation started successfully.
 *  - LE_NOT_FOUND if the Observation doesn't exist.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_ReadBufferJson
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startAfter,
        ///< [IN] Start after this many seconds ago,
        ///< or after an absolute number of seconds since the Epoch
        ///< (if startafter > 30 years).
        ///< Use NAN (not a number) to read the whole buffer.
    int outputFile,
        ///< [IN] File descriptor to write the data to.
    query_ReadCompletionFunc_t completionFuncPtr,
        ///< [IN] Completion callback to be called when operation finishes.
    void* contextPtr
        ///< [IN]
)
{
    return ifgen_query_ReadBufferJson(
        GetCurrentSessionRef(),
        obsPath,
        startAfter,
        outputFile,
        completionFuncPtr,
        contextPtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Read the timestamp of a single sample from a buffer.
 *
 * @note This can be used with any type of sample.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the Observation doesn't exist or does not have a sample newer than the given
 *                 startAfter timestamp.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_ReadBufferSampleTimestamp
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startAfter,
        ///< [IN] Start after this many seconds ago,
        ///< or after an absolute number of seconds since the Epoch
        ///< (if startafter > 30 years).
        ///< Use NAN (not a number) to read the oldest sample in the buffer.
    double* timestampPtr
        ///< [OUT] Timestamp of the sample, if LE_OK returned.
)
{
    return ifgen_query_ReadBufferSampleTimestamp(
        GetCurrentSessionRef(),
        obsPath,
        startAfter,
        timestampPtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Read a single Boolean sample from a buffer.
 *
 * @warning This can only be used with Boolean type samples.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the Observation doesn't exist or does not have a Boolean sample newer than
 *                 the given startAfter timestamp.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_ReadBufferSampleBoolean
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startAfter,
        ///< [IN] Start after this many seconds ago,
        ///< or after an absolute number of seconds since the Epoch
        ///< (if startafter > 30 years).
        ///< Use NAN (not a number) to read the oldest sample in the buffer.
    double* timestampPtr,
        ///< [OUT] Timestamp of the sample, if LE_OK returned.
    bool* valuePtr
        ///< [OUT] Value of the sample, if LE_OK returned.
)
{
    return ifgen_query_ReadBufferSampleBoolean(
        GetCurrentSessionRef(),
        obsPath,
        startAfter,
        timestampPtr,
        valuePtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Read a single numeric sample from a buffer.
 *
 * @warning This can only be used with numeric type samples.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the Observation doesn't exist or does not have a Numeric sample newer than
 *                 the given startAfter timestamp.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_ReadBufferSampleNumeric
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startAfter,
        ///< [IN] Start after this many seconds ago,
        ///< or after an absolute number of seconds since the Epoch
        ///< (if startafter > 30 years).
        ///< Use NAN (not a number) to read the oldest sample in the buffer.
    double* timestampPtr,
        ///< [OUT] Timestamp of the sample, if LE_OK returned.
    double* valuePtr
        ///< [OUT] Value of the sample, if LE_OK returned.
)
{
    return ifgen_query_ReadBufferSampleNumeric(
        GetCurrentSessionRef(),
        obsPath,
        startAfter,
        timestampPtr,
        valuePtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Read a single sample from a buffer as a string.
 *
 * @note This can be used with any type of sample.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the Observation doesn't exist or does not have a sample newer than the given
 *                 startAfter timestamp.
 *  - LE_OVERFLOW if the buffer provided is too small to hold the value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_ReadBufferSampleString
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startAfter,
        ///< [IN] Start after this many seconds ago,
        ///< or after an absolute number of seconds since the Epoch
        ///< (if startafter > 30 years).
        ///< Use NAN (not a number) to read the oldest sample in the buffer.
    double* timestampPtr,
        ///< [OUT] Timestamp of the sample, if LE_OK returned.
    char* value,
        ///< [OUT] Value of the sample, if LE_OK returned.
    size_t valueSize
        ///< [IN]
)
{
    return ifgen_query_ReadBufferSampleString(
        GetCurrentSessionRef(),
        obsPath,
        startAfter,
        timestampPtr,
        value,
        valueSize
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Read a single sample from a buffer as JSON.
 *
 * @note This can be used with any type of sample.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the Observation doesn't exist or does not have a sample newer than the given
 *                 startAfter timestamp.
 *  - LE_OVERFLOW if the buffer provided is too small to hold the value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_ReadBufferSampleJson
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startAfter,
        ///< [IN] Start after this many seconds ago,
        ///< or after an absolute number of seconds since the Epoch
        ///< (if startafter > 30 years).
        ///< Use NAN (not a number) to read the oldest sample in the buffer.
    double* timestampPtr,
        ///< [OUT] Timestamp of the sample, if LE_OK returned.
    char* value,
        ///< [OUT] Value of the sample, if LE_OK returned.
    size_t valueSize
        ///< [IN]
)
{
    return ifgen_query_ReadBufferSampleJson(
        GetCurrentSessionRef(),
        obsPath,
        startAfter,
        timestampPtr,
        value,
        valueSize
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the minimum value found in an Observation's data set within a given time span.
 *
 * @return The value, or NAN (not-a-number) if there's no numerical data in the Observation's
 *         buffer (if the buffer size is zero, the buffer is empty, or the buffer contains data
 *         of a non-numerical type).
 */
//--------------------------------------------------------------------------------------------------
double query_GetMin
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startTime
        ///< [IN] If < 30 years then seconds before now; else seconds since the Epoch.
)
{
    return ifgen_query_GetMin(
        GetCurrentSessionRef(),
        obsPath,
        startTime
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the maximum value found within a given time span in an Observation's buffer.
 *
 * @return The value, or NAN (not-a-number) if there's no numerical data in the Observation's
 *         buffer (if the buffer size is zero, the buffer is empty, or the buffer contains data
 *         of a non-numerical type).
 */
//--------------------------------------------------------------------------------------------------
double query_GetMax
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startTime
        ///< [IN] If < 30 years then seconds before now; else seconds since the Epoch.
)
{
    return ifgen_query_GetMax(
        GetCurrentSessionRef(),
        obsPath,
        startTime
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the mean (average) of all values found within a given time span in an Observation's buffer.
 *
 * @return The value, or NAN (not-a-number) if there's no numerical data in the Observation's
 *         buffer (if the buffer size is zero, the buffer is empty, or the buffer contains data
 *         of a non-numerical type).
 */
//--------------------------------------------------------------------------------------------------
double query_GetMean
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startTime
        ///< [IN] If < 30 years then seconds before now; else seconds since the Epoch.
)
{
    return ifgen_query_GetMean(
        GetCurrentSessionRef(),
        obsPath,
        startTime
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the standard deviation of all values found within a given time span in an
 * Observation's buffer.
 *
 * @return The value, or NAN (not-a-number) if there's no numerical data in the Observation's
 *         buffer (if the buffer size is zero, the buffer is empty, or the buffer contains data
 *         of a non-numerical type).
 */
//--------------------------------------------------------------------------------------------------
double query_GetStdDev
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startTime
        ///< [IN] If < 30 years then seconds before now; else seconds since the Epoch.
)
{
    return ifgen_query_GetStdDev(
        GetCurrentSessionRef(),
        obsPath,
        startTime
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the current data type of a resource.
 *
 * @note Observations and Placeholders will default to IO_DATA_TYPE_TRIGGER, but will change
 *       types as other types of data are pushed to them.  The data types of Inputs and Outputs
 *       are decided by the apps that create them.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetDataType
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    io_DataType_t* dataTypePtr
        ///< [OUT] The fetched data type, if LE_OK is returned.
)
{
    return ifgen_query_GetDataType(
        GetCurrentSessionRef(),
        path,
        dataTypePtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the current units of a resource.
 *
 * @note Observations and Placeholders will default to "", but will change units as data is
 *       pushed to them.  The units of Inputs and Outputs are decided by the apps that create them.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 *  - LE_OVERFLOW if the units string was truncated because it is larger than the buffer provided.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetUnits
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    char* units,
        ///< [OUT] The fetched units, if LE_OK is returned.
    size_t unitsSize
        ///< [IN]
)
{
    return ifgen_query_GetUnits(
        GetCurrentSessionRef(),
        path,
        units,
        unitsSize
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the timestamp of the current value of a resource.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 *  - LE_UNAVAILABLE if the resource doesn't have a current value (yet).
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetTimestamp
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    double* timestampPtr
        ///< [OUT] The fetched timestamp (in seconds since the Epoch), if LE_OK returned.
)
{
    return ifgen_query_GetTimestamp(
        GetCurrentSessionRef(),
        path,
        timestampPtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the current value of a resource, if it's Boolean type.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 *  - LE_UNAVAILABLE if the resource doesn't have a current value (yet).
 *  - LE_FORMAT_ERROR if the resource has another data type.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetBoolean
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    double* timestampPtr,
        ///< [OUT] Fetched timestamp (in seconds since the Epoch), if LE_OK returned.
    bool* valuePtr
        ///< [OUT] Fetched value, if LE_OK returned.
)
{
    return ifgen_query_GetBoolean(
        GetCurrentSessionRef(),
        path,
        timestampPtr,
        valuePtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the current value of a resource, if it's numeric type.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 *  - LE_UNAVAILABLE if the resource doesn't have a current value (yet).
 *  - LE_FORMAT_ERROR if the resource has another data type.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetNumeric
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    double* timestampPtr,
        ///< [OUT] Fetched timestamp (in seconds since the Epoch), if LE_OK returned.
    double* valuePtr
        ///< [OUT] Fetched value, if LE_OK returned.
)
{
    return ifgen_query_GetNumeric(
        GetCurrentSessionRef(),
        path,
        timestampPtr,
        valuePtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the current value of a resource, if it's a string type.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 *  - LE_UNAVAILABLE if the resource doesn't have a current value (yet).
 *  - LE_FORMAT_ERROR if the resource has another data type.
 *  - LE_OVERFLOW if the value was truncated because it is larger than the buffer provided.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetString
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    double* timestampPtr,
        ///< [OUT] Fetched timestamp (in seconds since the Epoch), if LE_OK returned.
    char* value,
        ///< [OUT] Fetched value, if LE_OK returned.
    size_t valueSize
        ///< [IN]
)
{
    return ifgen_query_GetString(
        GetCurrentSessionRef(),
        path,
        timestampPtr,
        value,
        valueSize
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the current value of a resource of any type, in JSON format.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 *  - LE_UNAVAILABLE if the resource doesn't have a current value (yet).
 *  - LE_OVERFLOW if the value was truncated because it is larger than the buffer provided.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetJson
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    double* timestampPtr,
        ///< [OUT] Fetched timestamp (in seconds since the Epoch), if LE_OK returned.
    char* value,
        ///< [OUT] Fetched value, if LE_OK returned.
    size_t valueSize
        ///< [IN]
)
{
    return ifgen_query_GetJson(
        GetCurrentSessionRef(),
        path,
        timestampPtr,
        value,
        valueSize
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Fetch the example JSON value string for a given Input resource.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to something that doesn't have a JSON type.
 *  - LE_UNAVAILABLE if the JSON-type resource doesn't have an example value.
 *  - LE_OVERFLOW if the value was truncated because it is larger than the buffer provided.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetJsonExample
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    char* example,
        ///< [OUT] Example will be put here, if LE_OK returned.
    size_t exampleSize
        ///< [IN]
)
{
    return ifgen_query_GetJsonExample(
        GetCurrentSessionRef(),
        path,
        example,
        exampleSize
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'query_TriggerPush'
 */
//--------------------------------------------------------------------------------------------------
query_TriggerPushHandlerRef_t query_AddTriggerPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    query_TriggerPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
)
{
    return ifgen_query_AddTriggerPushHandler(
        GetCurrentSessionRef(),
        path,
        callbackPtr,
        contextPtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'query_TriggerPush'
 */
//--------------------------------------------------------------------------------------------------
void query_RemoveTriggerPushHandler
(
    query_TriggerPushHandlerRef_t handlerRef
        ///< [IN]
)
{
     ifgen_query_RemoveTriggerPushHandler(
        GetCurrentSessionRef(),
        handlerRef
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'query_BooleanPush'
 */
//--------------------------------------------------------------------------------------------------
query_BooleanPushHandlerRef_t query_AddBooleanPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    query_BooleanPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
)
{
    return ifgen_query_AddBooleanPushHandler(
        GetCurrentSessionRef(),
        path,
        callbackPtr,
        contextPtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'query_BooleanPush'
 */
//--------------------------------------------------------------------------------------------------
void query_RemoveBooleanPushHandler
(
    query_BooleanPushHandlerRef_t handlerRef
        ///< [IN]
)
{
     ifgen_query_RemoveBooleanPushHandler(
        GetCurrentSessionRef(),
        handlerRef
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'query_NumericPush'
 */
//--------------------------------------------------------------------------------------------------
query_NumericPushHandlerRef_t query_AddNumericPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    query_NumericPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
)
{
    return ifgen_query_AddNumericPushHandler(
        GetCurrentSessionRef(),
        path,
        callbackPtr,
        contextPtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'query_NumericPush'
 */
//--------------------------------------------------------------------------------------------------
void query_RemoveNumericPushHandler
(
    query_NumericPushHandlerRef_t handlerRef
        ///< [IN]
)
{
     ifgen_query_RemoveNumericPushHandler(
        GetCurrentSessionRef(),
        handlerRef
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'query_StringPush'
 */
//--------------------------------------------------------------------------------------------------
query_StringPushHandlerRef_t query_AddStringPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    query_StringPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
)
{
    return ifgen_query_AddStringPushHandler(
        GetCurrentSessionRef(),
        path,
        callbackPtr,
        contextPtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'query_StringPush'
 */
//--------------------------------------------------------------------------------------------------
void query_RemoveStringPushHandler
(
    query_StringPushHandlerRef_t handlerRef
        ///< [IN]
)
{
     ifgen_query_RemoveStringPushHandler(
        GetCurrentSessionRef(),
        handlerRef
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'query_JsonPush'
 */
//--------------------------------------------------------------------------------------------------
query_JsonPushHandlerRef_t query_AddJsonPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    query_JsonPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
)
{
    return ifgen_query_AddJsonPushHandler(
        GetCurrentSessionRef(),
        path,
        callbackPtr,
        contextPtr
    );
}

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'query_JsonPush'
 */
//--------------------------------------------------------------------------------------------------
void query_RemoveJsonPushHandler
(
    query_JsonPushHandlerRef_t handlerRef
        ///< [IN]
)
{
     ifgen_query_RemoveJsonPushHandler(
        GetCurrentSessionRef(),
        handlerRef
    );
}
