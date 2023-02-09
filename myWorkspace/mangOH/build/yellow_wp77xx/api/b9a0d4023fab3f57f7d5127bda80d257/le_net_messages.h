/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_NET_MESSAGES_H_INCLUDE_GUARD
#define LE_NET_MESSAGES_H_INCLUDE_GUARD


#include "le_net_common.h"

#define _MAX_MSG_SIZE IFGEN_LE_NET_MSG_SIZE

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_net_ChangeRoute 0
#define _MSGID_le_net_SetDefaultGW 1
#define _MSGID_le_net_GetDefaultGW 2
#define _MSGID_le_net_BackupDefaultGW 3
#define _MSGID_le_net_RestoreDefaultGW 4
#define _MSGID_le_net_SetDNS 5
#define _MSGID_le_net_GetDNS 6
#define _MSGID_le_net_RestoreDNS 7


// Define type-safe pack/unpack functions for all enums, including included types

static inline bool le_dcs_PackTechnology
(
    uint8_t **bufferPtr,
    le_dcs_Technology_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool le_dcs_UnpackTechnology
(
    uint8_t **bufferPtr,
    le_dcs_Technology_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (le_dcs_Technology_t)value;
    }
    return result;
}

static inline bool le_dcs_PackState
(
    uint8_t **bufferPtr,
    le_dcs_State_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool le_dcs_UnpackState
(
    uint8_t **bufferPtr,
    le_dcs_State_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (le_dcs_State_t)value;
    }
    return result;
}

static inline bool le_dcs_PackEvent
(
    uint8_t **bufferPtr,
    le_dcs_Event_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool le_dcs_UnpackEvent
(
    uint8_t **bufferPtr,
    le_dcs_Event_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (le_dcs_Event_t)value;
    }
    return result;
}

// Define pack/unpack functions for all structures, including included types


static inline bool le_net_PackDnsServerAddresses
(
    uint8_t **bufferPtr,
    const le_net_DnsServerAddresses_t *valuePtr
)
{
    __attribute__((unused))
    bool subResult, result = true;

    LE_ASSERT(valuePtr);
    subResult = le_pack_PackString( bufferPtr,
                                    valuePtr->ipv4Addr1, 16);
    result = result && subResult;
    subResult = le_pack_PackString( bufferPtr,
                                    valuePtr->ipv4Addr2, 16);
    result = result && subResult;
    subResult = le_pack_PackString( bufferPtr,
                                    valuePtr->ipv6Addr1, 46);
    result = result && subResult;
    subResult = le_pack_PackString( bufferPtr,
                                    valuePtr->ipv6Addr2, 46);
    result = result && subResult;

    return result;
}

static inline bool le_net_UnpackDnsServerAddresses
(
    uint8_t **bufferPtr,
    le_net_DnsServerAddresses_t *valuePtr
)
{
    bool result = true;
    if (result)
    {
        result = le_pack_UnpackString(bufferPtr,
                                      valuePtr->ipv4Addr1,
                                      sizeof(valuePtr->ipv4Addr1),
                                      16);
    }
    if (result)
    {
        result = le_pack_UnpackString(bufferPtr,
                                      valuePtr->ipv4Addr2,
                                      sizeof(valuePtr->ipv4Addr2),
                                      16);
    }
    if (result)
    {
        result = le_pack_UnpackString(bufferPtr,
                                      valuePtr->ipv6Addr1,
                                      sizeof(valuePtr->ipv6Addr1),
                                      46);
    }
    if (result)
    {
        result = le_pack_UnpackString(bufferPtr,
                                      valuePtr->ipv6Addr2,
                                      sizeof(valuePtr->ipv6Addr2),
                                      46);
    }
    return result;
}

static inline bool le_net_PackDefaultGatewayAddresses
(
    uint8_t **bufferPtr,
    const le_net_DefaultGatewayAddresses_t *valuePtr
)
{
    __attribute__((unused))
    bool subResult, result = true;

    LE_ASSERT(valuePtr);
    subResult = le_pack_PackString( bufferPtr,
                                    valuePtr->ipv4Addr, 16);
    result = result && subResult;
    subResult = le_pack_PackString( bufferPtr,
                                    valuePtr->ipv6Addr, 46);
    result = result && subResult;

    return result;
}

static inline bool le_net_UnpackDefaultGatewayAddresses
(
    uint8_t **bufferPtr,
    le_net_DefaultGatewayAddresses_t *valuePtr
)
{
    bool result = true;
    if (result)
    {
        result = le_pack_UnpackString(bufferPtr,
                                      valuePtr->ipv4Addr,
                                      sizeof(valuePtr->ipv4Addr),
                                      16);
    }
    if (result)
    {
        result = le_pack_UnpackString(bufferPtr,
                                      valuePtr->ipv6Addr,
                                      sizeof(valuePtr->ipv6Addr),
                                      46);
    }
    return result;
}

static inline bool le_dcs_PackChannelInfo
(
    uint8_t **bufferPtr,
    const le_dcs_ChannelInfo_t *valuePtr
)
{
    __attribute__((unused))
    bool subResult, result = true;

    LE_ASSERT(valuePtr);
    subResult = le_pack_PackReference( bufferPtr,
                                                 valuePtr->ref );
    result = result && subResult;
    subResult = le_pack_PackString( bufferPtr,
                                    valuePtr->name, 32);
    result = result && subResult;
    subResult = le_dcs_PackTechnology( bufferPtr,
                                                 valuePtr->technology );
    result = result && subResult;
    subResult = le_dcs_PackState( bufferPtr,
                                                 valuePtr->state );
    result = result && subResult;

    return result;
}

static inline bool le_dcs_UnpackChannelInfo
(
    uint8_t **bufferPtr,
    le_dcs_ChannelInfo_t *valuePtr
)
{
    bool result = true;
    if (result)
    {
        result = le_pack_UnpackReference(bufferPtr,
                                                   &valuePtr->ref );
    }
    if (result)
    {
        result = le_pack_UnpackString(bufferPtr,
                                      valuePtr->name,
                                      sizeof(valuePtr->name),
                                      32);
    }
    if (result)
    {
        result = le_dcs_UnpackTechnology(bufferPtr,
                                                   &valuePtr->technology );
    }
    if (result)
    {
        result = le_dcs_UnpackState(bufferPtr,
                                                   &valuePtr->state );
    }
    return result;
}

#endif // LE_NET_MESSAGES_H_INCLUDE_GUARD
