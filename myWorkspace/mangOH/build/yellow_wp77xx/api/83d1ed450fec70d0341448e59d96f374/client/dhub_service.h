
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef DHUB_SERVICE_H_INCLUDE_GUARD
#define DHUB_SERVICE_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR IFGEN_IO_PROTOCOL_ID
#ifdef MK_TOOLS_BUILD
    extern const char** dhub_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*dhub_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "io"
#endif

#endif // DHUB_SERVICE_H_INCLUDE_GUARD
