

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef QUERY_INTERFACE_H_INCLUDE_GUARD
#define QUERY_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "io_server.h"

// Internal includes for this interface
#include "query_common.h"
//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t query_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t query_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void query_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Completion callbacks for query_ReadBufferJson() must look like this.
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing triggers to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'query_TriggerPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing Boolean values to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'query_BooleanPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing numeric values to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'query_NumericPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing string values to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'query_StringPush'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Callback function for pushing JSON values to an output
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'query_JsonPush'
 */
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
/**
 * Read data out of a buffer.  Data is written to a given file descriptor in JSON-encoded format
 * as an array of objects containing a timestamp and a value (or just a timestamp for triggers).
 * E.g.,
 *
 * @code
 * [{"t":1537483647.125,"v":true},{"t":1537483657.128,"v":true}]
 * @endcode
 *
 * @return
 *  - LE_OK if the read operation started successfully.
 *  - LE_NOT_FOUND if the Observation doesn't exist.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_ReadBufferJson
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startAfter,
        ///< [IN] Start after this many seconds ago,
        ///< or after an absolute number of seconds since the Epoch
        ///< (if startafter > 30 years).
        ///< Use NAN (not a number) to read the whole buffer.
    int outputFile,
        ///< [IN] File descriptor to write the data to.
    query_ReadCompletionFunc_t completionFuncPtr,
        ///< [IN] Completion callback to be called when operation finishes.
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Read the timestamp of a single sample from a buffer.
 *
 * @note This can be used with any type of sample.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the Observation doesn't exist or does not have a sample newer than the given
 *                 startAfter timestamp.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_ReadBufferSampleTimestamp
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startAfter,
        ///< [IN] Start after this many seconds ago,
        ///< or after an absolute number of seconds since the Epoch
        ///< (if startafter > 30 years).
        ///< Use NAN (not a number) to read the oldest sample in the buffer.
    double* timestampPtr
        ///< [OUT] Timestamp of the sample, if LE_OK returned.
);



//--------------------------------------------------------------------------------------------------
/**
 * Read a single Boolean sample from a buffer.
 *
 * @warning This can only be used with Boolean type samples.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the Observation doesn't exist or does not have a Boolean sample newer than
 *                 the given startAfter timestamp.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_ReadBufferSampleBoolean
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startAfter,
        ///< [IN] Start after this many seconds ago,
        ///< or after an absolute number of seconds since the Epoch
        ///< (if startafter > 30 years).
        ///< Use NAN (not a number) to read the oldest sample in the buffer.
    double* timestampPtr,
        ///< [OUT] Timestamp of the sample, if LE_OK returned.
    bool* valuePtr
        ///< [OUT] Value of the sample, if LE_OK returned.
);



//--------------------------------------------------------------------------------------------------
/**
 * Read a single numeric sample from a buffer.
 *
 * @warning This can only be used with numeric type samples.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the Observation doesn't exist or does not have a Numeric sample newer than
 *                 the given startAfter timestamp.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_ReadBufferSampleNumeric
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startAfter,
        ///< [IN] Start after this many seconds ago,
        ///< or after an absolute number of seconds since the Epoch
        ///< (if startafter > 30 years).
        ///< Use NAN (not a number) to read the oldest sample in the buffer.
    double* timestampPtr,
        ///< [OUT] Timestamp of the sample, if LE_OK returned.
    double* valuePtr
        ///< [OUT] Value of the sample, if LE_OK returned.
);



//--------------------------------------------------------------------------------------------------
/**
 * Read a single sample from a buffer as a string.
 *
 * @note This can be used with any type of sample.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the Observation doesn't exist or does not have a sample newer than the given
 *                 startAfter timestamp.
 *  - LE_OVERFLOW if the buffer provided is too small to hold the value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_ReadBufferSampleString
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startAfter,
        ///< [IN] Start after this many seconds ago,
        ///< or after an absolute number of seconds since the Epoch
        ///< (if startafter > 30 years).
        ///< Use NAN (not a number) to read the oldest sample in the buffer.
    double* timestampPtr,
        ///< [OUT] Timestamp of the sample, if LE_OK returned.
    char* value,
        ///< [OUT] Value of the sample, if LE_OK returned.
    size_t valueSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Read a single sample from a buffer as JSON.
 *
 * @note This can be used with any type of sample.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the Observation doesn't exist or does not have a sample newer than the given
 *                 startAfter timestamp.
 *  - LE_OVERFLOW if the buffer provided is too small to hold the value.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_ReadBufferSampleJson
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startAfter,
        ///< [IN] Start after this many seconds ago,
        ///< or after an absolute number of seconds since the Epoch
        ///< (if startafter > 30 years).
        ///< Use NAN (not a number) to read the oldest sample in the buffer.
    double* timestampPtr,
        ///< [OUT] Timestamp of the sample, if LE_OK returned.
    char* value,
        ///< [OUT] Value of the sample, if LE_OK returned.
    size_t valueSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the minimum value found in an Observation's data set within a given time span.
 *
 * @return The value, or NAN (not-a-number) if there's no numerical data in the Observation's
 *         buffer (if the buffer size is zero, the buffer is empty, or the buffer contains data
 *         of a non-numerical type).
 */
//--------------------------------------------------------------------------------------------------
double query_GetMin
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startTime
        ///< [IN] If < 30 years then seconds before now; else seconds since the Epoch.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the maximum value found within a given time span in an Observation's buffer.
 *
 * @return The value, or NAN (not-a-number) if there's no numerical data in the Observation's
 *         buffer (if the buffer size is zero, the buffer is empty, or the buffer contains data
 *         of a non-numerical type).
 */
//--------------------------------------------------------------------------------------------------
double query_GetMax
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startTime
        ///< [IN] If < 30 years then seconds before now; else seconds since the Epoch.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the mean (average) of all values found within a given time span in an Observation's buffer.
 *
 * @return The value, or NAN (not-a-number) if there's no numerical data in the Observation's
 *         buffer (if the buffer size is zero, the buffer is empty, or the buffer contains data
 *         of a non-numerical type).
 */
//--------------------------------------------------------------------------------------------------
double query_GetMean
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startTime
        ///< [IN] If < 30 years then seconds before now; else seconds since the Epoch.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the standard deviation of all values found within a given time span in an
 * Observation's buffer.
 *
 * @return The value, or NAN (not-a-number) if there's no numerical data in the Observation's
 *         buffer (if the buffer size is zero, the buffer is empty, or the buffer contains data
 *         of a non-numerical type).
 */
//--------------------------------------------------------------------------------------------------
double query_GetStdDev
(
    const char* LE_NONNULL obsPath,
        ///< [IN] Observation path. Can be absolute
        ///< (beginning with a '/') or relative to /obs/.
    double startTime
        ///< [IN] If < 30 years then seconds before now; else seconds since the Epoch.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current data type of a resource.
 *
 * @note Observations and Placeholders will default to IO_DATA_TYPE_TRIGGER, but will change
 *       types as other types of data are pushed to them.  The data types of Inputs and Outputs
 *       are decided by the apps that create them.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetDataType
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    io_DataType_t* dataTypePtr
        ///< [OUT] The fetched data type, if LE_OK is returned.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current units of a resource.
 *
 * @note Observations and Placeholders will default to "", but will change units as data is
 *       pushed to them.  The units of Inputs and Outputs are decided by the apps that create them.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 *  - LE_OVERFLOW if the units string was truncated because it is larger than the buffer provided.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetUnits
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    char* units,
        ///< [OUT] The fetched units, if LE_OK is returned.
    size_t unitsSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the timestamp of the current value of a resource.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 *  - LE_UNAVAILABLE if the resource doesn't have a current value (yet).
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetTimestamp
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    double* timestampPtr
        ///< [OUT] The fetched timestamp (in seconds since the Epoch), if LE_OK returned.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current value of a resource, if it's Boolean type.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 *  - LE_UNAVAILABLE if the resource doesn't have a current value (yet).
 *  - LE_FORMAT_ERROR if the resource has another data type.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetBoolean
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    double* timestampPtr,
        ///< [OUT] Fetched timestamp (in seconds since the Epoch), if LE_OK returned.
    bool* valuePtr
        ///< [OUT] Fetched value, if LE_OK returned.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current value of a resource, if it's numeric type.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 *  - LE_UNAVAILABLE if the resource doesn't have a current value (yet).
 *  - LE_FORMAT_ERROR if the resource has another data type.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetNumeric
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    double* timestampPtr,
        ///< [OUT] Fetched timestamp (in seconds since the Epoch), if LE_OK returned.
    double* valuePtr
        ///< [OUT] Fetched value, if LE_OK returned.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current value of a resource, if it's a string type.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 *  - LE_UNAVAILABLE if the resource doesn't have a current value (yet).
 *  - LE_FORMAT_ERROR if the resource has another data type.
 *  - LE_OVERFLOW if the value was truncated because it is larger than the buffer provided.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetString
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    double* timestampPtr,
        ///< [OUT] Fetched timestamp (in seconds since the Epoch), if LE_OK returned.
    char* value,
        ///< [OUT] Fetched value, if LE_OK returned.
    size_t valueSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the current value of a resource of any type, in JSON format.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to a namespace (which can't have a data type).
 *  - LE_UNAVAILABLE if the resource doesn't have a current value (yet).
 *  - LE_OVERFLOW if the value was truncated because it is larger than the buffer provided.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetJson
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    double* timestampPtr,
        ///< [OUT] Fetched timestamp (in seconds since the Epoch), if LE_OK returned.
    char* value,
        ///< [OUT] Fetched value, if LE_OK returned.
    size_t valueSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Fetch the example JSON value string for a given Input resource.
 *
 * @return
 *  - LE_OK if successful.
 *  - LE_NOT_FOUND if the resource was not found.
 *  - LE_UNSUPPORTED if the path refers to something that doesn't have a JSON type.
 *  - LE_UNAVAILABLE if the JSON-type resource doesn't have an example value.
 *  - LE_OVERFLOW if the value was truncated because it is larger than the buffer provided.
 */
//--------------------------------------------------------------------------------------------------
le_result_t query_GetJsonExample
(
    const char* LE_NONNULL path,
        ///< [IN] Resource path. Can be absolute (beginning
        ///< with a '/') or relative to the namespace of
        ///< the calling app (/app/<app-name>/).
    char* example,
        ///< [OUT] Example will be put here, if LE_OK returned.
    size_t exampleSize
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'query_TriggerPush'
 */
//--------------------------------------------------------------------------------------------------
query_TriggerPushHandlerRef_t query_AddTriggerPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    query_TriggerPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'query_TriggerPush'
 */
//--------------------------------------------------------------------------------------------------
void query_RemoveTriggerPushHandler
(
    query_TriggerPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'query_BooleanPush'
 */
//--------------------------------------------------------------------------------------------------
query_BooleanPushHandlerRef_t query_AddBooleanPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    query_BooleanPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'query_BooleanPush'
 */
//--------------------------------------------------------------------------------------------------
void query_RemoveBooleanPushHandler
(
    query_BooleanPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'query_NumericPush'
 */
//--------------------------------------------------------------------------------------------------
query_NumericPushHandlerRef_t query_AddNumericPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    query_NumericPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'query_NumericPush'
 */
//--------------------------------------------------------------------------------------------------
void query_RemoveNumericPushHandler
(
    query_NumericPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'query_StringPush'
 */
//--------------------------------------------------------------------------------------------------
query_StringPushHandlerRef_t query_AddStringPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    query_StringPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'query_StringPush'
 */
//--------------------------------------------------------------------------------------------------
void query_RemoveStringPushHandler
(
    query_StringPushHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'query_JsonPush'
 */
//--------------------------------------------------------------------------------------------------
query_JsonPushHandlerRef_t query_AddJsonPushHandler
(
    const char* LE_NONNULL path,
        ///< [IN] Absolute path of resource.
    query_JsonPushHandlerFunc_t callbackPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'query_JsonPush'
 */
//--------------------------------------------------------------------------------------------------
void query_RemoveJsonPushHandler
(
    query_JsonPushHandlerRef_t handlerRef
        ///< [IN]
);


#endif // QUERY_INTERFACE_H_INCLUDE_GUARD
