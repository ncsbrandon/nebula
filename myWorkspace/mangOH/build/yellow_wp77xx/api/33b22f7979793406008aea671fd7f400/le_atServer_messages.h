/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_ATSERVER_MESSAGES_H_INCLUDE_GUARD
#define LE_ATSERVER_MESSAGES_H_INCLUDE_GUARD


#include "le_atServer_common.h"

#define _MAX_MSG_SIZE IFGEN_LE_ATSERVER_MSG_SIZE

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_atServer_Suspend 0
#define _MSGID_le_atServer_Resume 1
#define _MSGID_le_atServer_Open 2
#define _MSGID_le_atServer_Close 3
#define _MSGID_le_atServer_Create 4
#define _MSGID_le_atServer_Delete 5
#define _MSGID_le_atServer_AddCmdRegistrationHandler 6
#define _MSGID_le_atServer_RemoveCmdRegistrationHandler 7
#define _MSGID_le_atServer_AddCommandHandler 8
#define _MSGID_le_atServer_RemoveCommandHandler 9
#define _MSGID_le_atServer_GetParameter 10
#define _MSGID_le_atServer_GetCommandName 11
#define _MSGID_le_atServer_GetDevice 12
#define _MSGID_le_atServer_SendIntermediateResponse 13
#define _MSGID_le_atServer_SendFinalResponse 14
#define _MSGID_le_atServer_SendFinalResultCode 15
#define _MSGID_le_atServer_SendUnsolicitedResponse 16
#define _MSGID_le_atServer_EnableEcho 17
#define _MSGID_le_atServer_DisableEcho 18
#define _MSGID_le_atServer_OpenBridge 19
#define _MSGID_le_atServer_CloseBridge 20
#define _MSGID_le_atServer_AddDeviceToBridge 21
#define _MSGID_le_atServer_RemoveDeviceFromBridge 22
#define _MSGID_le_atServer_EnableVerboseErrorCodes 23
#define _MSGID_le_atServer_EnableExtendedErrorCodes 24
#define _MSGID_le_atServer_DisableExtendedErrorCodes 25
#define _MSGID_le_atServer_CreateErrorCode 26
#define _MSGID_le_atServer_DeleteErrorCode 27
#define _MSGID_le_atServer_SetVerboseErrorCode 28
#define _MSGID_le_atServer_GetTextAsync 29


// Define type-safe pack/unpack functions for all enums, including included types

static inline bool le_atServer_PackType
(
    uint8_t **bufferPtr,
    le_atServer_Type_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool le_atServer_UnpackType
(
    uint8_t **bufferPtr,
    le_atServer_Type_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (le_atServer_Type_t)value;
    }
    return result;
}

static inline bool le_atServer_PackFinalRsp
(
    uint8_t **bufferPtr,
    le_atServer_FinalRsp_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool le_atServer_UnpackFinalRsp
(
    uint8_t **bufferPtr,
    le_atServer_FinalRsp_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (le_atServer_FinalRsp_t)value;
    }
    return result;
}

static inline bool le_atServer_PackAvailableDevice
(
    uint8_t **bufferPtr,
    le_atServer_AvailableDevice_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool le_atServer_UnpackAvailableDevice
(
    uint8_t **bufferPtr,
    le_atServer_AvailableDevice_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (le_atServer_AvailableDevice_t)value;
    }
    return result;
}

// Define pack/unpack functions for all structures, including included types


#endif // LE_ATSERVER_MESSAGES_H_INCLUDE_GUARD
