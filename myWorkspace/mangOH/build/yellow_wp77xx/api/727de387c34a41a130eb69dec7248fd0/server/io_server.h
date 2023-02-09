

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef IO_INTERFACE_H_INCLUDE_GUARD
#define IO_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Internal includes for this interface
#include "io_common.h"
//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t io_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t io_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void io_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Enumerates the data types supported.
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing triggers to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'io_TriggerPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing Boolean values to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'io_BooleanPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing numeric values to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'io_NumericPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing string values to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'io_StringPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing JSON values to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'io_JsonPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for notification that a Data Hub reconfiguration is beginning or ending.
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'io_UpdateStartEnd'
 */
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
/**
 * Create an input resource, which is used to push data into the Data Hub.
 *
 * Does nothing if the resource already exists.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_DUPLICATE if a resource by that name exists but with different direction, type or units.
 *  - LE_NO_MEMORY if the client is not permitted to create that many resources.
 *  - LE_BAD_PARAMETER if the path is malformed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t io_CreateInput
(
    const char* LE_NONNULL path,
        ///< [IN] Path within the client app's resource namespace.
    io_DataType_t dataType,
        ///< [IN] The data type.
    const char* LE_NONNULL units
        ///< [IN] e.g., "degC" (see senml); "" = unspecified.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the example value for a JSON-type Input resource.
 *
 * Does nothing if the resource is not found, is not an input, or doesn't have a JSON type.
 */
//--------------------------------------------------------------------------------------------------
void io_SetJsonExample
(
    const char* LE_NONNULL path,
        ///< [IN] Path within the client app's resource namespace.
    const char* LE_NONNULL example
        ///< [IN] The example JSON value string.
);



//--------------------------------------------------------------------------------------------------
/**
 * Create an output resource, which is used to receive data output from the Data Hub.
 *
 * Does nothing if the resource already exists.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_DUPLICATE if a resource by that name exists but with different direction, type or units.
 *  - LE_NO_MEMORY if the client is not permitted to create that many resources.
 *  - LE_BAD_PARAMETER if the path is malformed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t io_CreateOutput
(
    const char* LE_NONNULL path,
        ///< [IN] Path within the client app's resource namespace.
    io_DataType_t dataType,
        ///< [IN] The data type.
    const char* LE_NONNULL units
        ///< [IN] e.g., "degC" (see senml); "" = unspecified.
);



//--------------------------------------------------------------------------------------------------
/**
 * Delete a resource.
 *
 * Does nothing if the resource doesn't exist.
 */
//--------------------------------------------------------------------------------------------------
void io_DeleteResource
(
    const char* LE_NONNULL path
        ///< [IN] Resource path within the client app's namespace.
);



//--------------------------------------------------------------------------------------------------
/**
 * Push a trigger type data sample.
 */
//--------------------------------------------------------------------------------------------------
void io_PushTrigger
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    double timestamp
        ///< [IN] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
        ///< IO_NOW = now (i.e., generate a timestamp for me).
);



//--------------------------------------------------------------------------------------------------
/**
 * Push a Boolean type data sample.
 */
//--------------------------------------------------------------------------------------------------
void io_PushBoolean
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    double timestamp,
        ///< [IN] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
        ///< IO_NOW = now (i.e., generate a timestamp for me).
    bool value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Push a numeric type data sample.
 */
//--------------------------------------------------------------------------------------------------
void io_PushNumeric
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    double timestamp,
        ///< [IN] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
        ///< IO_NOW = now (i.e., generate a timestamp for me).
    double value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Push a string type data sample.
 */
//--------------------------------------------------------------------------------------------------
void io_PushString
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    double timestamp,
        ///< [IN] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
        ///< IO_NOW = now (i.e., generate a timestamp for me).
    const char* LE_NONNULL value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Push a JSON data sample.
 */
//--------------------------------------------------------------------------------------------------
void io_PushJson
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    double timestamp,
        ///< [IN] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
        ///< IO_NOW = now (i.e., generate a timestamp for me).
    const char* LE_NONNULL value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'io_TriggerPush'
 */
//--------------------------------------------------------------------------------------------------
io_TriggerPushHandlerRef_t io_AddTriggerPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    io_TriggerPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'io_TriggerPush'
 */
//--------------------------------------------------------------------------------------------------
void io_RemoveTriggerPushHandler
(
    io_TriggerPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'io_BooleanPush'
 */
//--------------------------------------------------------------------------------------------------
io_BooleanPushHandlerRef_t io_AddBooleanPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    io_BooleanPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'io_BooleanPush'
 */
//--------------------------------------------------------------------------------------------------
void io_RemoveBooleanPushHandler
(
    io_BooleanPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'io_NumericPush'
 */
//--------------------------------------------------------------------------------------------------
io_NumericPushHandlerRef_t io_AddNumericPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    io_NumericPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'io_NumericPush'
 */
//--------------------------------------------------------------------------------------------------
void io_RemoveNumericPushHandler
(
    io_NumericPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'io_StringPush'
 */
//--------------------------------------------------------------------------------------------------
io_StringPushHandlerRef_t io_AddStringPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    io_StringPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'io_StringPush'
 */
//--------------------------------------------------------------------------------------------------
void io_RemoveStringPushHandler
(
    io_StringPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'io_JsonPush'
 */
//--------------------------------------------------------------------------------------------------
io_JsonPushHandlerRef_t io_AddJsonPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    io_JsonPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'io_JsonPush'
 */
//--------------------------------------------------------------------------------------------------
void io_RemoveJsonPushHandler
(
    io_JsonPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Mark an Output resource "optional".  (By default, they are marked "mandatory".)
 */
//--------------------------------------------------------------------------------------------------
void io_MarkOptional
(
    const char* LE_NONNULL path
        ///< [IN] Resource path within the client app's namespace.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set a Boolean type value as the default value of a given resource.
 *
 * @note This will be ignored if the resource already has a default value.
 */
//--------------------------------------------------------------------------------------------------
void io_SetBooleanDefault
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    bool value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set a numeric type value as the default value of a given resource.
 *
 * @note This will be ignored if the resource already has a default value.
 */
//--------------------------------------------------------------------------------------------------
void io_SetNumericDefault
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    double value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set a string type value as the default value of a given resource.
 *
 * @note This will be ignored if the resource already has a default value.
 */
//--------------------------------------------------------------------------------------------------
void io_SetStringDefault
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    const char* LE_NONNULL value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set a JSON type value as the default value of a given resource.
 *
 * @note This will be ignored if the resource already has a default value.
 */
//--------------------------------------------------------------------------------------------------
void io_SetJsonDefault
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    const char* LE_NONNULL value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Fetch the timestamp of the current value of an Input or Output resource with any data type.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource does not exist.
 *  - LE_UNAVAILABLE if the resource does not currently have a value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t io_GetTimestamp
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    double* timestampPtr
        ///< [OUT] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
);



//--------------------------------------------------------------------------------------------------
/**
 * Fetch the current value of a Boolean type Input or Output resource.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource does not exist.
 *  - LE_UNAVAILABLE if the resource does not currently have a value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t io_GetBoolean
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    double* timestampPtr,
        ///< [OUT] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
    bool* valuePtr
        ///< [OUT]
);



//--------------------------------------------------------------------------------------------------
/**
 * Fetch the current value of a numeric type Input or Output resource.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource does not exist.
 *  - LE_UNAVAILABLE if the resource does not currently have a value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t io_GetNumeric
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    double* timestampPtr,
        ///< [OUT] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
    double* valuePtr
        ///< [OUT]
);



//--------------------------------------------------------------------------------------------------
/**
 * Fetch the current value of a string type Input or Output resource.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_OVERFLOW if the value buffer was too small to hold the value.
 *  - LE_NOT_FOUND if the resource does not exist.
 *  - LE_UNAVAILABLE if the resource does not currently have a value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t io_GetString
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    double* timestampPtr,
        ///< [OUT] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
    char* value,
        ///< [OUT]
    size_t valueSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Fetch the current value of an Input or Output resource (of any data type) in JSON format.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_OVERFLOW if the value buffer was too small to hold the value.
 *  - LE_NOT_FOUND if the resource does not exist.
 *  - LE_UNAVAILABLE if the resource does not currently have a value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t io_GetJson
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path within the client app's namespace.
    double* timestampPtr,
        ///< [OUT] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
    char* value,
        ///< [OUT]
    size_t valueSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'io_UpdateStartEnd'
 */
//--------------------------------------------------------------------------------------------------
io_UpdateStartEndHandlerRef_t io_AddUpdateStartEndHandler
(
    io_UpdateStartEndHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'io_UpdateStartEnd'
 */
//--------------------------------------------------------------------------------------------------
void io_RemoveUpdateStartEndHandler
(
    io_UpdateStartEndHandlerRef_t handlerRef
        ///< [IN]
);


#endif // IO_INTERFACE_H_INCLUDE_GUARD
