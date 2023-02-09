/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_RTC_MESSAGES_H_INCLUDE_GUARD
#define LE_RTC_MESSAGES_H_INCLUDE_GUARD


#include "le_rtc_common.h"

#define _MAX_MSG_SIZE IFGEN_LE_RTC_MSG_SIZE

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_le_rtc_GetUserTime 0
#define _MSGID_le_rtc_SetUserTime 1


// Define type-safe pack/unpack functions for all enums, including included types

// Define pack/unpack functions for all structures, including included types


#endif // LE_RTC_MESSAGES_H_INCLUDE_GUARD
