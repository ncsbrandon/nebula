
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LIGHT_SERVICE_H_INCLUDE_GUARD
#define LIGHT_SERVICE_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR IFGEN_LIGHT_PROTOCOL_ID
#ifdef MK_TOOLS_BUILD
    extern const char** light_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*light_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "light"
#endif

#endif // LIGHT_SERVICE_H_INCLUDE_GUARD
