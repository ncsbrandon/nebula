
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_antenna_server.h"
#include "le_antenna_messages.h"
#include "le_antenna_service.h"


//--------------------------------------------------------------------------------------------------
// Generic Server Types, Variables and Functions
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Type definition for generic function to remove a handler, given the handler ref.
 */
//--------------------------------------------------------------------------------------------------
typedef void(* RemoveHandlerFunc_t)(void *handlerRef);


//--------------------------------------------------------------------------------------------------
/**
 * Server Data Objects
 *
 * This object is used to store additional context info for each request
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    le_msg_SessionRef_t   clientSessionRef;     ///< The client to send the response to
    void*                 contextPtr;           ///< ContextPtr registered with handler
    le_event_HandlerRef_t handlerRef;           ///< HandlerRef for the registered handler
    RemoveHandlerFunc_t   removeHandlerFunc;    ///< Function to remove the registered handler
}
_ServerData_t;

//--------------------------------------------------------------------------------------------------
/**
 * Expected number of simultaneous server data objects.
 */
//--------------------------------------------------------------------------------------------------
#define HIGH_SERVER_DATA_COUNT  3

//--------------------------------------------------------------------------------------------------
/**
 * Static pool for server data objects
 */
//--------------------------------------------------------------------------------------------------
LE_MEM_DEFINE_STATIC_POOL(le_antenna_ServerData,
                          HIGH_SERVER_DATA_COUNT,
                          sizeof(_ServerData_t));

//--------------------------------------------------------------------------------------------------
/**
 * The memory pool for server data objects
 */
//--------------------------------------------------------------------------------------------------
static le_mem_PoolRef_t _ServerDataPool;

//--------------------------------------------------------------------------------------------------
/**
 *  Static safe reference map for use with Add/Remove handler references
 */
//--------------------------------------------------------------------------------------------------
LE_REF_DEFINE_STATIC_MAP(le_antenna_ServerHandlers,
    LE_MEM_BLOCKS(le_antenna_ServerCmd, HIGH_SERVER_DATA_COUNT));


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
 * Forward declaration needed by StartServer
 */
//--------------------------------------------------------------------------------------------------
static void ServerMsgRecvHandler
(
    le_msg_MessageRef_t msgRef,
    void*               contextPtr
);


//--------------------------------------------------------------------------------------------------
/**
 * Per-server data:
 *  - Server service reference
 *  - Server thread reference
 *  - Client session reference
 */
//--------------------------------------------------------------------------------------------------
LE_CDATA_DECLARE({le_msg_ServiceRef_t _ServerServiceRef;
        le_thread_Ref_t _ServerThreadRef;
        le_msg_SessionRef_t _ClientSessionRef;});

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
 * Cleanup client data if the client is no longer connected
 */
//--------------------------------------------------------------------------------------------------
__attribute__((unused)) static void CleanupClientData
(
    le_msg_SessionRef_t sessionRef,
    void *contextPtr
)
{
    LE_UNUSED(contextPtr);

    LE_DEBUG("Client %p is closed !!!", sessionRef);

    // Iterate over the server data reference map and remove anything that matches
    // the client session.
    _LOCK

    // Store the client session ref so it can be retrieved by the server using the
    // GetClientSessionRef() function, if it's needed inside handler removal functions.
    LE_CDATA_THIS->_ClientSessionRef = sessionRef;


    le_ref_IterRef_t iterRef = le_ref_GetIterator(_HandlerRefMap);
    le_result_t result = le_ref_NextNode(iterRef);
    _ServerData_t const* serverDataPtr;

    while ( result == LE_OK )
    {
        serverDataPtr =  le_ref_GetValue(iterRef);

        if ( sessionRef != serverDataPtr->clientSessionRef )
        {
            LE_DEBUG("Found session ref %p; does not match",
                     serverDataPtr->clientSessionRef);
        }
        else
        {
            LE_DEBUG("Found session ref %p; match found, so needs cleanup",
                     serverDataPtr->clientSessionRef);

            // Remove the handler, if the Remove handler functions exists.
            if ( serverDataPtr->removeHandlerFunc != NULL )
            {
                serverDataPtr->removeHandlerFunc( serverDataPtr->handlerRef );
            }

            // Release the server data block
            le_mem_Release((void*)serverDataPtr);

            // Delete the associated safeRef
            le_ref_DeleteRef( _HandlerRefMap, (void*)le_ref_GetSafeRef(iterRef) );
        }

        // Get the next value in the reference mpa
        result = le_ref_NextNode(iterRef);
    }

    // Clear the client session ref, since the event has now been processed.
    LE_CDATA_THIS->_ClientSessionRef = 0;

    _UNLOCK
}


//--------------------------------------------------------------------------------------------------
/**
 * Send the message to the client (queued version)
 *
 * This is a wrapper around le_msg_Send() with an extra parameter so that it can be used
 * with le_event_QueueFunctionToThread().
 */
//--------------------------------------------------------------------------------------------------
__attribute__((unused)) static void SendMsgToClientQueued
(
    void*  msgRef,  ///< [in] Reference to the message.
    void*  unused   ///< [in] Not used
)
{
    LE_UNUSED(unused);
    le_msg_Send(msgRef);
}


//--------------------------------------------------------------------------------------------------
/**
 * Send the message to the client.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((unused)) static void SendMsgToClient
(
    le_msg_MessageRef_t msgRef      ///< [in] Reference to the message.
)
{
    /*
     * If called from a thread other than the server thread, queue the message onto the server
     * thread.  This is necessary to allow async response/handler functions to be called from any
     * thread, whereas messages to the client can only be sent from the server thread.
     */
    if ( le_thread_GetCurrent() != LE_CDATA_THIS->_ServerThreadRef )
    {
        le_event_QueueFunctionToThread(LE_CDATA_THIS->_ServerThreadRef,
                                       SendMsgToClientQueued,
                                       msgRef,
                                       NULL);
    }
    else
    {
        le_msg_Send(msgRef);
    }
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_antenna_GetServiceRef
(
    void
)
{
    return LE_CDATA_THIS->_ServerServiceRef;
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_antenna_GetClientSessionRef
(
    void
)
{
    return LE_CDATA_THIS->_ClientSessionRef;
}


//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_antenna_AdvertiseService
(
    void
)
{
    LE_DEBUG("======= Starting Server %s ========", SERVICE_INSTANCE_NAME);

    // Get a reference to the trace keyword that is used to control tracing in this module.
#if defined(MK_TOOLS_BUILD) && !defined(NO_LOG_SESSION)
    TraceRef = le_log_GetTraceRef("ipc");
#endif

    // Create the server data pool
    _ServerDataPool = le_mem_InitStaticPool(le_antenna_ServerData,
                                            HIGH_SERVER_DATA_COUNT,
                                            sizeof(_ServerData_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    _HandlerRefMap = le_ref_InitStaticMap(le_antenna_ServerHandlers,
                        LE_MEM_BLOCKS(le_antenna_ServerData, HIGH_SERVER_DATA_COUNT));

    // Start the server side of the service
    le_msg_ProtocolRef_t protocolRef;

    protocolRef = le_msg_GetProtocolRef(PROTOCOL_ID_STR, sizeof(_Message_t));
    LE_CDATA_THIS->_ServerServiceRef = le_msg_CreateService(protocolRef, SERVICE_INSTANCE_NAME);
    le_msg_SetServiceRecvHandler(LE_CDATA_THIS->_ServerServiceRef, ServerMsgRecvHandler, NULL);
    le_msg_AdvertiseService(LE_CDATA_THIS->_ServerServiceRef);

    // Register for client sessions being closed
    le_msg_AddServiceCloseHandler(LE_CDATA_THIS->_ServerServiceRef, CleanupClientData, NULL);

    // Need to keep track of the thread that is registered to provide this service.
    LE_CDATA_THIS->_ServerThreadRef = le_thread_GetCurrent();
}


//--------------------------------------------------------------------------------------------------
// Client Specific Server Code
//--------------------------------------------------------------------------------------------------


static void Handle_le_antenna_Request
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;
    // Declare temporaries for input parameters
    le_antenna_Type_t antennaType = (le_antenna_Type_t) 0;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_antenna_UnpackType( &_msgBufPtr,
                                               &antennaType ))
    {
        goto error_unpack;
    }

    // Call the function
    le_antenna_ObjRef_t _result;
    _result  = le_antenna_Request ( 
        antennaType );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, _result ));

    // Pack any "out" parameters

#ifdef LE_CONFIG_RPC
    // Check if we are at the end of the response buffer
    if ((_msgBufPtr - _msgBufStartPtr) <
        (int)(le_msg_GetMaxPayloadSize(_msgRef) - sizeof(uint32_t)))
    {
        // Add EOF TagID to the end of response message
        *_msgBufPtr = LE_PACK_EOF;
    }
#endif

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_antenna_GetType
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;
    // Declare temporaries for input parameters
    le_antenna_ObjRef_t antennaRef = NULL;

    // Define storage for output parameters
    le_antenna_Type_t antennaTypeBuffer = (le_antenna_Type_t) 0;
    le_antenna_Type_t *antennaTypePtr = &antennaTypeBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        antennaTypePtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &antennaRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_antenna_GetType ( 
        antennaRef, 
        antennaTypePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (antennaTypePtr)
    {
        LE_ASSERT(le_antenna_PackType( &_msgBufPtr,
                                                      *antennaTypePtr ));
    }

#ifdef LE_CONFIG_RPC
    // Check if we are at the end of the response buffer
    if ((_msgBufPtr - _msgBufStartPtr) <
        (int)(le_msg_GetMaxPayloadSize(_msgRef) - sizeof(uint32_t)))
    {
        // Add EOF TagID to the end of response message
        *_msgBufPtr = LE_PACK_EOF;
    }
#endif

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_antenna_SetShortLimit
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;
    // Declare temporaries for input parameters
    le_antenna_ObjRef_t antennaRef = NULL;
    uint32_t shortLimit = 0;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &antennaRef ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackUint32( &_msgBufPtr,
                                               &shortLimit ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_antenna_SetShortLimit ( 
        antennaRef, 
        shortLimit );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters

#ifdef LE_CONFIG_RPC
    // Check if we are at the end of the response buffer
    if ((_msgBufPtr - _msgBufStartPtr) <
        (int)(le_msg_GetMaxPayloadSize(_msgRef) - sizeof(uint32_t)))
    {
        // Add EOF TagID to the end of response message
        *_msgBufPtr = LE_PACK_EOF;
    }
#endif

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_antenna_GetShortLimit
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;
    // Declare temporaries for input parameters
    le_antenna_ObjRef_t antennaRef = NULL;

    // Define storage for output parameters
    uint32_t shortLimitBuffer = 0;
    uint32_t *shortLimitPtr = &shortLimitBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        shortLimitPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &antennaRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_antenna_GetShortLimit ( 
        antennaRef, 
        shortLimitPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (shortLimitPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *shortLimitPtr ));
    }

#ifdef LE_CONFIG_RPC
    // Check if we are at the end of the response buffer
    if ((_msgBufPtr - _msgBufStartPtr) <
        (int)(le_msg_GetMaxPayloadSize(_msgRef) - sizeof(uint32_t)))
    {
        // Add EOF TagID to the end of response message
        *_msgBufPtr = LE_PACK_EOF;
    }
#endif

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_antenna_SetOpenLimit
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;
    // Declare temporaries for input parameters
    le_antenna_ObjRef_t antennaRef = NULL;
    uint32_t openLimit = 0;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &antennaRef ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackUint32( &_msgBufPtr,
                                               &openLimit ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_antenna_SetOpenLimit ( 
        antennaRef, 
        openLimit );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters

#ifdef LE_CONFIG_RPC
    // Check if we are at the end of the response buffer
    if ((_msgBufPtr - _msgBufStartPtr) <
        (int)(le_msg_GetMaxPayloadSize(_msgRef) - sizeof(uint32_t)))
    {
        // Add EOF TagID to the end of response message
        *_msgBufPtr = LE_PACK_EOF;
    }
#endif

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_antenna_GetOpenLimit
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;
    // Declare temporaries for input parameters
    le_antenna_ObjRef_t antennaRef = NULL;

    // Define storage for output parameters
    uint32_t openLimitBuffer = 0;
    uint32_t *openLimitPtr = &openLimitBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        openLimitPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &antennaRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_antenna_GetOpenLimit ( 
        antennaRef, 
        openLimitPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (openLimitPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *openLimitPtr ));
    }

#ifdef LE_CONFIG_RPC
    // Check if we are at the end of the response buffer
    if ((_msgBufPtr - _msgBufStartPtr) <
        (int)(le_msg_GetMaxPayloadSize(_msgRef) - sizeof(uint32_t)))
    {
        // Add EOF TagID to the end of response message
        *_msgBufPtr = LE_PACK_EOF;
    }
#endif

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void AsyncResponse_le_antenna_AddStatusEventHandler
(
    le_antenna_ObjRef_t antennaRef,
    le_antenna_Status_t status,
    void* contextPtr
)
{
    le_msg_MessageRef_t _msgRef;
    _Message_t* _msgPtr;
    _ServerData_t* serverDataPtr = (_ServerData_t*)contextPtr;

    // Will not be used if no data is sent back to client
    __attribute__((unused)) uint8_t* _msgBufPtr;

    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(serverDataPtr->clientSessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_le_antenna_AddStatusEventHandler;
    _msgBufPtr = _msgPtr->buffer;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr,
                                                  antennaRef ));
    LE_ASSERT(le_antenna_PackStatus( &_msgBufPtr,
                                                  status ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_antenna_AddStatusEventHandler
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;
    // Declare temporaries for input parameters
    le_antenna_ObjRef_t antennaRef = NULL;
    void *contextPtr = NULL;
    _ServerData_t* serverDataPtr = NULL;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &antennaRef ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackReference( &_msgBufPtr, &contextPtr ))
    {
        goto error_unpack;
    }

    // Create a new server data object and fill it in
    serverDataPtr = le_mem_Alloc(_ServerDataPool);
    serverDataPtr->clientSessionRef = le_msg_GetSession(_msgRef);
    serverDataPtr->contextPtr = contextPtr;
    serverDataPtr->handlerRef = NULL;
    serverDataPtr->removeHandlerFunc = NULL;
    contextPtr = serverDataPtr;

    // Call the function
    le_antenna_StatusEventHandlerRef_t _result;
    _result  = le_antenna_AddStatusEventHandler ( 
        antennaRef, AsyncResponse_le_antenna_AddStatusEventHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_antenna_RemoveStatusEventHandler;

        // Return a safe reference to the server data object as the reference.
        _LOCK
        _result = le_ref_CreateRef(_HandlerRefMap, serverDataPtr);
        _UNLOCK
    }
    else
    {
        // Adding handler failed; release serverDataPtr and return NULL back to the client.
        le_mem_Release(serverDataPtr);
    }

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, _result ));

    // Pack any "out" parameters

#ifdef LE_CONFIG_RPC
    // Check if we are at the end of the response buffer
    if ((_msgBufPtr - _msgBufStartPtr) <
        (int)(le_msg_GetMaxPayloadSize(_msgRef) - sizeof(uint32_t)))
    {
        // Add EOF TagID to the end of response message
        *_msgBufPtr = LE_PACK_EOF;
    }
#endif

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_antenna_RemoveStatusEventHandler
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;
    _ServerData_t* serverDataPtr;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    le_antenna_StatusEventHandlerRef_t handlerRef = NULL;
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                  &handlerRef ))
    {
        goto error_unpack;
    }
    // The passed in handlerRef is a safe reference for the server data object.  Need to get the
    // real handlerRef from the server data object and then delete both the safe reference and
    // the object since they are no longer needed.
    _LOCK
    serverDataPtr = le_ref_Lookup(_HandlerRefMap,
                                  handlerRef);
    if ( serverDataPtr == NULL )
    {
        _UNLOCK
        LE_KILL_CLIENT("Invalid reference");
        return;
    }
    le_ref_DeleteRef(_HandlerRefMap, handlerRef);
    _UNLOCK
    handlerRef = (le_antenna_StatusEventHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Call the function
    le_antenna_RemoveStatusEventHandler ( 
        handlerRef );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack any "out" parameters

#ifdef LE_CONFIG_RPC
    // Check if we are at the end of the response buffer
    if ((_msgBufPtr - _msgBufStartPtr) <
        (int)(le_msg_GetMaxPayloadSize(_msgRef) - sizeof(uint32_t)))
    {
        // Add EOF TagID to the end of response message
        *_msgBufPtr = LE_PACK_EOF;
    }
#endif

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_antenna_GetStatus
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;
    // Declare temporaries for input parameters
    le_antenna_ObjRef_t antennaRef = NULL;

    // Define storage for output parameters
    le_antenna_Status_t statusBuffer = (le_antenna_Status_t) 0;
    le_antenna_Status_t *statusPtr = &statusBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        statusPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &antennaRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_antenna_GetStatus ( 
        antennaRef, 
        statusPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (statusPtr)
    {
        LE_ASSERT(le_antenna_PackStatus( &_msgBufPtr,
                                                      *statusPtr ));
    }

#ifdef LE_CONFIG_RPC
    // Check if we are at the end of the response buffer
    if ((_msgBufPtr - _msgBufStartPtr) <
        (int)(le_msg_GetMaxPayloadSize(_msgRef) - sizeof(uint32_t)))
    {
        // Add EOF TagID to the end of response message
        *_msgBufPtr = LE_PACK_EOF;
    }
#endif

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_antenna_SetExternalAdc
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;
    // Declare temporaries for input parameters
    le_antenna_ObjRef_t antennaRef = NULL;
    int8_t adcId = 0;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &antennaRef ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackInt8( &_msgBufPtr,
                                               &adcId ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_antenna_SetExternalAdc ( 
        antennaRef, 
        adcId );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters

#ifdef LE_CONFIG_RPC
    // Check if we are at the end of the response buffer
    if ((_msgBufPtr - _msgBufStartPtr) <
        (int)(le_msg_GetMaxPayloadSize(_msgRef) - sizeof(uint32_t)))
    {
        // Add EOF TagID to the end of response message
        *_msgBufPtr = LE_PACK_EOF;
    }
#endif

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void Handle_le_antenna_GetExternalAdc
(
    le_msg_MessageRef_t _msgRef

)
{
    // Get the message buffer pointer
    __attribute__((unused)) uint8_t* _msgBufPtr =
        ((_Message_t*)le_msg_GetPayloadPtr(_msgRef))->buffer;

    // Needed if we are returning a result or output values
    uint8_t* _msgBufStartPtr = _msgBufPtr;
    // Declare temporaries for input parameters
    le_antenna_ObjRef_t antennaRef = NULL;

    // Define storage for output parameters
    int8_t adcIdBuffer = 0;
    int8_t *adcIdPtr = &adcIdBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        adcIdPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &antennaRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_antenna_GetExternalAdc ( 
        antennaRef, 
        adcIdPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (adcIdPtr)
    {
        LE_ASSERT(le_pack_PackInt8( &_msgBufPtr,
                                                      *adcIdPtr ));
    }

#ifdef LE_CONFIG_RPC
    // Check if we are at the end of the response buffer
    if ((_msgBufPtr - _msgBufStartPtr) <
        (int)(le_msg_GetMaxPayloadSize(_msgRef) - sizeof(uint32_t)))
    {
        // Add EOF TagID to the end of response message
        *_msgBufPtr = LE_PACK_EOF;
    }
#endif

    // Return the response
    TRACE("Sending response to client session %p : %ti bytes sent",
          le_msg_GetSession(_msgRef),
          _msgBufPtr-_msgBufStartPtr);


    le_msg_Respond(_msgRef);

    return;

error_unpack:
    LE_KILL_CLIENT("Error unpacking message");
}


static void ServerMsgRecvHandler
(
    le_msg_MessageRef_t msgRef,
    void*               contextPtr
)
{
    LE_UNUSED(contextPtr);

    // Get the message payload so that we can get the message "id"
    _Message_t* msgPtr = le_msg_GetPayloadPtr(msgRef);

    // Get the client session ref for the current message.  This ref is used by the server to
    // get info about the client process, such as user id.  If there are multiple clients, then
    // the session ref may be different for each message, hence it has to be queried each time.
    LE_CDATA_THIS->_ClientSessionRef = le_msg_GetSession(msgRef);

    // Dispatch to appropriate message handler and get response
    switch (msgPtr->id)
    {
        case _MSGID_le_antenna_Request :
            Handle_le_antenna_Request(msgRef);
            break;
        case _MSGID_le_antenna_GetType :
            Handle_le_antenna_GetType(msgRef);
            break;
        case _MSGID_le_antenna_SetShortLimit :
            Handle_le_antenna_SetShortLimit(msgRef);
            break;
        case _MSGID_le_antenna_GetShortLimit :
            Handle_le_antenna_GetShortLimit(msgRef);
            break;
        case _MSGID_le_antenna_SetOpenLimit :
            Handle_le_antenna_SetOpenLimit(msgRef);
            break;
        case _MSGID_le_antenna_GetOpenLimit :
            Handle_le_antenna_GetOpenLimit(msgRef);
            break;
        case _MSGID_le_antenna_AddStatusEventHandler :
            Handle_le_antenna_AddStatusEventHandler(msgRef);
            break;
        case _MSGID_le_antenna_RemoveStatusEventHandler :
            Handle_le_antenna_RemoveStatusEventHandler(msgRef);
            break;
        case _MSGID_le_antenna_GetStatus :
            Handle_le_antenna_GetStatus(msgRef);
            break;
        case _MSGID_le_antenna_SetExternalAdc :
            Handle_le_antenna_SetExternalAdc(msgRef);
            break;
        case _MSGID_le_antenna_GetExternalAdc :
            Handle_le_antenna_GetExternalAdc(msgRef);
            break;

        default: LE_ERROR("Unknowm msg id = %" PRIu32 , msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    LE_CDATA_THIS->_ClientSessionRef = 0;
}
