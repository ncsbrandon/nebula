
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef QUERY_SERVICE_H_INCLUDE_GUARD
#define QUERY_SERVICE_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR IFGEN_QUERY_PROTOCOL_ID
#ifdef MK_TOOLS_BUILD
    extern const char** query_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*query_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "query"
#endif

#endif // QUERY_SERVICE_H_INCLUDE_GUARD
