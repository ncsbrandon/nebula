
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "ma_battery_server.h"
#include "ma_battery_messages.h"
#include "ma_battery_service.h"


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
LE_MEM_DEFINE_STATIC_POOL(ma_battery_ServerData,
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
LE_REF_DEFINE_STATIC_MAP(ma_battery_ServerHandlers,
    LE_MEM_BLOCKS(ma_battery_ServerCmd, HIGH_SERVER_DATA_COUNT));


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
le_msg_ServiceRef_t ma_battery_GetServiceRef
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
le_msg_SessionRef_t ma_battery_GetClientSessionRef
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
void ma_battery_AdvertiseService
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
    _ServerDataPool = le_mem_InitStaticPool(ma_battery_ServerData,
                                            HIGH_SERVER_DATA_COUNT,
                                            sizeof(_ServerData_t));

    // Create safe reference map for handler references.
    // The size of the map should be based on the number of handlers defined for the server.
    _HandlerRefMap = le_ref_InitStaticMap(ma_battery_ServerHandlers,
                        LE_MEM_BLOCKS(ma_battery_ServerData, HIGH_SERVER_DATA_COUNT));

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


static void Handle_ma_battery_GetChargingStatus
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
    ma_battery_ChargingStatus_t _result;
    _result  = ma_battery_GetChargingStatus (  );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(ma_battery_PackChargingStatus( &_msgBufPtr, _result ));

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


static void Handle_ma_battery_GetHealthStatus
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
    ma_battery_HealthStatus_t _result;
    _result  = ma_battery_GetHealthStatus (  );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(ma_battery_PackHealthStatus( &_msgBufPtr, _result ));

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


static void Handle_ma_battery_GetVoltage
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
    double voltageBuffer = 0;
    double *voltagePtr = &voltageBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        voltagePtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = ma_battery_GetVoltage ( 
        voltagePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (voltagePtr)
    {
        LE_ASSERT(le_pack_PackDouble( &_msgBufPtr,
                                                      *voltagePtr ));
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


static void Handle_ma_battery_GetCurrent
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
    double currentBuffer = 0;
    double *currentPtr = &currentBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        currentPtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = ma_battery_GetCurrent ( 
        currentPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (currentPtr)
    {
        LE_ASSERT(le_pack_PackDouble( &_msgBufPtr,
                                                      *currentPtr ));
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


static void Handle_ma_battery_GetTemp
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
    double tempBuffer = 0;
    double *tempPtr = &tempBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        tempPtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = ma_battery_GetTemp ( 
        tempPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (tempPtr)
    {
        LE_ASSERT(le_pack_PackDouble( &_msgBufPtr,
                                                      *tempPtr ));
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


static void Handle_ma_battery_GetPercentRemaining
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
    uint16_t percentBuffer = 0;
    uint16_t *percentPtr = &percentBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        percentPtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = ma_battery_GetPercentRemaining ( 
        percentPtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (percentPtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *percentPtr ));
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


static void Handle_ma_battery_GetChargeRemaining
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
    uint16_t chargeBuffer = 0;
    uint16_t *chargePtr = &chargeBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        chargePtr = NULL;
    }

    // Unpack the input parameters from the message

    // Call the function
    le_result_t _result;
    _result  = ma_battery_GetChargeRemaining ( 
        chargePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (chargePtr)
    {
        LE_ASSERT(le_pack_PackUint16( &_msgBufPtr,
                                                      *chargePtr ));
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


static void AsyncResponse_ma_battery_AddLevelPercentageHandler
(
    uint8_t percentage,
    uint8_t percentageTrigger,
    bool isHighLevel,
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
    _msgPtr->id = _MSGID_ma_battery_AddLevelPercentageHandler;
    _msgBufPtr = _msgPtr->buffer;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(le_pack_PackUint8( &_msgBufPtr,
                                                  percentage ));
    LE_ASSERT(le_pack_PackUint8( &_msgBufPtr,
                                                  percentageTrigger ));
    LE_ASSERT(le_pack_PackBool( &_msgBufPtr,
                                                  isHighLevel ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_ma_battery_AddLevelPercentageHandler
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
    uint8_t levelLow = 0;
    uint8_t levelHigh = 0;
    void *contextPtr = NULL;
    _ServerData_t* serverDataPtr = NULL;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_pack_UnpackUint8( &_msgBufPtr,
                                               &levelLow ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackUint8( &_msgBufPtr,
                                               &levelHigh ))
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
    ma_battery_LevelPercentageHandlerRef_t _result;
    _result  = ma_battery_AddLevelPercentageHandler ( 
        levelLow, 
        levelHigh, AsyncResponse_ma_battery_AddLevelPercentageHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)ma_battery_RemoveLevelPercentageHandler;

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


static void Handle_ma_battery_RemoveLevelPercentageHandler
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
    ma_battery_LevelPercentageHandlerRef_t handlerRef = NULL;
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
    handlerRef = (ma_battery_LevelPercentageHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Call the function
    ma_battery_RemoveLevelPercentageHandler ( 
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


static void AsyncResponse_ma_battery_AddHealthChangeHandler
(
    ma_battery_HealthStatus_t health,
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
    _msgPtr->id = _MSGID_ma_battery_AddHealthChangeHandler;
    _msgBufPtr = _msgPtr->buffer;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(ma_battery_PackHealthStatus( &_msgBufPtr,
                                                  health ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_ma_battery_AddHealthChangeHandler
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
    void *contextPtr = NULL;
    _ServerData_t* serverDataPtr = NULL;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
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
    ma_battery_HealthChangeHandlerRef_t _result;
    _result  = ma_battery_AddHealthChangeHandler ( AsyncResponse_ma_battery_AddHealthChangeHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)ma_battery_RemoveHealthChangeHandler;

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


static void Handle_ma_battery_RemoveHealthChangeHandler
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
    ma_battery_HealthChangeHandlerRef_t handlerRef = NULL;
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
    handlerRef = (ma_battery_HealthChangeHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Call the function
    ma_battery_RemoveHealthChangeHandler ( 
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


static void AsyncResponse_ma_battery_AddChargingStatusChangeHandler
(
    ma_battery_ChargingStatus_t condition,
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
    _msgPtr->id = _MSGID_ma_battery_AddChargingStatusChangeHandler;
    _msgBufPtr = _msgPtr->buffer;

    // Always pack the client context pointer first
    LE_ASSERT(le_pack_PackReference( &_msgBufPtr, serverDataPtr->contextPtr ))

    // Pack the input parameters
    
    LE_ASSERT(ma_battery_PackChargingStatus( &_msgBufPtr,
                                                  condition ));

    // Send the async response to the client
    TRACE("Sending message to client session %p : %ti bytes sent",
          serverDataPtr->clientSessionRef,
          _msgBufPtr-_msgPtr->buffer);

    SendMsgToClient(_msgRef);
}


static void Handle_ma_battery_AddChargingStatusChangeHandler
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
    void *contextPtr = NULL;
    _ServerData_t* serverDataPtr = NULL;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
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
    ma_battery_ChargingStatusChangeHandlerRef_t _result;
    _result  = ma_battery_AddChargingStatusChangeHandler ( AsyncResponse_ma_battery_AddChargingStatusChangeHandler, 
        contextPtr );

    if (_result)
    {
        // Put the handler reference result and a pointer to the associated remove function
        // into the server data object.  This function pointer is needed in case the client
        // is closed and the handlers need to be removed.
        serverDataPtr->handlerRef = (le_event_HandlerRef_t)_result;
        serverDataPtr->removeHandlerFunc =
            (RemoveHandlerFunc_t)ma_battery_RemoveChargingStatusChangeHandler;

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


static void Handle_ma_battery_RemoveChargingStatusChangeHandler
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
    ma_battery_ChargingStatusChangeHandlerRef_t handlerRef = NULL;
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
    handlerRef = (ma_battery_ChargingStatusChangeHandlerRef_t)serverDataPtr->handlerRef;
    le_mem_Release(serverDataPtr);

    // Call the function
    ma_battery_RemoveChargingStatusChangeHandler ( 
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
        case _MSGID_ma_battery_GetChargingStatus :
            Handle_ma_battery_GetChargingStatus(msgRef);
            break;
        case _MSGID_ma_battery_GetHealthStatus :
            Handle_ma_battery_GetHealthStatus(msgRef);
            break;
        case _MSGID_ma_battery_GetVoltage :
            Handle_ma_battery_GetVoltage(msgRef);
            break;
        case _MSGID_ma_battery_GetCurrent :
            Handle_ma_battery_GetCurrent(msgRef);
            break;
        case _MSGID_ma_battery_GetTemp :
            Handle_ma_battery_GetTemp(msgRef);
            break;
        case _MSGID_ma_battery_GetPercentRemaining :
            Handle_ma_battery_GetPercentRemaining(msgRef);
            break;
        case _MSGID_ma_battery_GetChargeRemaining :
            Handle_ma_battery_GetChargeRemaining(msgRef);
            break;
        case _MSGID_ma_battery_AddLevelPercentageHandler :
            Handle_ma_battery_AddLevelPercentageHandler(msgRef);
            break;
        case _MSGID_ma_battery_RemoveLevelPercentageHandler :
            Handle_ma_battery_RemoveLevelPercentageHandler(msgRef);
            break;
        case _MSGID_ma_battery_AddHealthChangeHandler :
            Handle_ma_battery_AddHealthChangeHandler(msgRef);
            break;
        case _MSGID_ma_battery_RemoveHealthChangeHandler :
            Handle_ma_battery_RemoveHealthChangeHandler(msgRef);
            break;
        case _MSGID_ma_battery_AddChargingStatusChangeHandler :
            Handle_ma_battery_AddChargingStatusChangeHandler(msgRef);
            break;
        case _MSGID_ma_battery_RemoveChargingStatusChangeHandler :
            Handle_ma_battery_RemoveChargingStatusChangeHandler(msgRef);
            break;

        default: LE_ERROR("Unknowm msg id = %" PRIu32 , msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    LE_CDATA_THIS->_ClientSessionRef = 0;
}
