
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef IMU_SERVICE_H_INCLUDE_GUARD
#define IMU_SERVICE_H_INCLUDE_GUARD


#include "legato.h"

#define PROTOCOL_ID_STR IFGEN_IMU_PROTOCOL_ID
#ifdef MK_TOOLS_BUILD
    extern const char** imu_ServiceInstanceNamePtr;
    #define SERVICE_INSTANCE_NAME (*imu_ServiceInstanceNamePtr)
#else
    #define SERVICE_INSTANCE_NAME "imu"
#endif

#endif // IMU_SERVICE_H_INCLUDE_GUARD
