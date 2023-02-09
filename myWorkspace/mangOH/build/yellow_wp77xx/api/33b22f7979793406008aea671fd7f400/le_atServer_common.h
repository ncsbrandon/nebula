
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */
#ifndef LE_ATSERVER_COMMON_H_INCLUDE_GUARD
#define LE_ATSERVER_COMMON_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_atDefs_common.h"

#define IFGEN_LE_ATSERVER_PROTOCOL_ID "b7ddff0d499d71d3a23372d9447cbc78"
#define IFGEN_LE_ATSERVER_MSG_SIZE 4119



//--------------------------------------------------------------------------------------------------
/**
 *  Maximum text length.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATSERVER_TEXT_MAX_LEN 4096

//--------------------------------------------------------------------------------------------------
/**
 *  CME error pattern
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATSERVER_CME_ERROR "+CME ERROR: "

//--------------------------------------------------------------------------------------------------
/**
 *  CMS error pattern
 */
//--------------------------------------------------------------------------------------------------
#define LE_ATSERVER_CMS_ERROR "+CMS ERROR: "

//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for an AT command.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atServer_Cmd* le_atServer_CmdRef_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for a AT command device.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atServer_Device* le_atServer_DeviceRef_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for a AT commands server / AT commands client bridge.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atServer_Bridge* le_atServer_BridgeRef_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for an error code.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atServer_ErrorCode* le_atServer_ErrorCodeRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Received AT command type.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ATSERVER_TYPE_ACT = 0,
        ///< Action AT command (e.g. 'AT+CMD')
    LE_ATSERVER_TYPE_PARA = 1,
        ///< AT command received with parameters (e.g. 'AT+CMD=x,y')
    LE_ATSERVER_TYPE_TEST = 2,
        ///< Test AT command (e.g. 'AT+CMD=?')
    LE_ATSERVER_TYPE_READ = 3
        ///< Read AT command (e.g. 'AT+CMD?')
}
le_atServer_Type_t;


//--------------------------------------------------------------------------------------------------
/**
 * Final response.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ATSERVER_OK = 0,
        ///< AT command successfully treated
    LE_ATSERVER_NO_CARRIER = 1,
        ///< Connection terminated
    LE_ATSERVER_ERROR = 2,
        ///< Error in AT command treatement
    LE_ATSERVER_NO_DIALTONE = 3,
        ///< No dialtone detected
    LE_ATSERVER_BUSY = 4
        ///< Busy signal detected
}
le_atServer_FinalRsp_t;


//--------------------------------------------------------------------------------------------------
/**
 * Device availability.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ATSERVER_ALL_DEVICES = 0,
        ///< action will be done on all bound devices
    LE_ATSERVER_SPECIFIC_DEVICE = 1
        ///< action will be done on a specified device
}
le_atServer_AvailableDevice_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_atServer_CmdRegistration'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atServer_CmdRegistrationHandler* le_atServer_CmdRegistrationHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_atServer_Command'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_atServer_CommandHandler* le_atServer_CommandHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for platform-specific command registration.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_atServer_CmdRegistrationHandlerFunc_t)
(
        le_atServer_CmdRef_t commandRef,
        ///< AT command reference
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for the AT command processing.
 *
 * @note The argument "parametersNumber" is set only when "type" parameter value is
 * LE_AT_SERVER_TYPE_PARA
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_atServer_CommandHandlerFunc_t)
(
        le_atServer_CmdRef_t commandRef,
        ///< Received AT command reference
        le_atServer_Type_t type,
        ///< Received AT command type
        uint32_t parametersNumber,
        ///< Parameters number
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Get text callback
 *
 * @return
 *      - LE_OK: The function succeeded
 *      - LE_IO_ERROR: An io error happened and the function couldn't read from the device
 *      - LE_FORMAT_ERROR: Received an invalid character or an invalid sequence
 *      - LE_FAULT: Failed to remove backspaces
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_atServer_GetTextCallbackFunc_t)
(
        le_atServer_CmdRef_t cmdRef,
        ///< Received AT command reference
        le_result_t result,
        ///< Result
        const char* LE_NONNULL text,
        ///< Received text
        uint32_t len,
        ///< Text length
        void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Get if this client bound locally.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_le_atServer_HasLocalBinding
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Init data that is common across all threads
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_atServer_InitCommonData
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Perform common initialization and open a session
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_OpenSession
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    bool isBlocking
);

//--------------------------------------------------------------------------------------------------
/**
 * Suspend server / enter data mode
 *
 * When this function is called the server stops monitoring the fd for events
 * hence no more I/O operations are done on the fd by the server.
 *
 * @return
 *      - LE_OK             Success.
 *      - LE_BAD_PARAMETER  Invalid device reference.
 *      - LE_FAULT          Device not monitored
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_Suspend
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_DeviceRef_t device
        ///< [IN] device to be suspended
);

//--------------------------------------------------------------------------------------------------
/**
 * Resume server / enter command mode
 *
 * When this function is called the server resumes monitoring the fd for events
 * and is able to interpret AT commands again.
 *
 * @return
 *      - LE_OK             Success.
 *      - LE_BAD_PARAMETER  Invalid device reference.
 *      - LE_FAULT          Device not monitored
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_Resume
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_DeviceRef_t device
        ///< [IN] device to be resumed
);

//--------------------------------------------------------------------------------------------------
/**
 * This function opens an AT server session on the requested device.
 *
 * @return
 *      - Reference to the requested device.
 *      - NULL if the device is not available or fd is a BAD FILE DESCRIPTOR.
 *
 * @note Make sure to duplicate (man dup) your file descriptor before opening
 *       a server session to be able to use the suspend/resume feature
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_atServer_DeviceRef_t ifgen_le_atServer_Open
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        int fd
        ///< [IN] File descriptor.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function closes the AT server session on the requested device.
 *
 * @return
 *      - LE_OK             The function succeeded.
 *      - LE_BAD_PARAMETER  Invalid device reference.
 *      - LE_BUSY           The requested device is busy.
 *      - LE_FAULT          Failed to stop the server, check logs
 *                              for more information.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_Close
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_DeviceRef_t device
        ///< [IN] device to be unbound
);

//--------------------------------------------------------------------------------------------------
/**
 * This function created an AT command and register it into the AT parser.
 *
 * @return
 *      - Reference to the AT command.
 *      - NULL if an error occurs.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_atServer_CmdRef_t ifgen_le_atServer_Create
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        const char* LE_NONNULL name
        ///< [IN] AT command name string
);

//--------------------------------------------------------------------------------------------------
/**
 * This function deletes an AT command (i.e. unregister from the AT parser).
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to delete the command.
 *      - LE_BUSY          Command is in progress.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_Delete
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_CmdRef_t commandRef
        ///< [IN] AT command reference
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_atServer_CmdRegistration'
 *
 * This event provides information when a new AT command is subscribed.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_atServer_CmdRegistrationHandlerRef_t ifgen_le_atServer_AddCmdRegistrationHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_CmdRegistrationHandlerFunc_t handlerPtr,
        ///< [IN] Handler to called when a new AT command
        ///< is subscribed
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_atServer_CmdRegistration'
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_atServer_RemoveCmdRegistrationHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_CmdRegistrationHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_atServer_Command'
 *
 * This event provides information when the AT command is detected.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_atServer_CommandHandlerRef_t ifgen_le_atServer_AddCommandHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
        le_atServer_CommandHandlerFunc_t handlerPtr,
        ///< [IN] Handler to called when the AT command is detected
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_atServer_Command'
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_atServer_RemoveCommandHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_CommandHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to get the parameters of a received AT command.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to get the requested parameter.
 *
 * @note If the parameter is parsed with quotes, the quotes are removed when retrieving the
 * parameter value using this API. If a parmeter is not parsed with quotes, that parameter is
 * converted to uppercase equivalent.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_GetParameter
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
        uint32_t index,
        ///< [IN] agument index
        char* parameter,
        ///< [OUT] parameter value
        size_t parameterSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to get the AT command string.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to get the AT command string.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_GetCommandName
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
        char* name,
        ///< [OUT] AT command string
        size_t nameSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to get the device reference in use for an AT command specified with
 * its reference.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to get the AT command string.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_GetDevice
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
        le_atServer_DeviceRef_t* deviceRefPtr
        ///< [OUT] Device reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to send an intermediate response.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to send the intermediate response.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_SendIntermediateResponse
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
        const char* LE_NONNULL intermediateRsp
        ///< [IN] Intermediate response to be
        ///< sent
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to send the final response.
 *
 * @deprecated le_atServer_SendFinalResponse() should not be used anymore and will be removed soon.
 * It has been replaced by le_atServer_SendFinalResultCode()
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to send the final response.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_SendFinalResponse
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
        le_atServer_FinalRsp_t finalResponse,
        ///< [IN] Final response to be sent
        bool customStringAvailable,
        ///< [IN] Custom final response has to be sent
        ///< instead of the default one.
        const char* LE_NONNULL finalRsp
        ///< [IN] custom final response string
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to send the final result code.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to send the final result code.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_SendFinalResultCode
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_CmdRef_t commandRef,
        ///< [IN] AT command reference
        le_atServer_FinalRsp_t finalResult,
        ///< [IN] Final result code to be sent
        const char* LE_NONNULL pattern,
        ///< [IN] Prefix of the return message
        uint32_t errorCode
        ///< [IN] Numeric error code
);

//--------------------------------------------------------------------------------------------------
/**
 * This function can be used to send the unsolicited response.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to send the unsolicited response.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_SendUnsolicitedResponse
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        const char* LE_NONNULL unsolRsp,
        ///< [IN] Unsolicited response to be
        ///< sent
        le_atServer_AvailableDevice_t availableDevice,
        ///< [IN] device to send the
        ///< unsolicited response
        le_atServer_DeviceRef_t device
        ///< [IN] device reference where the
        ///< unsolicited response has to
        ///< be sent
);

//--------------------------------------------------------------------------------------------------
/**
 * This function enables echo on the selected device.
 *
 * @return
 *      - LE_OK             The function succeeded.
 *      - LE_BAD_PARAMETER  Invalid device reference.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_EnableEcho
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_DeviceRef_t device
        ///< [IN] device reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function disables echo on the selected device.
 *
 * @return
 *      - LE_OK             The function succeeded.
 *      - LE_BAD_PARAMETER  Invalid device reference.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_DisableEcho
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_DeviceRef_t device
        ///< [IN] device reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function opens a AT commands server bridge.
 * All unknown AT commands will be sent on this alternative file descriptor thanks to the AT client
 * Service.
 *
 * @return
 *      - Reference to the requested bridge.
 *      - NULL if the device can't be bridged
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_atServer_BridgeRef_t ifgen_le_atServer_OpenBridge
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        int fd
        ///< [IN] File descriptor.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function closes an opened bridge.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed to close the bridge.
 *      - LE_BUSY          The bridge is in use (devices references have to be removed first).
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_CloseBridge
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_BridgeRef_t bridgeRef
        ///< [IN] Bridge reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function adds a device to an opened bridge.
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_BUSY          The device is already used by the bridge.
 *      - LE_FAULT         The function failed to add the device to the bridge.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_AddDeviceToBridge
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_DeviceRef_t deviceRef,
        ///< [IN] Device reference to add to the bridge
        le_atServer_BridgeRef_t bridgeRef
        ///< [IN] Bridge refence
);

//--------------------------------------------------------------------------------------------------
/**
 * This function removes a device from a bridge
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_NOT_FOUND     The device is not isued by the specified bridge
 *      - LE_BUSY          The device is currently in use
 *      - LE_FAULT         The function failed to add the device to the bridge.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_RemoveDeviceFromBridge
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_DeviceRef_t deviceRef,
        ///< [IN] Device reference to add to the bridge
        le_atServer_BridgeRef_t bridgeRef
        ///< [IN] Bridge refence
);

//--------------------------------------------------------------------------------------------------
/**
 * This function enables verbose error codes on the selected device.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_atServer_EnableVerboseErrorCodes
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * This function enables extended error codes on the selected device.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_atServer_EnableExtendedErrorCodes
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * This function disables extended error codes on the selected device.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_atServer_DisableExtendedErrorCodes
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * This function creates a custom error code.
 * @return
 *      - ErrorCode    Reference to the created error code
 *      - NULL         Function failed to create the error code
 *
 * @note This function fails to create the error code if the combinaison (errorCode, pattern)
 * already exists or if the errorCode number is lower than 512.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_atServer_ErrorCodeRef_t ifgen_le_atServer_CreateErrorCode
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        uint32_t errorCode,
        ///< [IN] Numerical error code
        const char* LE_NONNULL pattern
        ///< [IN] Prefix of the response message
);

//--------------------------------------------------------------------------------------------------
/**
 * This function deletes a custom error code.
 *
 * @return
 *      - LE_OK            The function succeeded
 *      - LE_FAULT         The function failed to delete the error code
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_DeleteErrorCode
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_ErrorCodeRef_t errorCodeRef
        ///< [IN] Error code reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function adds a verbose message to a specified error code
 *
 * @return
 *      - LE_OK            The function succeeded
 *      - LE_FAULT         The function failed to set the verbose message
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_SetVerboseErrorCode
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_ErrorCodeRef_t errorCodeRef,
        ///< [IN] Error code reference
        const char* LE_NONNULL verboseCode
        ///< [IN] Verbose message
);

//--------------------------------------------------------------------------------------------------
/**
 * This function allows the user to register a le_atServer_GetTextCallback_t callback
 * to retrieve text and sends a prompt <CR><LF><greater_than><SPACE> on the current command's
 * device.
 *
 * @return
 *      - LE_OK             The function succeeded.
 *      - LE_BAD_PARAMETER  Invalid device or command reference.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_atServer_GetTextAsync
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_atServer_CmdRef_t cmdRef,
        ///< [IN] AT command reference
        le_atServer_GetTextCallbackFunc_t callbackPtr,
        ///< [IN] Get text callback
        void* contextPtr
        ///< [IN]
);

#endif // LE_ATSERVER_COMMON_H_INCLUDE_GUARD
