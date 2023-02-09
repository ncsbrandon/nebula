/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef MA_BATTERY_MESSAGES_H_INCLUDE_GUARD
#define MA_BATTERY_MESSAGES_H_INCLUDE_GUARD


#include "ma_battery_common.h"

#define _MAX_MSG_SIZE IFGEN_MA_BATTERY_MSG_SIZE

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_ma_battery_GetChargingStatus 0
#define _MSGID_ma_battery_GetHealthStatus 1
#define _MSGID_ma_battery_GetVoltage 2
#define _MSGID_ma_battery_GetCurrent 3
#define _MSGID_ma_battery_GetTemp 4
#define _MSGID_ma_battery_GetPercentRemaining 5
#define _MSGID_ma_battery_GetChargeRemaining 6
#define _MSGID_ma_battery_AddLevelPercentageHandler 7
#define _MSGID_ma_battery_RemoveLevelPercentageHandler 8
#define _MSGID_ma_battery_AddHealthChangeHandler 9
#define _MSGID_ma_battery_RemoveHealthChangeHandler 10
#define _MSGID_ma_battery_AddChargingStatusChangeHandler 11
#define _MSGID_ma_battery_RemoveChargingStatusChangeHandler 12


// Define type-safe pack/unpack functions for all enums, including included types

static inline bool ma_battery_PackChargingStatus
(
    uint8_t **bufferPtr,
    ma_battery_ChargingStatus_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool ma_battery_UnpackChargingStatus
(
    uint8_t **bufferPtr,
    ma_battery_ChargingStatus_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (ma_battery_ChargingStatus_t)value;
    }
    return result;
}

static inline bool ma_battery_PackHealthStatus
(
    uint8_t **bufferPtr,
    ma_battery_HealthStatus_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool ma_battery_UnpackHealthStatus
(
    uint8_t **bufferPtr,
    ma_battery_HealthStatus_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (ma_battery_HealthStatus_t)value;
    }
    return result;
}

// Define pack/unpack functions for all structures, including included types


#endif // MA_BATTERY_MESSAGES_H_INCLUDE_GUARD
