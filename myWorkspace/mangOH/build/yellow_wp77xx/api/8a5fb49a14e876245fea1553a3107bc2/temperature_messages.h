/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef TEMPERATURE_MESSAGES_H_INCLUDE_GUARD
#define TEMPERATURE_MESSAGES_H_INCLUDE_GUARD


#include "temperature_common.h"

#define _MAX_MSG_SIZE IFGEN_TEMPERATURE_MSG_SIZE

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_temperature_Read 0


// Define type-safe pack/unpack functions for all enums, including included types

// Define pack/unpack functions for all structures, including included types


#endif // TEMPERATURE_MESSAGES_H_INCLUDE_GUARD
