/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef QUERY_MESSAGES_H_INCLUDE_GUARD
#define QUERY_MESSAGES_H_INCLUDE_GUARD


#include "query_common.h"

#define _MAX_MSG_SIZE IFGEN_QUERY_MSG_SIZE

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_query_ReadBufferJson 0
#define _MSGID_query_ReadBufferSampleTimestamp 1
#define _MSGID_query_ReadBufferSampleBoolean 2
#define _MSGID_query_ReadBufferSampleNumeric 3
#define _MSGID_query_ReadBufferSampleString 4
#define _MSGID_query_ReadBufferSampleJson 5
#define _MSGID_query_GetMin 6
#define _MSGID_query_GetMax 7
#define _MSGID_query_GetMean 8
#define _MSGID_query_GetStdDev 9
#define _MSGID_query_GetDataType 10
#define _MSGID_query_GetUnits 11
#define _MSGID_query_GetTimestamp 12
#define _MSGID_query_GetBoolean 13
#define _MSGID_query_GetNumeric 14
#define _MSGID_query_GetString 15
#define _MSGID_query_GetJson 16
#define _MSGID_query_GetJsonExample 17
#define _MSGID_query_AddTriggerPushHandler 18
#define _MSGID_query_RemoveTriggerPushHandler 19
#define _MSGID_query_AddBooleanPushHandler 20
#define _MSGID_query_RemoveBooleanPushHandler 21
#define _MSGID_query_AddNumericPushHandler 22
#define _MSGID_query_RemoveNumericPushHandler 23
#define _MSGID_query_AddStringPushHandler 24
#define _MSGID_query_RemoveStringPushHandler 25
#define _MSGID_query_AddJsonPushHandler 26
#define _MSGID_query_RemoveJsonPushHandler 27


// Define type-safe pack/unpack functions for all enums, including included types

static inline bool io_PackDataType
(
    uint8_t **bufferPtr,
    io_DataType_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool io_UnpackDataType
(
    uint8_t **bufferPtr,
    io_DataType_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (io_DataType_t)value;
    }
    return result;
}

// Define pack/unpack functions for all structures, including included types


#endif // QUERY_MESSAGES_H_INCLUDE_GUARD
