

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_COAP_INTERFACE_H_INCLUDE_GUARD
#define LE_COAP_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Internal includes for this interface
#include "le_coap_common.h"
//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_coap_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_coap_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_coap_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * CoAP method and response codes as defined in RFC 7252
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * CoAP stream status (write request block-2 & read response block-1).
 *
 * Note: This enum is used for receiving CoAP messages and also starting a transmit stream from
 * the device. TX_STREAM_START/ TX_STREAM_END / TX_STREAM_CANCEL will never be received on incoming
 * CoAP messages but can be used to indicate status of outgoing stream using
 * le_coap_SendResponse() api.
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Push status
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Handler for receiving incoming CoAP messages
 *
 * @note
 *     Refer to CoAP Message Format https://tools.ietf.org/html/rfc7252#section-2.1 to know how to
 *     use Message ID and Token in a CoAP message.
 *
 *     Content type of payload is dependent on the server implementation. CoAP content formats
 *     Registry https://tools.ietf.org/html/rfc7252#section-12.3 describes how a content format is
 *     assigned. The CoAP Content Formats sub-registry is at
 *     https://www.iana.org/assignments/core-parameters/core-parameters.xhtml#content-formats.
 *     A server might support a different format other than the ones described in the sub registry.
 *
 * @warning
 *     Application needs to call le_coap_SendResponse API in order to acknowledge the incoming
 *     CoAP message in a 2-sec time window.
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_coap_MessageEvent'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Handler for receiving response to push operation.
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_coap_PushEvent'
 */
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
/**
 * Sends asynchronous CoAP response to server.
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if failed.
 *
 * Note: This API will return success if it successful in sending the message down the stack.
 * Retransmission will be handled at CoAP layer.
 *
 * Note: @c blockSize parameter needs to be a value equal to 16, 32, 64, 128, 256, 512 or 1024.
 * It allows the application to indicate to the server that the device wants to negotiate another
 * block size value. The @c blockSize value should be equal or smaller than @c blockSize provided
 * in @c le_coap_MessageHandlerFunc_t handler.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_coap_SendResponse
(
    uint16_t messageId,
        ///< [IN] Message id in CoAP message header
    const uint8_t* tokenPtr,
        ///< [IN] Token in CoAP message header
    size_t tokenSize,
        ///< [IN]
    uint16_t contentType,
        ///< [IN] Content type of the payload
    le_coap_Code_t responseCode,
        ///< [IN] Result of CoAP operation
    le_coap_StreamStatus_t streamStatus,
        ///< [IN] Status of transmit stream
    const uint8_t* payloadPtr,
        ///< [IN] CoAP payload
    size_t payloadSize,
        ///< [IN]
    uint16_t blockSize
        ///< [IN] Block size
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_coap_MessageEvent'
 *
 * This event sends the incoming CoAP message that are not handled by LwM2M to application.
 *
 * Note: It the content-type of incoming message is not recognizable at CoAP layer, those messages
 * are sent to application for processing.
 */
//--------------------------------------------------------------------------------------------------
le_coap_MessageEventHandlerRef_t le_coap_AddMessageEventHandler
(
    le_coap_MessageHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_coap_MessageEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_coap_RemoveMessageEventHandler
(
    le_coap_MessageEventHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_coap_PushEvent'
 *
 * This event delivers the response to Push operation.
 *
 */
//--------------------------------------------------------------------------------------------------
le_coap_PushEventHandlerRef_t le_coap_AddPushEventHandler
(
    le_coap_PushHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_coap_PushEvent'
 */
//--------------------------------------------------------------------------------------------------
void le_coap_RemovePushEventHandler
(
    le_coap_PushEventHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * This function sends unsolicited CoAP push messages to the server. Responses to push will be
 * received by message handler function.
 *
 * Note: The token can be generated by AirVantage Connector. In this case, @c tokenPtr parameter
 * should be set to @c NULL and @c tokenLength parameter should be set to @c 0.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_coap_Push
(
    const char* LE_NONNULL uri,
        ///< [IN] URI where push should end
    const uint8_t* tokenPtr,
        ///< [IN] Token in CoAP message header
    size_t tokenSize,
        ///< [IN]
    uint16_t contentType,
        ///< [IN] Content type of the payload
    le_coap_StreamStatus_t streamStatus,
        ///< [IN] Status of transmit stream
    const uint8_t* payloadPtr,
        ///< [IN] CoAP Payload
    size_t payloadSize
        ///< [IN]
);


#endif // LE_COAP_INTERFACE_H_INCLUDE_GUARD
