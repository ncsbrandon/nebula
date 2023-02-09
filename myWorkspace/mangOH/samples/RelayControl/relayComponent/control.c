/**
 * @file
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless, Inc. Use of this work is subject to license.
 */

#include "legato.h"
#include "interfaces.h"

#define KEY_COUNT_REQ      "RelayCountReq"
#define KEY_COUNT_RSP      "RelayCountRsp"

#define KEY_GETSTATE_REQ   "GetRelayStateReq"
#define KEY_GETSTATE_RSP   "GetRelayStateRsp"

#define KEY_SETSTATE_REQ   "SetRelayStateReq"

#define MSG_MAX_LEN      25

// GetState async
static le_sem_Ref_t GetStateSem;
static int8_t GetStateReqRelayId;
static bool GetStateResp;

static le_sem_Ref_t CountSem;
static int8_t CountResp = -1;

static le_thread_Ref_t ResponseThread;

static const le_clk_Time_t Timeout = {10, 0};

//--------------------------------------------------------------------------------------------------
/**
 * Returns the total number of relays controlled by the system.
 */
//--------------------------------------------------------------------------------------------------
le_result_t relayControl_Count(uint8_t *countPtr)
{
    if (CountResp != -1)
    {
        *countPtr = CountResp;
        return LE_OK;
    }

    // Make sure there is no pending results
    while (LE_OK == le_sem_TryWait(CountSem));

    // Issue request
    dataRouter_WriteString(KEY_COUNT_REQ, "x", time(NULL));

    // Wait for response
    if (LE_TIMEOUT == le_sem_WaitWithTimeOut(CountSem, Timeout))
    {
        LE_ERROR("Unable to get number of relays in time");
        return LE_TIMEOUT;
    }

    *countPtr = CountResp;
    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Get the state of a relay.
 */
//--------------------------------------------------------------------------------------------------
le_result_t relayControl_GetState(uint8_t id, bool *statePtr)
{
    char buffer[MSG_MAX_LEN];
    uint8_t maxId;

    if (LE_OK != relayControl_Count(&maxId))
    {
        return LE_NOT_POSSIBLE;
    }

    if(id >= maxId)
    {
        LE_ERROR("Relay id %u not controlled.", id);
        return LE_OUT_OF_RANGE;
    }

    snprintf(buffer, MSG_MAX_LEN, "%d", id);
    LE_DEBUG("GetState: %s", buffer);

    // Make sure there is no pending results
    while (LE_OK == le_sem_TryWait(GetStateSem));

    // Issue request
    GetStateReqRelayId = id;
    dataRouter_WriteString(KEY_GETSTATE_REQ, buffer, time(NULL));

    // Wait for response
    if (LE_TIMEOUT == le_sem_WaitWithTimeOut(GetStateSem, Timeout))
    {
        LE_ERROR("Unable to get state for relay %d in time", id);
        return LE_TIMEOUT;
    }

    *statePtr = GetStateResp;
    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Set the state of a relay.
 */
//--------------------------------------------------------------------------------------------------
le_result_t relayControl_SetState(uint8_t id, bool statePtr)
{
    char buffer[MSG_MAX_LEN];
    uint8_t maxId;

    if (LE_OK != relayControl_Count(&maxId))
    {
        return LE_NOT_POSSIBLE;
    }

    if(id >= maxId)
    {
        LE_ERROR("Relay id %u not controlled.", id);
        return LE_OUT_OF_RANGE;
    }

    snprintf(buffer, MSG_MAX_LEN, "%d,%d", id, statePtr);
    LE_DEBUG("SetState: %s", buffer);
    dataRouter_WriteString(KEY_SETSTATE_REQ, buffer, time(NULL));

    return LE_OK;
}

static void GetStateResponseHandler(dataRouter_DataType_t type, const char * keyPtr, void * contextPtr)
{
    char buf[10] = {0};
    uint32_t timestamp;

    LE_INFO("GetState response received %s", keyPtr);

    dataRouter_ReadString(keyPtr, buf, sizeof(buf), &timestamp);
    LE_DEBUG("response: %s", buf);

    int res, relayId, state;
    res = sscanf(buf, "%d,%d", &relayId, &state);
    LE_ASSERT(res == 2);

    if (GetStateReqRelayId != relayId)
    {
        LE_ERROR("Unexpected response from relay control (expected=%d, got=%d)",
            GetStateReqRelayId, relayId);
        return;
    }

    GetStateResp = (state == 1);
    le_sem_Post(GetStateSem);
}

static void CountResponseHandler(dataRouter_DataType_t type, const char * keyPtr, void * contextPtr)
{
    int32_t value;
    uint32_t timestamp;

    LE_INFO("Count response received %s", keyPtr);

    dataRouter_ReadInteger(keyPtr, &value, &timestamp);
    LE_DEBUG("count: %d", value);
    CountResp = value;

    le_sem_Post(CountSem);
}

static void * MainResponseHandler(void * context)
{
    dataRouter_ConnectService();

    LE_INFO("Starting session");
    dataRouter_SessionStart("","", 0, DATAROUTER_CACHE);

    // Register handlers to process async results from Arduino
    GetStateSem = le_sem_Create("GetStateReq", 0);
    dataRouter_AddDataUpdateHandler(KEY_GETSTATE_RSP, GetStateResponseHandler, NULL);

    CountSem = le_sem_Create("CountReq", 0);
    dataRouter_AddDataUpdateHandler(KEY_COUNT_RSP, CountResponseHandler, NULL);

    le_event_RunLoop();
    return NULL;
}

//--------------------------------------------------------------------------------------------------
/**
 * Initialization function for the relay component
 */
//--------------------------------------------------------------------------------------------------
COMPONENT_INIT
{
    // Start routing session
    LE_INFO("Starting session");
    dataRouter_SessionStart("","", 0, DATAROUTER_CACHE);

    // Init thread to process responses
    ResponseThread = le_thread_Create("ResponseThread", MainResponseHandler, NULL);
    le_thread_Start(ResponseThread);

    // Reset arduino upon app start to make sure we are in a default state
    LE_INFO("Reset arduino");
    mangoh_muxCtrl_ArduinoReset();
}
