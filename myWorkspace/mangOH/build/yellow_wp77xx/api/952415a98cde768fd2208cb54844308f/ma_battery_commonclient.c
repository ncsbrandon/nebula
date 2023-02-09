/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

#include "ma_battery_common.h"
#include "ma_battery_messages.h"


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
#define HIGH_CLIENT_DATA_COUNT   4


//--------------------------------------------------------------------------------------------------
/**
 * Static memory pool for client data
 */
//--------------------------------------------------------------------------------------------------
LE_MEM_DEFINE_STATIC_POOL(ma_battery_ClientData,
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
LE_REF_DEFINE_STATIC_MAP(ma_battery_ClientHandlers,
    LE_MEM_BLOCKS(ma_battery_ClientData, HIGH_CLIENT_DATA_COUNT));


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
LE_SHARED bool ifgen_ma_battery_HasLocalBinding
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
LE_SHARED void ifgen_ma_battery_InitCommonData
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
            _ClientDataPool = le_mem_InitStaticPool(ma_battery_ClientData,
                                                    HIGH_CLIENT_DATA_COUNT,
                                                    sizeof(_ClientData_t));
        }


        if (!_HandlerRefMap)
        {
            // Create safe reference map for handler references.
            // The size of the map should be based on the number of handlers defined multiplied by
            // the number of client threads.  Since this number can't be completely determined at
            // build time, just make a reasonable guess.
            _HandlerRefMap = le_ref_InitStaticMap(ma_battery_ClientHandlers,
                                                  LE_MEM_BLOCKS(ma_battery_ClientData,
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
LE_SHARED le_result_t ifgen_ma_battery_OpenSession
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


//--------------------------------------------------------------------------------------------------
/**
 * Provides battery charging status
 *
 * @return Charging status code.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED ma_battery_ChargingStatus_t ifgen_ma_battery_GetChargingStatus
(
    le_msg_SessionRef_t _ifgen_sessionRef
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    ma_battery_ChargingStatus_t _result =
        (ma_battery_ChargingStatus_t) 0;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_ma_battery_GetChargingStatus;
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
    if (!ma_battery_UnpackChargingStatus( &_msgBufPtr, &_result ))
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
 * Provides battery health status
 *
 * @return Health status code.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED ma_battery_HealthStatus_t ifgen_ma_battery_GetHealthStatus
(
    le_msg_SessionRef_t _ifgen_sessionRef
)
{
    le_msg_MessageRef_t _msgRef;
    le_msg_MessageRef_t _responseMsgRef;
    _Message_t* _msgPtr;

    // Will not be used if no data is sent/received from server.
    __attribute__((unused)) uint8_t* _msgBufPtr;

    ma_battery_HealthStatus_t _result =
        (ma_battery_HealthStatus_t) 0;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_ma_battery_GetHealthStatus;
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
    if (!ma_battery_UnpackHealthStatus( &_msgBufPtr, &_result ))
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
 * Get battery voltage (in Volts)
 *
 * @return
 *     - LE_OK on success.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED le_result_t ifgen_ma_battery_GetVoltage
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    double* voltagePtr
        ///< [OUT] The battery voltage, in V, if LE_OK is returned.
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
    _msgPtr->id = _MSGID_ma_battery_GetVoltage;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.
    uint32_t _requiredOutputs = 0;
    _requiredOutputs |= ((!!(voltagePtr)) << 0);
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
    if (voltagePtr &&
        (!le_pack_UnpackDouble( &_msgBufPtr,
                                               voltagePtr )))
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
 * Get battery current (in mA)
 *
 * @return
 *     - LE_OK on success.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED le_result_t ifgen_ma_battery_GetCurrent
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    double* currentPtr
        ///< [OUT] The battery current, in mA, if LE_OK is returned.
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
    _msgPtr->id = _MSGID_ma_battery_GetCurrent;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.
    uint32_t _requiredOutputs = 0;
    _requiredOutputs |= ((!!(currentPtr)) << 0);
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
    if (currentPtr &&
        (!le_pack_UnpackDouble( &_msgBufPtr,
                                               currentPtr )))
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
 * Get battery temperature in degrees Celcius
 *
 * @return
 *     - LE_OK on success.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED le_result_t ifgen_ma_battery_GetTemp
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    double* tempPtr
        ///< [OUT] Temperature in degrees Celcius, if LE_OK is returned.
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
    _msgPtr->id = _MSGID_ma_battery_GetTemp;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.
    uint32_t _requiredOutputs = 0;
    _requiredOutputs |= ((!!(tempPtr)) << 0);
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
    if (tempPtr &&
        (!le_pack_UnpackDouble( &_msgBufPtr,
                                               tempPtr )))
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
 * Get charge remaining, in percentage
 *
 * @return
 *     - LE_OK on success
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED le_result_t ifgen_ma_battery_GetPercentRemaining
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    uint16_t* percentPtr
        ///< [OUT] Percentage battery remaining, if LE_OK is returned.
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
    _msgPtr->id = _MSGID_ma_battery_GetPercentRemaining;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.
    uint32_t _requiredOutputs = 0;
    _requiredOutputs |= ((!!(percentPtr)) << 0);
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
    if (percentPtr &&
        (!le_pack_UnpackUint16( &_msgBufPtr,
                                               percentPtr )))
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
 * Get charge remaining, in mAh
 *
 * @ return
 *    - LE_OK on success
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED le_result_t ifgen_ma_battery_GetChargeRemaining
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    uint16_t* chargePtr
        ///< [OUT] Charge in mAh remaining, if LE_OK is returned.
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
    _msgPtr->id = _MSGID_ma_battery_GetChargeRemaining;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.
    uint32_t _requiredOutputs = 0;
    _requiredOutputs |= ((!!(chargePtr)) << 0);
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
    if (chargePtr &&
        (!le_pack_UnpackUint16( &_msgBufPtr,
                                               chargePtr )))
    {
        goto error_unpack;
    }


    // Release the message object, now that all results/output has been copied.
    le_msg_ReleaseMsg(_responseMsgRef);


    return _result;

error_unpack:
    LE_FATAL("Unexpected response from server.");
}


// This function parses the message buffer received from the server, and then calls the user
// registered handler, which is stored in a client data object.
static void _Handle_ifgen_ma_battery_AddLevelPercentageHandler
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
    uint8_t percentage = 0;
    uint8_t percentageTrigger = 0;
    bool isHighLevel = false;

    // Pull out additional data from the client data pointer
    ma_battery_LevelPercentageHandlerFunc_t _handlerRef_ifgen_ma_battery_AddLevelPercentageHandler =
        (ma_battery_LevelPercentageHandlerFunc_t)_clientDataPtr->handlerPtr;
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
    if (!le_pack_UnpackUint8( &_msgBufPtr,
                                               &percentage ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackUint8( &_msgBufPtr,
                                               &percentageTrigger ))
    {
        goto error_unpack;
    }
    if (!le_pack_UnpackBool( &_msgBufPtr,
                                               &isHighLevel ))
    {
        goto error_unpack;
    }

    // Call the registered handler
    if ( _handlerRef_ifgen_ma_battery_AddLevelPercentageHandler != NULL )
    {
        _handlerRef_ifgen_ma_battery_AddLevelPercentageHandler(percentage, percentageTrigger, isHighLevel, contextPtr );
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
 * Add handler function for EVENT 'ma_battery_LevelPercentage'
 *
 * Register a callback function to be called the battery level goes above levelHigh or
 * below levelLow.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED ma_battery_LevelPercentageHandlerRef_t ifgen_ma_battery_AddLevelPercentageHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    uint8_t levelLow,
        ///< [IN] Low percentage trigger threshold (0 = no low level alarm)
    uint8_t levelHigh,
        ///< [IN] High percentage trigger threshold (>=100 means no high alm)
    ma_battery_LevelPercentageHandlerFunc_t handlerPtr,
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

    ma_battery_LevelPercentageHandlerRef_t _result =
        NULL;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_ma_battery_AddLevelPercentageHandler;
    _msgBufPtr = _msgPtr->buffer;

    // Pack a list of outputs requested by the client.

    // Pack the input parameters
    LE_ASSERT(le_pack_PackUint8( &_msgBufPtr,
                                                  levelLow ));
    LE_ASSERT(le_pack_PackUint8( &_msgBufPtr,
                                                  levelHigh ));
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
 * Remove handler function for EVENT 'ma_battery_LevelPercentage'
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED void ifgen_ma_battery_RemoveLevelPercentageHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    ma_battery_LevelPercentageHandlerRef_t handlerRef
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
    _msgPtr->id = _MSGID_ma_battery_RemoveLevelPercentageHandler;
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
    handlerRef = (ma_battery_LevelPercentageHandlerRef_t)
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


// This function parses the message buffer received from the server, and then calls the user
// registered handler, which is stored in a client data object.
static void _Handle_ifgen_ma_battery_AddHealthChangeHandler
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
    ma_battery_HealthStatus_t health = (ma_battery_HealthStatus_t) 0;

    // Pull out additional data from the client data pointer
    ma_battery_HealthHandlerFunc_t _handlerRef_ifgen_ma_battery_AddHealthChangeHandler =
        (ma_battery_HealthHandlerFunc_t)_clientDataPtr->handlerPtr;
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
    if (!ma_battery_UnpackHealthStatus( &_msgBufPtr,
                                               &health ))
    {
        goto error_unpack;
    }

    // Call the registered handler
    if ( _handlerRef_ifgen_ma_battery_AddHealthChangeHandler != NULL )
    {
        _handlerRef_ifgen_ma_battery_AddHealthChangeHandler(health, contextPtr );
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
 * Add handler function for EVENT 'ma_battery_HealthChange'
 *
 * Register a callback function to be called when there is a change in battery health status.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED ma_battery_HealthChangeHandlerRef_t ifgen_ma_battery_AddHealthChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    ma_battery_HealthHandlerFunc_t handlerPtr,
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

    ma_battery_HealthChangeHandlerRef_t _result =
        NULL;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_ma_battery_AddHealthChangeHandler;
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
 * Remove handler function for EVENT 'ma_battery_HealthChange'
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED void ifgen_ma_battery_RemoveHealthChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    ma_battery_HealthChangeHandlerRef_t handlerRef
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
    _msgPtr->id = _MSGID_ma_battery_RemoveHealthChangeHandler;
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
    handlerRef = (ma_battery_HealthChangeHandlerRef_t)
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


// This function parses the message buffer received from the server, and then calls the user
// registered handler, which is stored in a client data object.
static void _Handle_ifgen_ma_battery_AddChargingStatusChangeHandler
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
    ma_battery_ChargingStatus_t condition = (ma_battery_ChargingStatus_t) 0;

    // Pull out additional data from the client data pointer
    ma_battery_ChargingStatusHandlerFunc_t _handlerRef_ifgen_ma_battery_AddChargingStatusChangeHandler =
        (ma_battery_ChargingStatusHandlerFunc_t)_clientDataPtr->handlerPtr;
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
    if (!ma_battery_UnpackChargingStatus( &_msgBufPtr,
                                               &condition ))
    {
        goto error_unpack;
    }

    // Call the registered handler
    if ( _handlerRef_ifgen_ma_battery_AddChargingStatusChangeHandler != NULL )
    {
        _handlerRef_ifgen_ma_battery_AddChargingStatusChangeHandler(condition, contextPtr );
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
 * Add handler function for EVENT 'ma_battery_ChargingStatusChange'
 *
 * Register a callback function to be called when there is a change in battery charging status.
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED ma_battery_ChargingStatusChangeHandlerRef_t ifgen_ma_battery_AddChargingStatusChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    ma_battery_ChargingStatusHandlerFunc_t handlerPtr,
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

    ma_battery_ChargingStatusChangeHandlerRef_t _result =
        NULL;

    // Range check values, if appropriate


    // Create a new message object and get the message buffer
    _msgRef = le_msg_CreateMsg(_ifgen_sessionRef);
    _msgPtr = le_msg_GetPayloadPtr(_msgRef);
    _msgPtr->id = _MSGID_ma_battery_AddChargingStatusChangeHandler;
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
 * Remove handler function for EVENT 'ma_battery_ChargingStatusChange'
 */
//--------------------------------------------------------------------------------------------------
__attribute__((weak))
LE_SHARED void ifgen_ma_battery_RemoveChargingStatusChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    ma_battery_ChargingStatusChangeHandlerRef_t handlerRef
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
    _msgPtr->id = _MSGID_ma_battery_RemoveChargingStatusChangeHandler;
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
    handlerRef = (ma_battery_ChargingStatusChangeHandlerRef_t)
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
        case _MSGID_ma_battery_AddLevelPercentageHandler :
            le_event_QueueFunctionToThread(callersThreadRef, _Handle_ifgen_ma_battery_AddLevelPercentageHandler, msgRef, clientDataPtr);
            break;
        case _MSGID_ma_battery_AddHealthChangeHandler :
            le_event_QueueFunctionToThread(callersThreadRef, _Handle_ifgen_ma_battery_AddHealthChangeHandler, msgRef, clientDataPtr);
            break;
        case _MSGID_ma_battery_AddChargingStatusChangeHandler :
            le_event_QueueFunctionToThread(callersThreadRef, _Handle_ifgen_ma_battery_AddChargingStatusChangeHandler, msgRef, clientDataPtr);
            break;

        default:
            LE_FATAL("Unknowm msg id = %" PRIu32 " for client thread = %p",
                msgPtr->id, callersThreadRef);
    }
}
