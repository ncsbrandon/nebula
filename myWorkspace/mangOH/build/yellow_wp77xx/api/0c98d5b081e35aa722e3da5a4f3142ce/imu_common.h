
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */
#ifndef IMU_COMMON_H_INCLUDE_GUARD
#define IMU_COMMON_H_INCLUDE_GUARD


#include "legato.h"

#define IFGEN_IMU_PROTOCOL_ID "6d14d7f2b815e817176e591c206d6e52"
#define IFGEN_IMU_MSG_SIZE 36




//--------------------------------------------------------------------------------------------------
/**
 * Get if this client bound locally.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_imu_HasLocalBinding
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Init data that is common across all threads
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_imu_InitCommonData
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Perform common initialization and open a session
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_imu_OpenSession
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    bool isBlocking
);

//--------------------------------------------------------------------------------------------------
/**
 * Read the accelerometer's linear acceleration measurement in meters per second squared.
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_imu_ReadAccel
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        double* xPtr,
        ///< [OUT] Where the x-axis acceleration (m/s2) will be put if LE_OK is returned.
        double* yPtr,
        ///< [OUT] Where the y-axis acceleration (m/s2) will be put if LE_OK is returned.
        double* zPtr
        ///< [OUT] Where the z-axis acceleration (m/s2) will be put if LE_OK is returned.
);

//--------------------------------------------------------------------------------------------------
/**
 * Read the gyroscope's angular velocity measurement in radians per seconds.
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_imu_ReadGyro
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        double* xPtr,
        ///< [OUT] Where the x-axis rotation (rads/s) will be put if LE_OK is returned.
        double* yPtr,
        ///< [OUT] Where the y-axis rotation (rads/s) will be put if LE_OK is returned.
        double* zPtr
        ///< [OUT] Where the z-axis rotation (rads/s) will be put if LE_OK is returned.
);

#endif // IMU_COMMON_H_INCLUDE_GUARD
