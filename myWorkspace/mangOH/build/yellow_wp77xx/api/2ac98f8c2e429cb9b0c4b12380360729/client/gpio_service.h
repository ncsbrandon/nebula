
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef GPIO_SERVICE_H_INCLUDE_GUARD
#define GPIO_SERVICE_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR IFGEN_LE_GPIO_PROTOCOL_ID
#ifdef MK_TOOLS_BUILD
    extern const char** gpio_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*gpio_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "le_gpio"
#endif

#endif // GPIO_SERVICE_H_INCLUDE_GUARD
