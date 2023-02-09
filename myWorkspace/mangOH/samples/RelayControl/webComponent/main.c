/**
 * @file
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless, Inc. Use of this work is subject to license.
 */


#include <legato.h>
#include "interfaces.h"

#include "mongoose/mongoose.h"

#define MAX_RELAY_NB    16

static const char HttpPort[] = "8080";

static void SendInternalError(struct mg_connection *connPtr)
{
    mg_send_head(connPtr, 500, 0, NULL);
}

//--------------------------------------------------------------------------------------------------
/**
 * Print an array of JSON structures reflecting the states of all relays.
 */
//--------------------------------------------------------------------------------------------------
static void ListRelays(struct mg_connection *connPtr)
{
    uint8_t nbRelays = 0;
    le_result_t res;
    char buf[(MAX_RELAY_NB*25) + 10];
    size_t bufPos = 0;

    res = relayControl_Count(&nbRelays);
    if (res != LE_OK)
    {
        LE_ERROR("Unable to get the number of relays");
        goto error;
    }

    int id;
    const char *prependStr = "[";
    bool state;
    int wrote;

    for (id = 0; id < nbRelays; id++)
    {
        if (relayControl_GetState(id, &state) != LE_OK)
        {
            LE_ERROR("Unable to get state of relay %d", id);
            goto error;
        }

        wrote = snprintf(&buf[bufPos],
                         sizeof(buf)-bufPos,
                         "%s{\"state\": %d, \"id\": \"%d\"}",
                         prependStr, (int)state, id);

        if ( (wrote < 0) || (bufPos+wrote) >= sizeof(buf) )
        {
            LE_ERROR("Buffer too small");
            goto error;
        }

        bufPos += wrote;
        prependStr = ", ";
    }

    wrote = snprintf(&buf[bufPos],
                     sizeof(buf)-bufPos, "]");

    if ( (wrote < 0) || (bufPos+wrote) >= sizeof(buf) )
    {
        LE_ERROR("Buffer too small");
        goto error;
    }

    mg_printf(connPtr, "HTTP/1.0 200 OK\r\nContent-Length: %d\r\n"
                       "Content-Type: application/json\r\n\r\n%s",
                       (int) strlen(buf), buf);

error:
    SendInternalError(connPtr);
}

//--------------------------------------------------------------------------------------------------
/**
 * Print a JSON structure to reflect the state of a relay.
 */
//--------------------------------------------------------------------------------------------------
static void PrintRelay(struct mg_connection *connPtr, int id)
{
    bool state;
    char buf[50];

    if (relayControl_GetState(id, &state) != LE_OK)
    {
        LE_ERROR("Unable to get state of relay %d", id);
        goto error;
    }

    sprintf(buf, "{\"state\": %d, \"id\": \"%d\"}", (int)state, id);

    mg_printf(connPtr, "HTTP/1.0 200 OK\r\nContent-Length: %d\r\n"
                       "Content-Type: application/json\r\n\r\n%s",
                       (int) strlen(buf), buf);

error:
    SendInternalError(connPtr);
}

//--------------------------------------------------------------------------------------------------
/**
 * Handle case where relay state was sent as a form.
 */
//--------------------------------------------------------------------------------------------------
static le_result_t SetRelayForm(const char *bufStatePtr, int id)
{
    bool state = false;

    if (bufStatePtr[1] != '\0')
    {
        return LE_BAD_PARAMETER;
    }

    switch(bufStatePtr[0])
    {
        case '1':
            state = true;
            break;

        case '0':
            state = false;
            break;

        default:
            return LE_BAD_PARAMETER;
    }

    return relayControl_SetState(id, state);
}

//--------------------------------------------------------------------------------------------------
/**
 * Handle case where relay state was sent as a JSON structure.
 */
//--------------------------------------------------------------------------------------------------
static le_result_t SetRelayJson(const struct mg_str *bodyPtr, int id)
{
    bool state = false;
    struct json_token tokens[200], *statePtr;
    int n;

    n = parse_json(bodyPtr->p, bodyPtr->len, tokens, sizeof(tokens) / sizeof(tokens[0]));
    if (n < 0)
    {
        return LE_BAD_PARAMETER;
    }

    statePtr = find_json_token(tokens, "state");
    if (statePtr == NULL)
    {
        return LE_BAD_PARAMETER;
    }

    if (0 == strncmp("1", statePtr->ptr, statePtr->len))
    {
        state = true;
    }
    else if (0 == strncmp("0", statePtr->ptr, statePtr->len))
    {
        state = false;
    }
    else
    {
        return LE_BAD_PARAMETER;
    }

    LE_DEBUG("Set relay state (JSON): %d", state);

    return relayControl_SetState(id, state);
}

//--------------------------------------------------------------------------------------------------
/**
 * Given parameters from the HTTP client, set the relay state.
 */
//--------------------------------------------------------------------------------------------------
static le_result_t SetRelay(struct http_message *messagePtr, int id)
{
    const struct mg_str *bodyPtr = messagePtr->query_string.len > 0 ? &messagePtr->query_string : &messagePtr->body;
    char bufState[10] = {0};

    // JSON ?
    int i;

    for (i = 0; i < MG_MAX_HTTP_HEADERS && messagePtr->header_names[i].len > 0; i++) {
        struct mg_str headerNameStr = messagePtr->header_names[i];
        struct mg_str headerValueStr = messagePtr->header_values[i];

        if (mg_vcasecmp(&headerNameStr, "Content-Type") != 0)
        {
            continue;
        }

        if (mg_vcasecmp(&headerValueStr, "application/json") != 0)
        {
            break;
        }

        return SetRelayJson(&messagePtr->body, id);
    }

    // Form ?
    if (0 != mg_get_http_var(bodyPtr, "state", bufState, sizeof(bufState)))
    {
        return SetRelayForm(bufState, id);
    }

    LE_ERROR("Invalid set relay request");

    return LE_BAD_PARAMETER;
}

//--------------------------------------------------------------------------------------------------
/**
 * Event handler for the web server.
 */
//--------------------------------------------------------------------------------------------------
static void EventHandler(struct mg_connection *connPtr, int ev, void *p)
{
    if (ev != MG_EV_HTTP_REQUEST)
    {
        return;
    }

    struct http_message *messagePtr = (struct http_message *)p;
    le_result_t res = LE_NOT_POSSIBLE;

    LE_INFO("HTTP Request %s", messagePtr->uri.p);

    if (messagePtr->uri.len < strlen("/relays"))
    {
        res = LE_NOT_FOUND;
    }
    else if (memcmp(messagePtr->uri.p, "/relays", strlen("/relays")) != 0)
    {
        res = LE_NOT_FOUND;
    }
    else
    {
        const char *uriPtr = messagePtr->uri.p + strlen("/relays");

        LE_INFO("HTTP SubRequest %s", uriPtr);

        if (*uriPtr == ' ')
        {
            ListRelays(connPtr);
            res = LE_OK;
        }
        else if(*uriPtr == '/')
        {
            int id;

            uriPtr += 1;

            id = atoi(uriPtr);
            LE_INFO("Relay %d", id);

            res = LE_OK;

            if (mg_vcasecmp(&messagePtr->method, "POST") == 0)
            {
                res = SetRelay(messagePtr, id);
            }

            if (res == LE_OK)
            {
                PrintRelay(connPtr, id);
            }
        }
    }

    if (res == LE_NOT_FOUND)
    {
        LE_ERROR("URI not found");
        mg_send_head(connPtr, 404, 0, NULL);
        return;
    }

    if (res != LE_OK)
    {
        LE_ERROR("Internal error");
        mg_send_head(connPtr, 500, 0, NULL);
    }
}

static void Serve(const char *portPtr)
{
    struct mg_mgr manager;
    struct mg_connection *connPtr;

    LE_INFO("Init server on port %s", portPtr);

    mg_mgr_init(&manager, NULL);
    connPtr = mg_bind(&manager, portPtr, EventHandler);
    mg_set_protocol_http_websocket(connPtr);

    /* For each new connection, execute ev_handler in a separate thread */
    //mg_enable_multithreading(connPtr);

    LE_INFO("Starting server on port %s", portPtr);

    for (;;)
    {
        mg_mgr_poll(&manager, 3000);
    }
    mg_mgr_free(&manager);
}

//--------------------------------------------------------------------------------------------------
/**
 * Initialization function for the webServer
 */
//--------------------------------------------------------------------------------------------------
COMPONENT_INIT
{
    Serve(HttpPort);
    LE_FATAL("webServer returned");
}
