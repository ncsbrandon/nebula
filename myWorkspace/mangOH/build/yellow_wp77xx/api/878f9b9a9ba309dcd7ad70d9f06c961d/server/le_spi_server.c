
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#include "le_spi_server.h"
#include "le_spi_messages.h"
#include "le_spi_service.h"


//--------------------------------------------------------------------------------------------------
// Generic Server Types, Variables and Functions
//--------------------------------------------------------------------------------------------------



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
le_msg_ServiceRef_t le_spi_GetServiceRef
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
le_msg_SessionRef_t le_spi_GetClientSessionRef
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
void le_spi_AdvertiseService
(
    void
)
{
    LE_DEBUG("======= Starting Server %s ========", SERVICE_INSTANCE_NAME);

    // Get a reference to the trace keyword that is used to control tracing in this module.
#if defined(MK_TOOLS_BUILD) && !defined(NO_LOG_SESSION)
    TraceRef = le_log_GetTraceRef("ipc");
#endif

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


static void Handle_le_spi_Open
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
    char deviceName[129] = {0};

    // Define storage for output parameters
    le_spi_DeviceHandleRef_t handleBuffer = NULL;
    le_spi_DeviceHandleRef_t *handlePtr = &handleBuffer;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        handlePtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackString( &_msgBufPtr,
                               deviceName,
                               sizeof(deviceName),
                               128 ))
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_spi_Open ( 
        deviceName, 
        handlePtr );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (handlePtr)
    {
        LE_ASSERT(le_pack_PackReference( &_msgBufPtr,
                                                      *handlePtr ));
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


static void Handle_le_spi_Close
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
    le_spi_DeviceHandleRef_t handle = NULL;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &handle ))
    {
        goto error_unpack;
    }

    // Call the function
    le_spi_Close ( 
        handle );

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


static void Handle_le_spi_Configure
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
    le_spi_DeviceHandleRef_t handle = NULL;
    int32_t mode = 0;
    uint8_t bits = 0;
    uint32_t speed = 0;
    int32_t msb = 0;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &handle ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackInt32( &_msgBufPtr,
                                               &mode ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackUint8( &_msgBufPtr,
                                               &bits ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackUint32( &_msgBufPtr,
                                               &speed ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackInt32( &_msgBufPtr,
                                               &msb ))
    {
        goto error_unpack;
    }

    // Call the function
    le_spi_Configure ( 
        handle, 
        mode, 
        bits, 
        speed, 
        msb );

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


static void Handle_le_spi_WriteReadHD
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
    le_spi_DeviceHandleRef_t handle = NULL;
    size_t writeDataSize = 0;
    uint8_t
        writeDataPtr[1024]
            = { 0 };
    bool writeDataResult;
    size_t readDataSize = 0;

    // Define storage for output parameters
    uint8_t readDataBuffer[1024] = { 0 };
    uint8_t *readDataPtr = readDataBuffer;
    size_t *readDataSizePtr = &readDataSize;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        readDataPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &handle ))
    {
        goto error_unpack;
    }
            LE_PACK_UNPACKARRAY( &_msgBufPtr,
                         writeDataPtr, &writeDataSize,
                         1024,
                         le_pack_UnpackUint8,
                         &writeDataResult );
    if (!writeDataResult)
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackSize( &_msgBufPtr,
                             &readDataSize ))
    {
        goto error_unpack;
    }
    if ( (readDataSize > 1024) )
    {
        LE_DEBUG("Adjusting readDataSize from %" PRIuS " to 1024", readDataSize);
        readDataSize = 1024;
    }

    // Call the function
    le_result_t _result;
    _result  = le_spi_WriteReadHD ( 
        handle, 
        writeDataPtr, 
        writeDataSize, 
        readDataPtr, 
        &readDataSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (readDataPtr)
    {
        bool readDataResult;
            LE_PACK_PACKARRAY( &_msgBufPtr,
                           readDataPtr, (*readDataSizePtr),
                           1024, le_pack_PackUint8,
                           &readDataResult );
        LE_ASSERT(readDataResult);
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


static void Handle_le_spi_WriteHD
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
    le_spi_DeviceHandleRef_t handle = NULL;
    size_t writeDataSize = 0;
    uint8_t
        writeDataPtr[1024]
            = { 0 };
    bool writeDataResult;

    // Define storage for output parameters

    // Unpack which outputs are needed

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &handle ))
    {
        goto error_unpack;
    }
            LE_PACK_UNPACKARRAY( &_msgBufPtr,
                         writeDataPtr, &writeDataSize,
                         1024,
                         le_pack_UnpackUint8,
                         &writeDataResult );
    if (!writeDataResult)
    {
        goto error_unpack;
    }

    // Call the function
    le_result_t _result;
    _result  = le_spi_WriteHD ( 
        handle, 
        writeDataPtr, 
        writeDataSize );

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


static void Handle_le_spi_ReadHD
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
    le_spi_DeviceHandleRef_t handle = NULL;
    size_t readDataSize = 0;

    // Define storage for output parameters
    uint8_t readDataBuffer[1024] = { 0 };
    uint8_t *readDataPtr = readDataBuffer;
    size_t *readDataSizePtr = &readDataSize;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        readDataPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &handle ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackSize( &_msgBufPtr,
                             &readDataSize ))
    {
        goto error_unpack;
    }
    if ( (readDataSize > 1024) )
    {
        LE_DEBUG("Adjusting readDataSize from %" PRIuS " to 1024", readDataSize);
        readDataSize = 1024;
    }

    // Call the function
    le_result_t _result;
    _result  = le_spi_ReadHD ( 
        handle, 
        readDataPtr, 
        &readDataSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (readDataPtr)
    {
        bool readDataResult;
            LE_PACK_PACKARRAY( &_msgBufPtr,
                           readDataPtr, (*readDataSizePtr),
                           1024, le_pack_PackUint8,
                           &readDataResult );
        LE_ASSERT(readDataResult);
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


static void Handle_le_spi_WriteReadFD
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
    le_spi_DeviceHandleRef_t handle = NULL;
    size_t writeDataSize = 0;
    uint8_t
        writeDataPtr[1024]
            = { 0 };
    bool writeDataResult;
    size_t readDataSize = 0;

    // Define storage for output parameters
    uint8_t readDataBuffer[1024] = { 0 };
    uint8_t *readDataPtr = readDataBuffer;
    size_t *readDataSizePtr = &readDataSize;

    // Unpack which outputs are needed
    uint32_t _requiredOutputs = 0;
    if (!le_pack_UnpackUint32(&_msgBufPtr, &_requiredOutputs))
    {
        goto error_unpack;
    }
    if (!(_requiredOutputs & (1u << 0)))
    {
        readDataPtr = NULL;
    }

    // Unpack the input parameters from the message
    if (!le_pack_UnpackReference( &_msgBufPtr,
                                               &handle ))
    {
        goto error_unpack;
    }
            LE_PACK_UNPACKARRAY( &_msgBufPtr,
                         writeDataPtr, &writeDataSize,
                         1024,
                         le_pack_UnpackUint8,
                         &writeDataResult );
    if (!writeDataResult)
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackSize( &_msgBufPtr,
                             &readDataSize ))
    {
        goto error_unpack;
    }
    if ( (readDataSize > 1024) )
    {
        LE_DEBUG("Adjusting readDataSize from %" PRIuS " to 1024", readDataSize);
        readDataSize = 1024;
    }

    // Call the function
    le_result_t _result;
    _result  = le_spi_WriteReadFD ( 
        handle, 
        writeDataPtr, 
        writeDataSize, 
        readDataPtr, 
        &readDataSize );

    // Re-use the message buffer for the response
    _msgBufPtr = _msgBufStartPtr;

    // Pack the result first
    LE_ASSERT(le_pack_PackResult( &_msgBufPtr, _result ));

    // Pack any "out" parameters
    if (readDataPtr)
    {
        bool readDataResult;
            LE_PACK_PACKARRAY( &_msgBufPtr,
                           readDataPtr, (*readDataSizePtr),
                           1024, le_pack_PackUint8,
                           &readDataResult );
        LE_ASSERT(readDataResult);
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
        case _MSGID_le_spi_Open :
            Handle_le_spi_Open(msgRef);
            break;
        case _MSGID_le_spi_Close :
            Handle_le_spi_Close(msgRef);
            break;
        case _MSGID_le_spi_Configure :
            Handle_le_spi_Configure(msgRef);
            break;
        case _MSGID_le_spi_WriteReadHD :
            Handle_le_spi_WriteReadHD(msgRef);
            break;
        case _MSGID_le_spi_WriteHD :
            Handle_le_spi_WriteHD(msgRef);
            break;
        case _MSGID_le_spi_ReadHD :
            Handle_le_spi_ReadHD(msgRef);
            break;
        case _MSGID_le_spi_WriteReadFD :
            Handle_le_spi_WriteReadFD(msgRef);
            break;

        default: LE_ERROR("Unknowm msg id = %" PRIu32 , msgPtr->id);
    }

    // Clear the client session ref associated with the current message, since the message
    // has now been processed.
    LE_CDATA_THIS->_ClientSessionRef = 0;
}
