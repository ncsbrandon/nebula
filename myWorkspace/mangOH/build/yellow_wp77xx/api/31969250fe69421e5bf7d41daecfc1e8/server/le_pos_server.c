
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_pos_server.h"
#include "le_pos_messages.h"
#include "le_pos_service.h"


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
LE_MEM_DEFINE_STATIC_POOL(le_pos_ServerData,
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
LE_REF_DEFINE_STATIC_MAP(le_pos_ServerHandlers,
    LE_MEM_BLOCKS(le_pos_ServerCmd, HIGH_SERVER_DATA_COUNT));


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
le_msg_ServiceRef_t le_pos_GetServiceRef
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
le_msg_SessionRef_t le_pos_GetClientSessionRef
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
void le_pos_AdvertiseService
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
    _ServerDataPool = le_mem_InitStaticPool(le_pos_ServerData,
                                            HIGH_SERVER_DATA_COUNT,
                                            sizeof(_ServerData_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    _HandlerRefMap = le_ref_InitStaticMap(le_pos_ServerHandlers,
                        LE_MEM_BLOCKS(le_pos_ServerData, HIGH_SERVER_DATA_COUNT));

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


static void AsyncResponse_le_pos_AddMovementHandler
(
    le_pos_SampleRef_t positionSampleRef,
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
    _msgPtr->id = _MSGID_le_pos_AddMovementHandler;
    _msgBufPtr = _msgPtr->buffer;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr,
                                                  positionSampleRef ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_le_pos_AddMovementHandler
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
    uint32_t horizontalMagnitude = 0;
    uint32_t verticalMagnitude = 0;
    void *contextPtr = NULL;
    _ServerData_t* serverDataPtr = NULL;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_pack_UnpackUint32( &_msgBufPtr,
                                               &horizontalMagnitude ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackUint32( &_msgBufPtr,
                                               &verticalMagnitude ))
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
    le_pos_MovementHandlerRef_t _result;
    _result  = le_pos_AddMovementHandler ( 
        horizontalMagnitude, 
        verticalMagnitude, AsyncResponse_le_pos_AddMovementHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)le_pos_RemoveMovementHandler;

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


static void Handle_le_pos_RemoveMovementHandler
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
    le_pos_MovementHandlerRef_t handlerRef = NULL;
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
    handlerRef = (le_pos_MovementHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Call the function
    le_pos_RemoveMovementHandler ( 
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


static void Handle_le_pos_Get2DLocation
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

    // Define storage for output parameters
    int32_t latitudeBuffer = 0;
    int32_t *latitudePtr = &latitudeBuffer;
    int32_t longitudeBuffer = 0;
    int32_t *longitudePtr = &longitudeBuffer;
    int32_t hAccuracyBuffer = 0;
    int32_t *hAccuracyPtr = &hAccuracyBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        latitudePtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        longitudePtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 2)))
    {
        hAccuracyPtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = le_pos_Get2DLocation ( 
        latitudePtr, 
        longitudePtr, 
        hAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (latitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *latitudePtr ));
    }
    if (longitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *longitudePtr ));
    }
    if (hAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *hAccuracyPtr ));
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


static void Handle_le_pos_Get3DLocation
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

    // Define storage for output parameters
    int32_t latitudeBuffer = 0;
    int32_t *latitudePtr = &latitudeBuffer;
    int32_t longitudeBuffer = 0;
    int32_t *longitudePtr = &longitudeBuffer;
    int32_t hAccuracyBuffer = 0;
    int32_t *hAccuracyPtr = &hAccuracyBuffer;
    int32_t altitudeBuffer = 0;
    int32_t *altitudePtr = &altitudeBuffer;
    int32_t vAccuracyBuffer = 0;
    int32_t *vAccuracyPtr = &vAccuracyBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        latitudePtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        longitudePtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 2)))
    {
        hAccuracyPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 3)))
    {
        altitudePtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 4)))
    {
        vAccuracyPtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = le_pos_Get3DLocation ( 
        latitudePtr, 
        longitudePtr, 
        hAccuracyPtr, 
        altitudePtr, 
        vAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (latitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *latitudePtr ));
    }
    if (longitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *longitudePtr ));
    }
    if (hAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *hAccuracyPtr ));
    }
    if (altitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *altitudePtr ));
    }
    if (vAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *vAccuracyPtr ));
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


static void Handle_le_pos_GetTime
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

    // Define storage for output parameters
    uint16_t hoursBuffer = 0;
    uint16_t *hoursPtr = &hoursBuffer;
    uint16_t minutesBuffer = 0;
    uint16_t *minutesPtr = &minutesBuffer;
    uint16_t secondsBuffer = 0;
    uint16_t *secondsPtr = &secondsBuffer;
    uint16_t millisecondsBuffer = 0;
    uint16_t *millisecondsPtr = &millisecondsBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        hoursPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        minutesPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 2)))
    {
        secondsPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 3)))
    {
        millisecondsPtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = le_pos_GetTime ( 
        hoursPtr, 
        minutesPtr, 
        secondsPtr, 
        millisecondsPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (hoursPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *hoursPtr ));
    }
    if (minutesPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *minutesPtr ));
    }
    if (secondsPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *secondsPtr ));
    }
    if (millisecondsPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *millisecondsPtr ));
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


static void Handle_le_pos_GetDate
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

    // Define storage for output parameters
    uint16_t yearBuffer = 0;
    uint16_t *yearPtr = &yearBuffer;
    uint16_t monthBuffer = 0;
    uint16_t *monthPtr = &monthBuffer;
    uint16_t dayBuffer = 0;
    uint16_t *dayPtr = &dayBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        yearPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        monthPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 2)))
    {
        dayPtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = le_pos_GetDate ( 
        yearPtr, 
        monthPtr, 
        dayPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (yearPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *yearPtr ));
    }
    if (monthPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *monthPtr ));
    }
    if (dayPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *dayPtr ));
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


static void Handle_le_pos_GetMotion
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

    // Define storage for output parameters
    uint32_t hSpeedBuffer = 0;
    uint32_t *hSpeedPtr = &hSpeedBuffer;
    uint32_t hSpeedAccuracyBuffer = 0;
    uint32_t *hSpeedAccuracyPtr = &hSpeedAccuracyBuffer;
    int32_t vSpeedBuffer = 0;
    int32_t *vSpeedPtr = &vSpeedBuffer;
    int32_t vSpeedAccuracyBuffer = 0;
    int32_t *vSpeedAccuracyPtr = &vSpeedAccuracyBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        hSpeedPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        hSpeedAccuracyPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 2)))
    {
        vSpeedPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 3)))
    {
        vSpeedAccuracyPtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = le_pos_GetMotion ( 
        hSpeedPtr, 
        hSpeedAccuracyPtr, 
        vSpeedPtr, 
        vSpeedAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (hSpeedPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *hSpeedPtr ));
    }
    if (hSpeedAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *hSpeedAccuracyPtr ));
    }
    if (vSpeedPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *vSpeedPtr ));
    }
    if (vSpeedAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *vSpeedAccuracyPtr ));
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


static void Handle_le_pos_GetHeading
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

    // Define storage for output parameters
    uint32_t headingBuffer = 0;
    uint32_t *headingPtr = &headingBuffer;
    uint32_t headingAccuracyBuffer = 0;
    uint32_t *headingAccuracyPtr = &headingAccuracyBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        headingPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        headingAccuracyPtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = le_pos_GetHeading ( 
        headingPtr, 
        headingAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (headingPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *headingPtr ));
    }
    if (headingAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *headingAccuracyPtr ));
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


static void Handle_le_pos_GetDirection
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

    // Define storage for output parameters
    uint32_t directionBuffer = 0;
    uint32_t *directionPtr = &directionBuffer;
    uint32_t directionAccuracyBuffer = 0;
    uint32_t *directionAccuracyPtr = &directionAccuracyBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        directionPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        directionAccuracyPtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = le_pos_GetDirection ( 
        directionPtr, 
        directionAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (directionPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *directionPtr ));
    }
    if (directionAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *directionAccuracyPtr ));
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


static void Handle_le_pos_GetFixState
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

    // Define storage for output parameters
    le_pos_FixState_t stateBuffer = (le_pos_FixState_t) 0;
    le_pos_FixState_t *statePtr = &stateBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        statePtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = le_pos_GetFixState ( 
        statePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (statePtr)
    {
        LE_ASSERT(le_pos_PackFixState( &_msgBufPtr,
                                                      *statePtr ));
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


static void Handle_le_pos_sample_Get2DLocation
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
    le_pos_SampleRef_t positionSampleRef = NULL;

    // Define storage for output parameters
    int32_t latitudeBuffer = 0;
    int32_t *latitudePtr = &latitudeBuffer;
    int32_t longitudeBuffer = 0;
    int32_t *longitudePtr = &longitudeBuffer;
    int32_t horizontalAccuracyBuffer = 0;
    int32_t *horizontalAccuracyPtr = &horizontalAccuracyBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        latitudePtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        longitudePtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 2)))
    {
        horizontalAccuracyPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_Get2DLocation ( 
        positionSampleRef, 
        latitudePtr, 
        longitudePtr, 
        horizontalAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (latitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *latitudePtr ));
    }
    if (longitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *longitudePtr ));
    }
    if (horizontalAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *horizontalAccuracyPtr ));
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


static void Handle_le_pos_sample_GetTime
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
    le_pos_SampleRef_t positionSampleRef = NULL;

    // Define storage for output parameters
    uint16_t hoursBuffer = 0;
    uint16_t *hoursPtr = &hoursBuffer;
    uint16_t minutesBuffer = 0;
    uint16_t *minutesPtr = &minutesBuffer;
    uint16_t secondsBuffer = 0;
    uint16_t *secondsPtr = &secondsBuffer;
    uint16_t millisecondsBuffer = 0;
    uint16_t *millisecondsPtr = &millisecondsBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        hoursPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        minutesPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 2)))
    {
        secondsPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 3)))
    {
        millisecondsPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetTime ( 
        positionSampleRef, 
        hoursPtr, 
        minutesPtr, 
        secondsPtr, 
        millisecondsPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (hoursPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *hoursPtr ));
    }
    if (minutesPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *minutesPtr ));
    }
    if (secondsPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *secondsPtr ));
    }
    if (millisecondsPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *millisecondsPtr ));
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


static void Handle_le_pos_sample_GetDate
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
    le_pos_SampleRef_t positionSampleRef = NULL;

    // Define storage for output parameters
    uint16_t yearBuffer = 0;
    uint16_t *yearPtr = &yearBuffer;
    uint16_t monthBuffer = 0;
    uint16_t *monthPtr = &monthBuffer;
    uint16_t dayBuffer = 0;
    uint16_t *dayPtr = &dayBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        yearPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        monthPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 2)))
    {
        dayPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetDate ( 
        positionSampleRef, 
        yearPtr, 
        monthPtr, 
        dayPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (yearPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *yearPtr ));
    }
    if (monthPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *monthPtr ));
    }
    if (dayPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *dayPtr ));
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


static void Handle_le_pos_sample_GetAltitude
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
    le_pos_SampleRef_t positionSampleRef = NULL;

    // Define storage for output parameters
    int32_t altitudeBuffer = 0;
    int32_t *altitudePtr = &altitudeBuffer;
    int32_t altitudeAccuracyBuffer = 0;
    int32_t *altitudeAccuracyPtr = &altitudeAccuracyBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        altitudePtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        altitudeAccuracyPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetAltitude ( 
        positionSampleRef, 
        altitudePtr, 
        altitudeAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (altitudePtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *altitudePtr ));
    }
    if (altitudeAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *altitudeAccuracyPtr ));
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


static void Handle_le_pos_sample_GetHorizontalSpeed
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
    le_pos_SampleRef_t positionSampleRef = NULL;

    // Define storage for output parameters
    uint32_t hSpeedBuffer = 0;
    uint32_t *hSpeedPtr = &hSpeedBuffer;
    uint32_t hSpeedAccuracyBuffer = 0;
    uint32_t *hSpeedAccuracyPtr = &hSpeedAccuracyBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        hSpeedPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        hSpeedAccuracyPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetHorizontalSpeed ( 
        positionSampleRef, 
        hSpeedPtr, 
        hSpeedAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (hSpeedPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *hSpeedPtr ));
    }
    if (hSpeedAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *hSpeedAccuracyPtr ));
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


static void Handle_le_pos_sample_GetVerticalSpeed
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
    le_pos_SampleRef_t positionSampleRef = NULL;

    // Define storage for output parameters
    int32_t vspeedBuffer = 0;
    int32_t *vspeedPtr = &vspeedBuffer;
    int32_t vspeedAccuracyBuffer = 0;
    int32_t *vspeedAccuracyPtr = &vspeedAccuracyBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        vspeedPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        vspeedAccuracyPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetVerticalSpeed ( 
        positionSampleRef, 
        vspeedPtr, 
        vspeedAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (vspeedPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *vspeedPtr ));
    }
    if (vspeedAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackInt32( &_msgBufPtr,
                                                      *vspeedAccuracyPtr ));
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


static void Handle_le_pos_sample_GetHeading
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
    le_pos_SampleRef_t positionSampleRef = NULL;

    // Define storage for output parameters
    uint32_t headingBuffer = 0;
    uint32_t *headingPtr = &headingBuffer;
    uint32_t headingAccuracyBuffer = 0;
    uint32_t *headingAccuracyPtr = &headingAccuracyBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        headingPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        headingAccuracyPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetHeading ( 
        positionSampleRef, 
        headingPtr, 
        headingAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (headingPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *headingPtr ));
    }
    if (headingAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *headingAccuracyPtr ));
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


static void Handle_le_pos_sample_GetDirection
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
    le_pos_SampleRef_t positionSampleRef = NULL;

    // Define storage for output parameters
    uint32_t directionBuffer = 0;
    uint32_t *directionPtr = &directionBuffer;
    uint32_t directionAccuracyBuffer = 0;
    uint32_t *directionAccuracyPtr = &directionAccuracyBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        directionPtr = NULL;
    }
    if (!(_requiredOutputs & (1u << 1)))
    {
        directionAccuracyPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetDirection ( 
        positionSampleRef, 
        directionPtr, 
        directionAccuracyPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (directionPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *directionPtr ));
    }
    if (directionAccuracyPtr)
    {
        LE_ASSERT(le_pack_PackUint32( &_msgBufPtr,
                                                      *directionAccuracyPtr ));
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


static void Handle_le_pos_sample_GetFixState
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
    le_pos_SampleRef_t positionSampleRef = NULL;

    // Define storage for output parameters
    le_pos_FixState_t stateBuffer = (le_pos_FixState_t) 0;
    le_pos_FixState_t *statePtr = &stateBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        statePtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_sample_GetFixState ( 
        positionSampleRef, 
        statePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (statePtr)
    {
        LE_ASSERT(le_pos_PackFixState( &_msgBufPtr,
                                                      *statePtr ));
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


static void Handle_le_pos_sample_Release
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
    le_pos_SampleRef_t positionSampleRef = NULL;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &positionSampleRef ))
    {
        goto error_unpack;
    }

    // Call the function
    le_pos_sample_Release ( 
        positionSampleRef );

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


static void Handle_le_pos_SetAcquisitionRate
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
    uint32_t acquisitionRate = 0;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_pack_UnpackUint32( &_msgBufPtr,
                                               &acquisitionRate ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_SetAcquisitionRate ( 
        acquisitionRate );

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


static void Handle_le_pos_GetAcquisitionRate
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

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message

    // Call the function
    uint32_t _result;
    _result  = le_pos_GetAcquisitionRate (  );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackUint32( &_msgBufPtr, _result ));

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
}


static void Handle_le_pos_SetDistanceResolution
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
    le_pos_Resolution_t resolution = (le_pos_Resolution_t) 0;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_pos_UnpackResolution( &_msgBufPtr,
                                               &resolution ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_pos_SetDistanceResolution ( 
        resolution );

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
        case _MSGID_le_pos_AddMovementHandler :
            Handle_le_pos_AddMovementHandler(msgRef);
            break;
        case _MSGID_le_pos_RemoveMovementHandler :
            Handle_le_pos_RemoveMovementHandler(msgRef);
            break;
        case _MSGID_le_pos_Get2DLocation :
            Handle_le_pos_Get2DLocation(msgRef);
            break;
        case _MSGID_le_pos_Get3DLocation :
            Handle_le_pos_Get3DLocation(msgRef);
            break;
        case _MSGID_le_pos_GetTime :
            Handle_le_pos_GetTime(msgRef);
            break;
        case _MSGID_le_pos_GetDate :
            Handle_le_pos_GetDate(msgRef);
            break;
        case _MSGID_le_pos_GetMotion :
            Handle_le_pos_GetMotion(msgRef);
            break;
        case _MSGID_le_pos_GetHeading :
            Handle_le_pos_GetHeading(msgRef);
            break;
        case _MSGID_le_pos_GetDirection :
            Handle_le_pos_GetDirection(msgRef);
            break;
        case _MSGID_le_pos_GetFixState :
            Handle_le_pos_GetFixState(msgRef);
            break;
        case _MSGID_le_pos_sample_Get2DLocation :
            Handle_le_pos_sample_Get2DLocation(msgRef);
            break;
        case _MSGID_le_pos_sample_GetTime :
            Handle_le_pos_sample_GetTime(msgRef);
            break;
        case _MSGID_le_pos_sample_GetDate :
            Handle_le_pos_sample_GetDate(msgRef);
            break;
        case _MSGID_le_pos_sample_GetAltitude :
            Handle_le_pos_sample_GetAltitude(msgRef);
            break;
        case _MSGID_le_pos_sample_GetHorizontalSpeed :
            Handle_le_pos_sample_GetHorizontalSpeed(msgRef);
            break;
        case _MSGID_le_pos_sample_GetVerticalSpeed :
            Handle_le_pos_sample_GetVerticalSpeed(msgRef);
            break;
        case _MSGID_le_pos_sample_GetHeading :
            Handle_le_pos_sample_GetHeading(msgRef);
            break;
        case _MSGID_le_pos_sample_GetDirection :
            Handle_le_pos_sample_GetDirection(msgRef);
            break;
        case _MSGID_le_pos_sample_GetFixState :
            Handle_le_pos_sample_GetFixState(msgRef);
            break;
        case _MSGID_le_pos_sample_Release :
            Handle_le_pos_sample_Release(msgRef);
            break;
        case _MSGID_le_pos_SetAcquisitionRate :
            Handle_le_pos_SetAcquisitionRate(msgRef);
            break;
        case _MSGID_le_pos_GetAcquisitionRate :
            Handle_le_pos_GetAcquisitionRate(msgRef);
            break;
        case _MSGID_le_pos_SetDistanceResolution :
            Handle_le_pos_SetDistanceResolution(msgRef);
            break;

        default: LE_ERROR("Unknowm msg id = %" PRIu32 , msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    LE_CDATA_THIS->_ClientSessionRef = 0;
}
