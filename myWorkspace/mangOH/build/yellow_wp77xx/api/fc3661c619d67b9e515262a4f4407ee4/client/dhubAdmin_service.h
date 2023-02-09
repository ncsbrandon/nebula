
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef DHUBADMIN_SERVICE_H_INCLUDE_GUARD
#define DHUBADMIN_SERVICE_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR IFGEN_ADMIN_PROTOCOL_ID
#ifdef MK_TOOLS_BUILD
    extern const char** dhubAdmin_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*dhubAdmin_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "admin"
#endif

#endif // DHUBADMIN_SERVICE_H_INCLUDE_GUARD
