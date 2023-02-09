

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef ADMIN_INTERFACE_H_INCLUDE_GUARD
#define ADMIN_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "io_server.h"

// Internal includes for this interface
#include "admin_common.h"
//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t admin_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t admin_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void admin_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Enumerates the different types of entries that can exist in the resource tree.
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Enumerates the different operations on a Resource - add and remove.
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Enumerates the different types of transforms which can be applied to an observation buffer
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing triggers to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'admin_TriggerPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing Boolean values to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'admin_BooleanPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing numeric values to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'admin_NumericPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing string values to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'admin_StringPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing JSON values to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'admin_JsonPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Register a handler, to be called back whenever a Resource is added or removed
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'admin_ResourceTreeChange'
 */
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
/**
 * Push a trigger type data sample to a resource.
 *
 * @note If the resource doesn't exist, the push will be ignored.  This will not cause a
 *       Placeholder resource to be created.
 */
//--------------------------------------------------------------------------------------------------
void admin_PushTrigger
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute resource tree path.
    double timestamp
        ///< [IN] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
        ///< Zero = now (i.e., generate a timestamp for me).
);



//--------------------------------------------------------------------------------------------------
/**
 * Push a Boolean type data sample to a resource.
 *
 * @note If the resource doesn't exist, the push will be ignored.  This will not cause a
 *       Placeholder resource to be created.
 */
//--------------------------------------------------------------------------------------------------
void admin_PushBoolean
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute resource tree path.
    double timestamp,
        ///< [IN] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
        ///< Zero = now (i.e., generate a timestamp for me).
    bool value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Push a numeric type data sample to a resource.
 *
 * @note If the resource doesn't exist, the push will be ignored.  This will not cause a
 *       Placeholder resource to be created.
 */
//--------------------------------------------------------------------------------------------------
void admin_PushNumeric
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute resource tree path.
    double timestamp,
        ///< [IN] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
        ///< Zero = now (i.e., generate a timestamp for me).
    double value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Push a string type data sample to a resource.
 *
 * @note If the resource doesn't exist, the push will be ignored.  This will not cause a
 *       Placeholder resource to be created.
 */
//--------------------------------------------------------------------------------------------------
void admin_PushString
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute resource tree path.
    double timestamp,
        ///< [IN] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
        ///< Zero = now (i.e., generate a timestamp for me).
    const char* LE_NONNULL value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Push a JSON data sample to a resource.
 *
 * @note If the resource doesn't exist, the push will be ignored.  This will not cause a
 *       Placeholder resource to be created.
 */
//--------------------------------------------------------------------------------------------------
void admin_PushJson
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute resource tree path.
    double timestamp,
        ///< [IN] Timestamp in seconds since the Epoch 1970-01-01 00:00:00 +0000 (UTC).
        ///< Zero = now (i.e., generate a timestamp for me).
    const char* LE_NONNULL value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'admin_TriggerPush'
 */
//--------------------------------------------------------------------------------------------------
admin_TriggerPushHandlerRef_t admin_AddTriggerPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    admin_TriggerPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'admin_TriggerPush'
 */
//--------------------------------------------------------------------------------------------------
void admin_RemoveTriggerPushHandler
(
    admin_TriggerPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'admin_BooleanPush'
 */
//--------------------------------------------------------------------------------------------------
admin_BooleanPushHandlerRef_t admin_AddBooleanPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    admin_BooleanPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'admin_BooleanPush'
 */
//--------------------------------------------------------------------------------------------------
void admin_RemoveBooleanPushHandler
(
    admin_BooleanPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'admin_NumericPush'
 */
//--------------------------------------------------------------------------------------------------
admin_NumericPushHandlerRef_t admin_AddNumericPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    admin_NumericPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'admin_NumericPush'
 */
//--------------------------------------------------------------------------------------------------
void admin_RemoveNumericPushHandler
(
    admin_NumericPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'admin_StringPush'
 */
//--------------------------------------------------------------------------------------------------
admin_StringPushHandlerRef_t admin_AddStringPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    admin_StringPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'admin_StringPush'
 */
//--------------------------------------------------------------------------------------------------
void admin_RemoveStringPushHandler
(
    admin_StringPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'admin_JsonPush'
 */
//--------------------------------------------------------------------------------------------------
admin_JsonPushHandlerRef_t admin_AddJsonPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    admin_JsonPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'admin_JsonPush'
 */
//--------------------------------------------------------------------------------------------------
void admin_RemoveJsonPushHandler
(
    admin_JsonPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Creates a data flow route from one resource to another by setting the data source for the
 * destination resource.  If the destination resource already has a source resource, it will be
 * replaced. Does nothing if the route already exists.
 *
 * Creates Placeholders for any source or destination resource that doesn't yet exist in the
 * resource tree.
 *
 * @note While an Input can have a source configured, it will ignore anything pushed to it
 *       from other resources via that route. Inputs only accept values pushed by the app that
 *       created them or from the administrator pushed directly to them via one of the
 *       @ref c_dataHubAdmin_Pushing "Push functions".
 *
 * @return
 *  - LE_OK if route already existed or new route was successfully created.
 *  - LE_BAD_PARAMETER if one of the paths is invalid.
 *  - LE_DUPLICATE if the addition of this route would result in a loop.
 */
//--------------------------------------------------------------------------------------------------
le_result_t admin_SetSource
(
    const char* LE_NONNULL destPath,
        ///< [IN] Absolute path of destination resource.
    const char* LE_NONNULL srcPath
        ///< [IN] Absolute path of source resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Fetches the data flow source resource from which a given resource expects to receive data
 * samples.
 *
 * @note While an Input can have a source configured, it will ignore anything pushed to it
 *       from other resources via that route. Inputs only accept values pushed by the app that
 *       created them or from the administrator pushed directly to them via one of the
 *       @ref c_dataHubAdmin_Pushing "Push functions".
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_BAD_PARAMETER if the path is invalid.
 *  - LE_NOT_FOUND if the resource doesn't exist or doesn't have a source.
 *  - LE_OVERFLOW if the path of the source resource won't fit in the string buffer provided.
 */
//--------------------------------------------------------------------------------------------------
le_result_t admin_GetSource
(
    const char* LE_NONNULL destPath,
        ///< [IN] Absolute path of destination resource.
    char* srcPath,
        ///< [OUT] Absolute path of source resource.
    size_t srcPathSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove the data flow route into a resource.
 */
//--------------------------------------------------------------------------------------------------
void admin_RemoveSource
(
    const char* LE_NONNULL destPath
        ///< [IN] Absolute path of destination resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Create an Observation in the /obs/ namespace.
 *
 *  @return
 *  - LE_OK if the observation was created or it already existed.
 *  - LE_BAD_PARAMETER if the path is invalid.
 */
//--------------------------------------------------------------------------------------------------
le_result_t admin_CreateObs
(
    const char* LE_NONNULL path
        ///< [IN] Path within the /obs/ namespace.
);



//--------------------------------------------------------------------------------------------------
/**
 * Delete an Observation in the /obs/ namespace.
 */
//--------------------------------------------------------------------------------------------------
void admin_DeleteObs
(
    const char* LE_NONNULL path
        ///< [IN] Path within the /obs/ namespace.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the minimum period between data samples accepted by a given Observation.
 *
 * This is used to throttle the rate of data passing into and through an Observation.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetMinPeriod
(
    const char* LE_NONNULL path,
        ///< [IN] Path within the /obs/ namespace.
    double minPeriod
        ///< [IN] The minimum period, in seconds.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the minimum period between data samples accepted by a given Observation.
 *
 * @return The value, or NAN (not a number) if not set.
 */
//--------------------------------------------------------------------------------------------------
double admin_GetMinPeriod
(
    const char* LE_NONNULL path
        ///< [IN] Path within the /obs/ namespace.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the highest value in a range that will be accepted by a given Observation.
 *
 * Ignored for all non-numeric types except Boolean for which non-zero = true and zero = false.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetHighLimit
(
    const char* LE_NONNULL path,
        ///< [IN] Path within the /obs/ namespace.
    double highLimit
        ///< [IN] The highest value in the range, or NAN (not a number) to remove limit.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the highest value in a range that will be accepted by a given Observation.
 *
 * @return The value, or NAN (not a number) if not set.
 */
//--------------------------------------------------------------------------------------------------
double admin_GetHighLimit
(
    const char* LE_NONNULL path
        ///< [IN] Path within the /obs/ namespace.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the lowest value in a range that will be accepted by a given Observation.
 *
 * Ignored for all non-numeric types except Boolean for which non-zero = true and zero = false.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetLowLimit
(
    const char* LE_NONNULL path,
        ///< [IN] Path within the /obs/ namespace.
    double lowLimit
        ///< [IN] The lowest value in the range, or NAN (not a number) to remove limit.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the lowest value in a range that will be accepted by a given Observation.
 *
 * @return The value, or NAN (not a number) if not set.
 */
//--------------------------------------------------------------------------------------------------
double admin_GetLowLimit
(
    const char* LE_NONNULL path
        ///< [IN] Path within the /obs/ namespace.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the magnitude that a new value must vary from the current value to be accepted by
 * a given Observation.
 *
 * Ignored for trigger types.
 *
 * For all other types, any non-zero value means accept any change, but drop if the same as current.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetChangeBy
(
    const char* LE_NONNULL path,
        ///< [IN] Path within the /obs/ namespace.
    double change
        ///< [IN] The magnitude, or either zero or NAN (not a number) to remove limit.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the magnitude that a new value must vary from the current value to be accepted by
 * a given Observation.
 *
 * @return The value, or NAN (not a number) if not set.
 */
//--------------------------------------------------------------------------------------------------
double admin_GetChangeBy
(
    const char* LE_NONNULL path
        ///< [IN] Path within the /obs/ namespace.
);



//--------------------------------------------------------------------------------------------------
/**
 * Perform a transform on an observation's buffered data. Value of the observation will be
 * the output of the transform
 *
 * Ignored for all non-numeric types except Boolean for which non-zero = true and zero = false.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetTransform
(
    const char* LE_NONNULL path,
        ///< [IN] Path within the /obs/ namespace.
    admin_TransformType_t transformType,
        ///< [IN] Type of transform to apply
    const double* paramsPtr,
        ///< [IN] Optional parameter list
    size_t paramsSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the type of transform currently applied to an Observation.
 *
 * @return The TransformType
 */
//--------------------------------------------------------------------------------------------------
admin_TransformType_t admin_GetTransform
(
    const char* LE_NONNULL path
        ///< [IN] Path within the /obs/ namespace.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the JSON member/element specifier for extraction of data from within a structured JSON
 * value received by a given Observation.
 *
 * If this is set, all non-JSON data will be ignored, and all JSON data that does not contain the
 * the specified object member or array element will also be ignored.
 *
 * To clear, set to an empty string.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetJsonExtraction
(
    const char* LE_NONNULL path,
        ///< [IN] Path within the /obs/ namespace.
    const char* LE_NONNULL extractionSpec
        ///< [IN] str specifying member/element to extract.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the JSON member/element specifier for extraction of data from within a structured JSON
 * value received by a given Observation.
 *
 * @return
 *  - LE_OK if successful
 *  - LE_NOT_FOUND if the resource doesn't exist or doesn't have a JSON extraction specifier set.
 *  - LE_OVERFLOW if the JSON extraction specifier won't fit in the string buffer provided.
 */
//--------------------------------------------------------------------------------------------------
le_result_t admin_GetJsonExtraction
(
    const char* LE_NONNULL path,
        ///< [IN] Path within the /obs/ namespace.
    char* result,
        ///< [OUT] Buffer where result goes if LE_OK returned.
    size_t resultSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the maximum number of data samples to buffer in a given Observation.  Buffers are FIFO
 * circular buffers. When full, the buffer drops the oldest value to make room for a new addition.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetBufferMaxCount
(
    const char* LE_NONNULL path,
        ///< [IN] Path within the /obs/ namespace.
    uint32_t count
        ///< [IN] The number of samples to buffer (0 = remove setting).
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the buffer size setting for a given Observation.
 *
 * @return The buffer size (in number of samples) or 0 if not set or the Observation does not exist.
 */
//--------------------------------------------------------------------------------------------------
uint32_t admin_GetBufferMaxCount
(
    const char* LE_NONNULL path
        ///< [IN] Path within the /obs/ namespace.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the minimum time between backups of an Observation's buffer to non-volatile storage.
 * If the buffer's size is non-zero and the backup period is non-zero, then the buffer will be
 * backed-up to non-volatile storage when it changes, but never more often than this period setting
 * specifies.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetBufferBackupPeriod
(
    const char* LE_NONNULL path,
        ///< [IN] Path within the /obs/ namespace.
    uint32_t seconds
        ///< [IN] The minimum number of seconds between backups (0 = disable backups)
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the minimum time between backups of an Observation's buffer to non-volatile storage.
 * See admin_SetBufferBackupPeriod() for more information.
 *
 * @return The buffer backup period (in seconds) or 0 if backups are disabled or the Observation
 *         does not exist.
 */
//--------------------------------------------------------------------------------------------------
uint32_t admin_GetBufferBackupPeriod
(
    const char* LE_NONNULL path
        ///< [IN] Path within the /obs/ namespace.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the default value of a resource to a Boolean value.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetBooleanDefault
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    bool value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the default value of a resource to a numeric value.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetNumericDefault
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    double value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the default value of a resource to a string value.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetStringDefault
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    const char* LE_NONNULL value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the default value of a resource to a JSON value.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetJsonDefault
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    const char* LE_NONNULL value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Discover whether a given resource has a default value.
 *
 * @return true if there is a default value set, false if not.
 */
//--------------------------------------------------------------------------------------------------
bool admin_HasDefault
(
    const char* LE_NONNULL path
        ///< [IN] Absolute path of the resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the data type of the default value that is currently set on a given resource.
 *
 * @return The data type, or IO_DATA_TYPE_TRIGGER if not set.
 */
//--------------------------------------------------------------------------------------------------
io_DataType_t admin_GetDefaultDataType
(
    const char* LE_NONNULL path
        ///< [IN] Absolute path of the resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the default value of a resource, if it is Boolean.
 *
 * @return the default value, or false if not set or set to another data type.
 */
//--------------------------------------------------------------------------------------------------
bool admin_GetBooleanDefault
(
    const char* LE_NONNULL path
        ///< [IN] Absolute path of the resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the default value, if it is numeric.
 *
 * @return the default value, or NAN (not a number) if not set or set to another data type.
 */
//--------------------------------------------------------------------------------------------------
double admin_GetNumericDefault
(
    const char* LE_NONNULL path
        ///< [IN] Absolute path of the resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the default value, if it is a string.
 *
 * @return
 *  - LE_OK if successful,
 *  - LE_OVERFLOW if the buffer provided is too small to hold the value.
 *  - LE_NOT_FOUND if the resource doesn't have a string default value set.
 */
//--------------------------------------------------------------------------------------------------
le_result_t admin_GetStringDefault
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    char* value,
        ///< [OUT]
    size_t valueSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the default value, in JSON format.
 *
 * @note This works for any type of default value.
 *
 * @return
 *  - LE_OK if successful,
 *  - LE_OVERFLOW if the buffer provided is too small to hold the value.
 *  - LE_NOT_FOUND if the resource doesn't have a default value set.
 */
//--------------------------------------------------------------------------------------------------
le_result_t admin_GetJsonDefault
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    char* value,
        ///< [OUT]
    size_t valueSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove any default value on a given resource.
 */
//--------------------------------------------------------------------------------------------------
void admin_RemoveDefault
(
    const char* LE_NONNULL path
        ///< [IN] Absolute path of the resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Set an override of Boolean type on a given resource.
 *
 * @note Override will be ignored by an Input or Output resource if the override's data type
 *       does not match the data type of the Input or Output.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetBooleanOverride
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    bool value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set an override of numeric type on a given resource.
 *
 * @note Override will be ignored by an Input or Output resource if the override's data type
 *       does not match the data type of the Input or Output.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetNumericOverride
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    double value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set an override of string type on a given resource.
 *
 * @note Override will be ignored by an Input or Output resource if the override's data type
 *       does not match the data type of the Input or Output.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetStringOverride
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    const char* LE_NONNULL value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Set an override of JSON type on a given resource.
 *
 * @note Override will be ignored by an Input or Output resource if the override's data type
 *       does not match the data type of the Input or Output.
 */
//--------------------------------------------------------------------------------------------------
void admin_SetJsonOverride
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    const char* LE_NONNULL value
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Find out whether the resource currently has an override set.
 *
 * @return true if the resource has an override, false otherwise.
 *
 * @note It's possible for an Input or Output to have an override set, but not be overridden.
 *       This is because setting an override to a data type that does not match the Input or
 *       Output resource's data type will result in the override being ignored.  Observations
 *       (and Placeholders) have flexible data types, so if they have an override set, they will
 *       definitely be overridden.
 */
//--------------------------------------------------------------------------------------------------
bool admin_HasOverride
(
    const char* LE_NONNULL path
        ///< [IN] Absolute path of the resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the data type of the override value that is currently set on a given resource.
 *
 * @return The data type, or IO_DATA_TYPE_TRIGGER if not set.
 */
//--------------------------------------------------------------------------------------------------
io_DataType_t admin_GetOverrideDataType
(
    const char* LE_NONNULL path
        ///< [IN] Absolute path of the resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the override value of a resource, if it is Boolean.
 *
 * @return the override value, or false if not set or set to another data type.
 */
//--------------------------------------------------------------------------------------------------
bool admin_GetBooleanOverride
(
    const char* LE_NONNULL path
        ///< [IN] Absolute path of the resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the override value, if it is numeric.
 *
 * @return the override value, or NAN (not a number) if not set or set to another data type.
 */
//--------------------------------------------------------------------------------------------------
double admin_GetNumericOverride
(
    const char* LE_NONNULL path
        ///< [IN] Absolute path of the resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the override value, if it is a string.
 *
 * @return
 *  - LE_OK if successful,
 *  - LE_OVERFLOW if the buffer provided is too small to hold the value.
 *  - LE_NOT_FOUND if the resource doesn't have a string override value set.
 */
//--------------------------------------------------------------------------------------------------
le_result_t admin_GetStringOverride
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    char* value,
        ///< [OUT]
    size_t valueSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the override value, in JSON format.
 *
 * @note This works for any type of override value.
 *
 * @return
 *  - LE_OK if successful,
 *  - LE_OVERFLOW if the buffer provided is too small to hold the value.
 *  - LE_NOT_FOUND if the resource doesn't have an override value set.
 */
//--------------------------------------------------------------------------------------------------
le_result_t admin_GetJsonOverride
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    char* value,
        ///< [OUT]
    size_t valueSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove any override on a given resource.
 */
//--------------------------------------------------------------------------------------------------
void admin_RemoveOverride
(
    const char* LE_NONNULL path
        ///< [IN] Absolute path of the resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the name of the first child entry under a given parent entry in the resource tree.
 *
 * @return
 *  - LE_OK if successful
 *  - LE_OVERFLOW if the buffer provided is too small to hold the child's path.
 *  - LE_NOT_FOUND if the resource doesn't have any children.
 */
//--------------------------------------------------------------------------------------------------
le_result_t admin_GetFirstChild
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    char* child,
        ///< [OUT] Absolute path of the first child resource.
    size_t childSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the name of the next child entry under the same parent as a given entry in the resource tree.
 *
 * @return
 *  - LE_OK if successful
 *  - LE_OVERFLOW if the buffer provided is too small to hold the next sibling's path.
 *  - LE_NOT_FOUND if the resource is the last child in its parent's list of children.
 */
//--------------------------------------------------------------------------------------------------
le_result_t admin_GetNextSibling
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of the resource.
    char* sibling,
        ///< [OUT] Absolute path of the next sibling resource.
    size_t siblingSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Find out what type of entry lives at a given path in the resource tree.
 *
 * @return The entry type. ADMIN_ENTRY_TYPE_NONE if there's no entry at the given path.
 */
//--------------------------------------------------------------------------------------------------
admin_EntryType_t admin_GetEntryType
(
    const char* LE_NONNULL path
        ///< [IN] Absolute path of the resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Check if a given resource is a mandatory output.  If so, it means that this is an output resource
 * that must have a value before the related app function will begin working.
 *
 * @return true if a mandatory output, false if it's an optional output or not an output at all.
 */
//--------------------------------------------------------------------------------------------------
bool admin_IsMandatory
(
    const char* LE_NONNULL path
        ///< [IN] Absolute path of the resource.
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'admin_ResourceTreeChange'
 */
//--------------------------------------------------------------------------------------------------
admin_ResourceTreeChangeHandlerRef_t admin_AddResourceTreeChangeHandler
(
    admin_ResourceTreeChangeHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'admin_ResourceTreeChange'
 */
//--------------------------------------------------------------------------------------------------
void admin_RemoveResourceTreeChangeHandler
(
    admin_ResourceTreeChangeHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Signal to the Data Hub that administrative changes are about to be performed.
 *
 * This will result in call-backs to any handlers registered using io_AddUpdateStartEndHandler().
 */
//--------------------------------------------------------------------------------------------------
void admin_StartUpdate
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Signal to the Data Hub that all pending administrative changes have been applied and that
 * normal operation may resume.
 *
 * This may trigger clean-up actions, such as deleting non-volatile backups of any Observations
 * that do not exist at the time this function is called.
 *
 * This will also result in call-backs to any handlers registered using
 * io_AddUpdateStartEndHandler().
 */
//--------------------------------------------------------------------------------------------------
void admin_EndUpdate
(
    void
);


#endif // ADMIN_INTERFACE_H_INCLUDE_GUARD
