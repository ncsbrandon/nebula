/**
 * @file
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless, Inc. Use of this work is subject to license.
 */

#include "interfaces.h"
#include "legato.h"

#include "router.h"
#include "mqtt.h"

static void swi_mangoh_data_router_mqttIncomingMsgHdlr(
    const char*,
    const char*,
    const char*,
    const char*,
    void*);
static void swi_mangoh_data_router_mqttSessionStateHdlr(bool, int32_t, int32_t, void*);

static void swi_mangoh_data_router_mqttIncomingMsgHdlr(
    const char* topic,
    const char* key,
    const char* value,
    const char* timestamp,
    void*       context)
{
    swi_mangoh_data_router_mqtt_t* mqtt = (swi_mangoh_data_router_mqtt_t*)context;

    LE_ASSERT(topic);
    LE_ASSERT(key);
    LE_ASSERT(value);
    LE_ASSERT(timestamp);

    LE_DEBUG("MQTT --> key('%s'), value('%s'), timestamp('%s')", key, value, timestamp);

    swi_mangoh_data_router_dbItem_t* dbItem = swi_mangoh_data_router_db_getDataItem(mqtt->db, key);
    if (!dbItem)
    {
        LE_ERROR("ERROR swi_mangoh_data_router_db_getDataItem() failed");
        goto cleanup;
    }

    switch (dbItem->data.type)
    {
        case DATAROUTER_BOOLEAN:
            dbItem->data.bValue = strcmp(value, "true") ? false : true;
            break;

        case DATAROUTER_INTEGER:
            dbItem->data.iValue = atoi(value);
            break;

        case DATAROUTER_FLOAT:
            dbItem->data.fValue = atof(value);
            break;

        case DATAROUTER_STRING:
            strcpy(dbItem->data.sValue, value);
            break;
    }

    dbItem->data.timestamp = atoi(timestamp);

    swi_mangoh_data_router_notifySubscribers(key, dbItem);

cleanup:
    return;
}

static void swi_mangoh_data_router_mqttSessionStateHdlr(
    bool    isConnected,
    int32_t errorCode,
    int32_t subErrorCode,
    void*   context)
{
    swi_mangoh_data_router_mqtt_t* mqtt = (swi_mangoh_data_router_mqtt_t*)context;
    le_result_t                    res  = LE_OK;

    LE_ASSERT(context);

    LE_INFO("MQTT --> connected(%d), error code(%d/%d)", isConnected, errorCode, subErrorCode);
    mqtt->connecting = false;
    mqtt->connected  = isConnected;

    if (mqtt->connected)
    {
        le_sls_Link_t* linkPtr = le_sls_Pop(&mqtt->outstandingRequests);
        while (linkPtr)
        {
            char value[SWI_MANGOH_DATA_ROUTER_DATA_MAX_LEN] = {0};
            swi_mangoh_data_router_mqtt_dataLink_t* dataElem =
                CONTAINER_OF(linkPtr, swi_mangoh_data_router_mqtt_dataLink_t, link);
            int32_t error = 0;

            switch (dataElem->data.type)
            {
                case DATAROUTER_BOOLEAN:
                    strncpy(
                        value,
                        dataElem->data.bValue ? "true" : "false",
                        SWI_MANGOH_DATA_ROUTER_DATA_MAX_LEN);
                    LE_ASSERT(value[SWI_MANGOH_DATA_ROUTER_DATA_MAX_LEN - 1] == '\0');
                    break;

                case DATAROUTER_INTEGER:
                    snprintf(value, sizeof(value), "%d", dataElem->data.iValue);
                    break;

                case DATAROUTER_FLOAT:
                    snprintf(value, sizeof(value), "%f", dataElem->data.fValue);
                    break;

                case DATAROUTER_STRING:
                    strcpy(value, dataElem->data.sValue);
                    break;
            }

            LE_DEBUG("<-- key/value('%s'/'%s')", dataElem->key, value);
            mqtt_Send(dataElem->key, value, &error);
            if (error)
            {
                LE_ERROR("mqtt_Send() failed(%d)", error);
            }

            free(dataElem);
            linkPtr = le_sls_Pop(&mqtt->outstandingRequests);
        }

        if (mqtt->disconnect)
        {
            LE_DEBUG("disconnect MQTT session");
            mqtt_Disconnect();
        }
    }
    else
    {
        LE_DEBUG("reconnect MQTT session");
        res = le_timer_Start(mqtt->reconnectTimer);
        if (res != LE_OK)
        {
            LE_ERROR("ERROR le_timer_Start() failed(%d)", res);
            goto cleanup;
        }
    }

cleanup:
    return;
}


void swi_mangoh_data_router_mqttReconnect(le_timer_Ref_t timerRef)
{
    swi_mangoh_data_router_mqtt_t* mqtt =
        (swi_mangoh_data_router_mqtt_t*)le_timer_GetContextPtr(timerRef);

    LE_ASSERT(mqtt);

    LE_DEBUG("connect -> AV('%s')", mqtt->password);
    mqtt_Connect(mqtt->password);
    mqtt->connecting = true;
}

void swi_mangoh_data_router_mqttSessionStart(
    const char*                    appId,
    const char*                    url,
    const char*                    password,
    swi_mangoh_data_router_mqtt_t* mqtt,
    swi_mangoh_data_router_db_t*   db)
{
    le_result_t res = LE_OK;

    LE_ASSERT(appId);
    LE_ASSERT(url);
    LE_ASSERT(password);
    LE_ASSERT(mqtt);
    LE_ASSERT(db);

    mqtt->reconnectTimer = le_timer_Create(appId);
    res = le_timer_SetHandler(mqtt->reconnectTimer, swi_mangoh_data_router_mqttReconnect);
    if (res != LE_OK)
    {
        LE_ERROR("ERROR le_timer_SetHandler() failed(%d)", res);
        goto cleanup;
    }

    le_clk_Time_t interval = {.sec  = SWI_MANGOH_DATA_ROUTER_MQTT_RECONNECT_INTERVAL_SECS,
                              .usec = 0};
    res = le_timer_SetInterval(mqtt->reconnectTimer, interval);
    if (res != LE_OK)
    {
        LE_ERROR("ERROR le_timer_SetInterval() failed(%d)", res);
        goto cleanup;
    }

    res = le_timer_SetContextPtr(mqtt->reconnectTimer, mqtt);
    if (res != LE_OK)
    {
        LE_ERROR("ERROR le_timer_SetContextPtr() failed(%d)", res);
        goto cleanup;
    }

    mqtt->sessionStateHdlrRef =
        mqtt_AddSessionStateHandler(swi_mangoh_data_router_mqttSessionStateHdlr, mqtt);
    if (!mqtt->sessionStateHdlrRef)
    {
        LE_ERROR("ERROR mqtt_AddSessionStateHandler() failed");
        goto cleanup;
    }

    mqtt->incomingMsgHdlrRef =
        mqtt_AddIncomingMessageHandler(swi_mangoh_data_router_mqttIncomingMsgHdlr, mqtt);
    if (!mqtt->incomingMsgHdlrRef)
    {
        LE_ERROR("ERROR mqtt_AddIncomingMessageHandler() failed");
        goto cleanup;
    }

    mqtt->db                  = db;
    mqtt->outstandingRequests = LE_SLS_LIST_INIT;
    strcpy(mqtt->url, url);
    strcpy(mqtt->password, password);

    LE_DEBUG("url('%s')", mqtt->url);
    mqtt_Config(
        mqtt->url,
        SWI_MANGOH_DATA_ROUTER_MQTT_PORT_NUMBER,
        SWI_MANGOH_DATA_ROUTER_MQTT_KEEP_ALIVE,
        0);

    LE_DEBUG("connect -> AV('%s')", mqtt->password);
    mqtt_Connect(mqtt->password);
    mqtt->connecting = true;

cleanup:
    return;
}

void swi_mangoh_data_router_mqttWrite(
    const char*                      key,
    const swi_mangoh_data_router_dbItem_t* dbItem,
    swi_mangoh_data_router_mqtt_t*   mqtt)
{
    LE_ASSERT(dbItem);
    LE_ASSERT(mqtt);

    if (mqtt->connected)
    {
        char    value[SWI_MANGOH_DATA_ROUTER_DATA_MAX_LEN] = {0};
        int32_t error                                      = 0;

        switch (dbItem->data.type)
        {
            case DATAROUTER_BOOLEAN:
                strncpy(
                    value,
                    dbItem->data.bValue ? "true" : "false",
                    SWI_MANGOH_DATA_ROUTER_DATA_MAX_LEN);
                LE_ASSERT(value[SWI_MANGOH_DATA_ROUTER_DATA_MAX_LEN - 1] == '\0');
                break;

            case DATAROUTER_INTEGER:
                snprintf(value, sizeof(value), "%d", dbItem->data.iValue);
                break;

            case DATAROUTER_FLOAT:
                snprintf(value, sizeof(value), "%f", dbItem->data.fValue);
                break;

            case DATAROUTER_STRING:
                strcpy(value, dbItem->data.sValue);
                break;
        }

        LE_DEBUG(
            "MQTT <-- key('%s'), value('%s'), timestamp(%lu)", key, value, dbItem->data.timestamp);
        mqtt_Send(key, value, &error);
        if (error)
        {
            LE_ERROR("mqtt_Send() failed(%d)", error);
        }
    }
    else
    {
        if (le_sls_NumLinks(&mqtt->outstandingRequests) <
            SWI_MANGOH_DATA_ROUTER_MQTT_QUEUED_REQUESTS_MAX_NUM)
        {
            swi_mangoh_data_router_mqtt_dataLink_t* dataElem =
                calloc(1, sizeof(swi_mangoh_data_router_mqtt_dataLink_t));
            if (!dataElem)
            {
                LE_ERROR("ERROR calloc() failed");
                goto cleanup;
            }

            LE_DEBUG("queue('%s')", key);
            strcpy(dataElem->key, key);
            memcpy(&dataElem->data, &dbItem->data, sizeof(swi_mangoh_data_router_data_t));
            dataElem->link = LE_SLS_LINK_INIT;
            le_sls_Queue(&mqtt->outstandingRequests, &dataElem->link);
        }
        else
        {
            LE_WARN("cannot queue('%s') data update", key);
        }
    }

cleanup:
    return;
}

bool swi_mangoh_data_router_mqttSessionEnd(swi_mangoh_data_router_mqtt_t* mqtt)
{
    bool ret = true;

    LE_ASSERT(mqtt);

    if (mqtt->connecting && !le_sls_IsEmpty(&mqtt->outstandingRequests))
    {
        LE_DEBUG("delayed MQTT session disconnect");
        mqtt->disconnect = true;
        ret              = false;
    }
    else
    {
        LE_DEBUG("disconnect MQTT session");
        mqtt_Disconnect();

        LE_DEBUG("remove MQTT session");
        mqtt_RemoveSessionStateHandler(mqtt->sessionStateHdlrRef);
        mqtt_RemoveIncomingMessageHandler(mqtt->incomingMsgHdlrRef);
        le_timer_Delete(mqtt->reconnectTimer);
    }

    return ret;
}
