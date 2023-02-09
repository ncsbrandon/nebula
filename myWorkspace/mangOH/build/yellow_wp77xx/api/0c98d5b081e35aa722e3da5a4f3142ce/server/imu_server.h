

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef IMU_INTERFACE_H_INCLUDE_GUARD
#define IMU_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Internal includes for this interface
#include "imu_common.h"
//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t imu_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t imu_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void imu_AdvertiseService
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Read the accelerometer's linear acceleration measurement in meters per second squared.
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t imu_ReadAccel
(
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
le_result_t imu_ReadGyro
(
    double* xPtr,
        ///< [OUT] Where the x-axis rotation (rads/s) will be put if LE_OK is returned.
    double* yPtr,
        ///< [OUT] Where the y-axis rotation (rads/s) will be put if LE_OK is returned.
    double* zPtr
        ///< [OUT] Where the z-axis rotation (rads/s) will be put if LE_OK is returned.
);


#endif // IMU_INTERFACE_H_INCLUDE_GUARD
