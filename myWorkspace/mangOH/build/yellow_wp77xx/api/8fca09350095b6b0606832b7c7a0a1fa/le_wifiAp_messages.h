/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_WIFIAP_MESSAGES_H_INCLUDE_GUARD
#define LE_WIFIAP_MESSAGES_H_INCLUDE_GUARD


#include "le_wifiAp_common.h"

#define _MAX_MSG_SIZE IFGEN_LE_WIFIAP_MSG_SIZE

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_wifiAp_AddNewEventHandler 0
#define _MSGID_le_wifiAp_RemoveNewEventHandler 1
#define _MSGID_le_wifiAp_Start 2
#define _MSGID_le_wifiAp_Stop 3
#define _MSGID_le_wifiAp_SetSsid 4
#define _MSGID_le_wifiAp_SetSecurityProtocol 5
#define _MSGID_le_wifiAp_SetPassPhrase 6
#define _MSGID_le_wifiAp_SetPreSharedKey 7
#define _MSGID_le_wifiAp_SetDiscoverable 8
#define _MSGID_le_wifiAp_SetIeeeStandard 9
#define _MSGID_le_wifiAp_GetIeeeStandard 10
#define _MSGID_le_wifiAp_SetChannel 11
#define _MSGID_le_wifiAp_SetCountryCode 12
#define _MSGID_le_wifiAp_SetMaxNumberOfClients 13
#define _MSGID_le_wifiAp_SetIpRange 14


// Define type-safe pack/unpack functions for all enums, including included types

static inline bool le_wifiAp_PackEvent
(
    uint8_t **bufferPtr,
    le_wifiAp_Event_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool le_wifiAp_UnpackEvent
(
    uint8_t **bufferPtr,
    le_wifiAp_Event_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (le_wifiAp_Event_t)value;
    }
    return result;
}

static inline bool le_wifiAp_PackSecurityProtocol
(
    uint8_t **bufferPtr,
    le_wifiAp_SecurityProtocol_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool le_wifiAp_UnpackSecurityProtocol
(
    uint8_t **bufferPtr,
    le_wifiAp_SecurityProtocol_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (le_wifiAp_SecurityProtocol_t)value;
    }
    return result;
}

static inline bool le_wifiAp_PackIeeeStdBitMask
(
    uint8_t **bufferPtr,
    le_wifiAp_IeeeStdBitMask_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool le_wifiAp_UnpackIeeeStdBitMask
(
    uint8_t **bufferPtr,
    le_wifiAp_IeeeStdBitMask_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (le_wifiAp_IeeeStdBitMask_t)value;
    }
    return result;
}

// Define pack/unpack functions for all structures, including included types


#endif // LE_WIFIAP_MESSAGES_H_INCLUDE_GUARD
