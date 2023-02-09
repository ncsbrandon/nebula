/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

#include "le_wifiAp_common.h"
#include "le_wifiAp_messages.h"


//--------------------------------------------------------------------------------------------------
/**
 * Client Data Objects
 *
 * This object is used for each registered handler.  This is needed since we are not using
 * events, but are instead queueing functions directly with the event loop.
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    void                    *handlerPtr;        ///< Registered handler function
    void                    *contextPtr;        ///< ContextPtr registered with handler
    le_event_HandlerRef_t    handlerRef;        ///< HandlerRef for the registered handler
    le_thread_Ref_t          callersThreadRef;  ///< Caller's thread.
}
_ClientData_t;


//--------------------------------------------------------------------------------------------------
/**
 * Default expected maximum simultaneous client data items.
 */
//--------------------------------------------------------------------------------------------------
#define HIGH_CLIENT_DATA_COUNT   2


//--------------------------------------------------------------------------------------------------
/**
 * Static memory pool for client data
 */
//--------------------------------------------------------------------------------------------------
LE_MEM_DEFINE_STATIC_POOL(le_wifiAp_ClientData,
                          HIGH_CLIENT_DATA_COUNT,
                          sizeof(_ClientData_t));


//--------------------------------------------------------------------------------------------------
/**
 * The memory pool for client data objects
 */
//--------------------------------------------------------------------------------------------------
static le_mem_PoolRef_t _ClientDataPool;


//--------------------------------------------------------------------------------------------------
/**
 * Static safe reference map for use with Add/Remove handler references
 */
//--------------------------------------------------------------------------------------------------
LE_REF_DEFINE_STATIC_MAP(le_wifiAp_ClientHandlers,
    LE_MEM_BLOCKS(le_wifiAp_ClientData, HIGH_CLIENT_DATA_COUNT));


//--------------------------------------------------------------------------------------------------
/**
 * Safe Reference Map for use with Add/Remove handler references
 *
 * @warning Use _Mutex, defined below, to protect accesses to this data.
 */
//--------------------------------------------------------------------------------------------------
static le_ref_MapRef_t _HandlerRefMap;


//--------------------------------------------------------------------------------------------------
/**
 * Mutex and associated macros for use with the above HandlerRefMap.
 *
 * Unused attribute is needed because this variable may not always get used.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((unused)) static pthread_mutex_t _Mutex = PTHREAD_MUTEX_INITIALIZER;   // POSIX "Fast" mutex.

/// Locks the mutex.
#define _LOCK    LE_ASSERT(pthread_mutex_lock(&_Mutex) == 0);

/// Unlocks the mutex.
#define _UNLOCK  LE_ASSERT(pthread_mutex_unlock(&_Mutex) == 0);


//--------------------------------------------------------------------------------------------------
/**
 * Trace reference used for controlling tracing in this module.
 */
//--------------------------------------------------------------------------------------------------
#if defined(MK_TOOLS_BUILD) && !defined(NO_LOG_SESSION)

static le_log_TraceRef_t TraceRef;

/// Macro used to generate trace output in this module.
/// Takes the same parameters as LE_DEBUG() et. al.
#define TRACE(...) LE_TRACE(TraceRef, ##__VA_ARGS__)

/// Macro used to query current trace state in this module
#define IS_TRACE_ENABLED LE_IS_TRACE_ENABLED(TraceRef)

#else

#define TRACE(...)
#define IS_TRACE_ENABLED 0

#endif

//--------------------------------------------------------------------------------------------------
/**
 * Message to call when unsolicited message (e.g. callback) is received from server.
 */
//--------------------------------------------------------------------------------------------------
static void ClientIndicationRecvHandler(le_msg_MessageRef_t  msgRef,
                                        void*                contextPtr);

//--------------------------------------------------------------------------------------------------
/**
 * Get if this client bound locally.
 *
 * If using this version of the function, it's a remote binding.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED bool ifgen_le_wifiAp_HasLocalBinding
(
    void
)
{
    return false;
}

//--------------------------------------------------------------------------------------------------
/**
 * Init data that is common across all threads
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED void ifgen_le_wifiAp_InitCommonData
(
    void
)
{
    // Make sure each entry is only initialized once
    _LOCK;
    {
        if (!_ClientDataPool)
        {
            // Allocate the client data pool
            _ClientDataPool = le_mem_InitStaticPool(le_wifiAp_ClientData,
                                                    HIGH_CLIENT_DATA_COUNT,
                                                    sizeof(_ClientData_t));
        }


        if (!_HandlerRefMap)
        {
            // Create safe reference map for handler references.
            // The size of the map should be based on the number of handlers defined multiplied by
            // the number of client threads.  Since this number can't be completely determined at
            // build time, just make a reasonable guess.
            _HandlerRefMap = le_ref_InitStaticMap(le_wifiAp_ClientHandlers,
                                                  LE_MEM_BLOCKS(le_wifiAp_ClientData,
                                                                HIGH_CLIENT_DATA_COUNT));
        }

#if defined(MK_TOOLS_BUILD) && !defined(NO_LOG_SESSION)
        // Get a reference to the trace keyword that is used to control tracing in this module.
        if (!TraceRef)
        {
            TraceRef = le_log_GetTraceRef("ipc");
        }
#endif
    }
    _UNLOCK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Perform common initialization and open a session
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_OpenSession
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    bool isBlocking
)
{
    le_msg_SetSessionRecvHandler(_ifgen_sessionRef, ClientIndicationRecvHandler, NULL);

    if ( isBlocking )
    {
        le_msg_OpenSessionSync(_ifgen_sessionRef);
    }
    else
    {
        le_result_t result;

        result = le_msg_TryOpenSessionSync(_ifgen_sessionRef);
        if ( result != LE_OK )
        {
            le_msg_DeleteSession(_ifgen_sessionRef);

            switch (result)
            {
                case LE_UNAVAILABLE:
                    LE_DEBUG("Service not offered");
                    break;

                case LE_NOT_PERMITTED:
                    LE_DEBUG("Missing binding");
                    break;

                case LE_COMM_ERROR:
                    LE_DEBUG("Can't reach ServiceDirectory");
                    break;

                default:
                    LE_CRIT("le_msg_TryOpenSessionSync() returned unexpected result code %d (%s)",
                            result,
                            LE_RESULT_TXT(result));
                    break;
            }

            return result;
        }
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
// Client Specific Client Code
//--------------------------------------------------------------------------------------------------


// This function parses the message buffer received from the server, and then calls the user
// registered handler, which is stored in a client data object.
static void _Handle_ifgen_le_wifiAp_AddNewEventHandler
(
    void* _reportPtr,
    void* _dataPtr
)
{
    le_msg_MessageRef_t _msgRef = _reportPtr;
    _Message_t* _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    uint8_t* _msgBufPtr = _msgPtr->buffer;

    // The client data pointer is passed in as a parameter, since the lookup in the safe ref map
    // and check for NULL has already been done when this function is queued.
    _ClientData_t* _clientDataPtr = _dataPtr;

    // Declare temporaries for input parameters
    le_wifiAp_Event_t event = (le_wifiAp_Event_t) 0;

    // Pull out additional data from the client data pointer
    le_wifiAp_EventHandlerFunc_t _handlerRef_ifgen_le_wifiAp_AddNewEventHandler =
        (le_wifiAp_EventHandlerFunc_t)_clientDataPtr->handlerPtr;
    void* contextPtr = _clientDataPtr->contextPtr;

    // The clientContextPtr always exists and is always first. It is a safe reference to the client
    // data object, but we already get the pointer to the client data object through the _dataPtr
    // parameter, so we don't need to do anything with clientContextPtr, other than unpacking it.
    void* _clientContextPtr;
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                  &_clientContextPtr ))
    {
        goto error_unpack;
    }

    // Unpack the remaining parameters.
    if (!le_wifiAp_UnpackEvent( &_msgBufPtr,
                                               &event ))
    {
        goto error_unpack;
    }

    // Call the registered handler
    if ( _handlerRef_ifgen_le_wifiAp_AddNewEventHandler != NULL )
    {
        _handlerRef_ifgen_le_wifiAp_AddNewEventHandler(event, contextPtr );
    }
    else
    {
        LE_FATAL("Error in client data: no registered handler");
    }

    // Release the message, now that we are finished with it.
    le_msg_ReleaseMsg(_msgRef);

    return;

error_unpack:
    // Handle any unpack errors by dying -- server should not be sending invalid data; if it is
    // something is seriously wrong.
    LE_FATAL("Error unpacking message");
}


//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_wifiAp_NewEvent'
 *
 * These events provide information on WiFi Access Point
 *
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED le_wifiAp_NewEventHandlerRef_t ifgen_le_wifiAp_AddNewEventHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    le_wifiAp_EventHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_wifiAp_NewEventHandlerRef_t _result =
        NULL;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_AddNewEventHandler;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    // The handlerPtr and contextPtr input parameters are stored in the client
    // data object, and it is a safe reference to this object that is passed down
    // as the context pointer.  The handlerPtr is not passed down.
    // Create a new client data object and fill it in
    _ClientData_t* _clientDataPtr = le_mem_Alloc(_ClientDataPool);
    _clientDataPtr->handlerPtr = handlerPtr;
    _clientDataPtr->contextPtr = contextPtr;
    _clientDataPtr->callersThreadRef = le_thread_GetCurrent();
    // Create a safeRef to be passed down as the contextPtr
    _LOCK
    contextPtr = le_ref_CreateRef(_HandlerRefMap, _clientDataPtr);
    _UNLOCK
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, contextPtr ));

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackReference( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    if (_result)
    {
        // Put the handler reference result into the client data object, and
        // then return a safe reference to the client data object as the reference;
        // this safe reference is contained in the contextPtr, which was assigned
        // when the client data object was created.
        _clientDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        _result = contextPtr;
    }
    else
    {
        // Add failed, release the client data.
        le_mem_Release(_clientDataPtr);
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
}


//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_wifiAp_NewEvent'
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED void ifgen_le_wifiAp_RemoveNewEventHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    le_wifiAp_NewEventHandlerRef_t handlerRef
        ///< [IN]
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_RemoveNewEventHandler;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    // The passed in handlerRef is a safe reference for the client data object.  Need to get the
    // real handlerRef from the client data object and then delete both the safe reference and
    // the object since they are no longer needed.
    _LOCK
    _ClientData_t* clientDataPtr = le_ref_Lookup(_HandlerRefMap, handlerRef);
    LE_FATAL_IF(clientDataPtr==NULL, "Invalid reference");
    le_ref_DeleteRef(_HandlerRefMap, handlerRef);
    _UNLOCK
    handlerRef = (le_wifiAp_NewEventHandlerRef_t)
         clientDataPtr->handlerRef;
    le_mem_Release(clientDataPtr);
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr,
                                     handlerRef ));

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);

    return;
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_Start
(
    le_msg_SessionRef_t _ifgen_sessionRef
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_Start;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_Stop
(
    le_msg_SessionRef_t _ifgen_sessionRef
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_Stop;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_SetSsid
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    const uint8_t* ssidPtr,
        ///< [IN] The SSID to set as a octet array.
    size_t ssidSize
        ///< [IN]
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate
    if ( (NULL == ssidPtr) &&
         (0 != ssidSize) )
    {
        LE_FATAL("If ssidPtr is NULL "
                 "ssidSize must be zero");
    }
    if ( ssidSize > 33 )
    {
        LE_FATAL("ssidSize > 33");
    }


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_SetSsid;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    bool ssidResult;
            LE_PACK_PACKARRAY( &_msgBufPtr,
                       ssidPtr, ssidSize,
                       33, le_pack_PackUint8,
                       &ssidResult );
    LE_ASSERT(ssidResult);

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_SetSecurityProtocol
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    le_wifiAp_SecurityProtocol_t securityProtocol
        ///< [IN] The security protocol to use.
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_SetSecurityProtocol;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    LE_ASSERT(le_wifiAp_PackSecurityProtocol( &_msgBufPtr,
                                                  securityProtocol ));

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_SetPassPhrase
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    const char* LE_NONNULL passPhrase
        ///< [IN] pass-phrase for PSK
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate
    if ( strnlen(passPhrase, 64) > 64 )
    {
        LE_FATAL("strnlen(passPhrase, 64) > 64");
    }


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_SetPassPhrase;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    LE_ASSERT(le_pack_PackString( &_msgBufPtr,
                                  passPhrase, 64 ));

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_SetPreSharedKey
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    const char* LE_NONNULL preSharedKey
        ///< [IN] PSK. Note the difference between PSK and Pass Phrase.
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate
    if ( strnlen(preSharedKey, 65) > 65 )
    {
        LE_FATAL("strnlen(preSharedKey, 65) > 65");
    }


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_SetPreSharedKey;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    LE_ASSERT(le_pack_PackString( &_msgBufPtr,
                                  preSharedKey, 65 ));

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_SetDiscoverable
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    bool discoverable
        ///< [IN] If TRUE the Access Point shows up on scans, else it is hidden.
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_SetDiscoverable;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    LE_ASSERT(le_pack_PackBool( &_msgBufPtr,
                                                  discoverable ));

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_SetIeeeStandard
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    le_wifiAp_IeeeStdBitMask_t stdMask
        ///< [IN] Bit mask for the IEEE standard.
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_SetIeeeStandard;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    LE_ASSERT(le_wifiAp_PackIeeeStdBitMask( &_msgBufPtr,
                                                  stdMask ));

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_GetIeeeStandard
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    le_wifiAp_IeeeStdBitMask_t* stdMaskPtrPtr
        ///< [OUT] Bit mask for the IEEE standard.
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_GetIeeeStandard;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.
    uint32_t _requiredOutputs = 0;
    _requiredOutputs |= ((!!(stdMaskPtrPtr)) << 0);
    LE_ASSERT(le_pack_PackUint32(&_msgBufPtr, _requiredOutputs));

    // Pack the input parameters

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters
    if (stdMaskPtrPtr &&
        (!le_wifiAp_UnpackIeeeStdBitMask( &_msgBufPtr,
                                               stdMaskPtrPtr )))
    {
        goto error_unpack;
    }


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_SetChannel
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    uint16_t channelNumber
        ///< [IN] the channel number.
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_SetChannel;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                  channelNumber ));

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_SetCountryCode
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    const char* LE_NONNULL countryCode
        ///< [IN] the country code.
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate
    if ( strnlen(countryCode, 3) > 3 )
    {
        LE_FATAL("strnlen(countryCode, 3) > 3");
    }


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_SetCountryCode;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    LE_ASSERT(le_pack_PackString( &_msgBufPtr,
                                  countryCode, 3 ));

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_SetMaxNumberOfClients
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    int8_t maxNumberOfClient
        ///< [IN] the maximum number of clients
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_SetMaxNumberOfClients;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    LE_ASSERT(le_pack_PackInt8( &_msgBufPtr,
                                                  maxNumberOfClient ));

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
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
__attribute__((weak))
LE_SHARED le_result_t ifgen_le_wifiAp_SetIpRange
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    const char* LE_NONNULL ipAp,
        ///< [IN] the IP address of the Access Point.
    const char* LE_NONNULL ipStart,
        ///< [IN] the start IP address of the Access Point.
    const char* LE_NONNULL ipStop
        ///< [IN] the stop IP address of the Access Point.
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    le_result_t _result =
        LE_OK;

    // Range check values, if appropriate
    if ( strnlen(ipAp, 15) > 15 )
    {
        LE_FATAL("strnlen(ipAp, 15) > 15");
    }
    if ( strnlen(ipStart, 15) > 15 )
    {
        LE_FATAL("strnlen(ipStart, 15) > 15");
    }
    if ( strnlen(ipStop, 15) > 15 )
    {
        LE_FATAL("strnlen(ipStop, 15) > 15");
    }


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_wifiAp_SetIpRange;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    LE_ASSERT(le_pack_PackString( &_msgBufPtr,
                                  ipAp, 15 ));
    LE_ASSERT(le_pack_PackString( &_msgBufPtr,
                                  ipStart, 15 ));
    LE_ASSERT(le_pack_PackString( &_msgBufPtr,
                                  ipStop, 15 ));

    // Send a request to the server and get the response.
    TRACE("Sending message to server and waiting for response : %ti bytes sent",
          _msgBufPtr-_msgPtr->buffer);

    _responseMsgRef = le_msg_RequestSyncResponse(_msgRef);
    // It is a serious error if we don't get a valid response from the server.  Call disconnect
    // handler (if one is defined) to allow cleanup
    if (_responseMsgRef == NULL)
    {
        le_msg_SessionEventHandler_t sessionCloseHandler = NULL;
        void*                        closeContextPtr = NULL;

        le_msg_GetSessionCloseHandler(_ifgen_sessionRef,
                                      &sessionCloseHandler,
                                      &closeContextPtr);
        if (sessionCloseHandler)
        {
            sessionCloseHandler(_ifgen_sessionRef, closeContextPtr);
        }

        LE_FATAL("Error receiving response from server");
    }

    // Process the result and/or output parameters, if there are any.
    _msgPtr = le_msg_GetPayloadPtr(_responseMsgRef);
    _msgBufPtr = _msgPtr->buffer;

    // Unpack the result first
    if (!le_pack_UnpackResult( &_msgBufPtr, &_result ))
    {
        goto error_unpack;
    }

    // Unpack any "out" parameters


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
}


static void ClientIndicationRecvHandler
(
    le_msg_MessageRef_t  msgRef,
    void*                contextPtr
)
{
    LE_UNUSED(contextPtr);

    // Get the message payload
    _Message_t* msgPtr = le_msg_GetPayloadPtr(msgRef);
    uint8_t* _msgBufPtr = msgPtr->buffer;

    // Have to partially unpack the received message in order to know which thread
    // the queued function should actually go to.
    void* clientContextPtr;
    if (!le_pack_UnpackReference( &_msgBufPtr, &clientContextPtr ))
    {
        LE_FATAL("Failed to unpack message from server.");
        return;
    }

    // The clientContextPtr is a safe reference for the client data object.  If the client data
    // pointer is NULL, this means the handler was removed before the event was reported to the
    // client. This is valid, and the event will be dropped.
    _LOCK
    _ClientData_t* clientDataPtr = le_ref_Lookup(_HandlerRefMap, clientContextPtr);
    _UNLOCK
    if ( clientDataPtr == NULL )
    {
        LE_DEBUG("Ignore reported event after handler removed");
        return;
    }

    // Pull out the callers thread
    le_thread_Ref_t callersThreadRef = clientDataPtr->callersThreadRef;

    // Trigger the appropriate event
    switch (msgPtr->id)
    {
        case _MSGID_le_wifiAp_AddNewEventHandler :
            le_event_QueueFunctionToThread(callersThreadRef, _Handle_ifgen_le_wifiAp_AddNewEventHandler, msgRef, clientDataPtr);
            break;

        default:
            LE_FATAL("Unknowm msg id = %" PRIu32 " for client thread = %p",
                msgPtr->id, callersThreadRef);
    }
}
