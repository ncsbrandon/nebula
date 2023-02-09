

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef MA_BATTERY_INTERFACE_H_INCLUDE_GUARD
#define MA_BATTERY_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Internal includes for this interface
#include "ma_battery_common.h"
//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t ma_battery_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t ma_battery_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void ma_battery_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Charging status of battery
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Health status of battery
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Percentage charge level change event handler (callback).
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'ma_battery_LevelPercentage'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Battery health change event handler (callback).
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'ma_battery_HealthChange'
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Battery charge change event handler (callback).
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'ma_battery_ChargingStatusChange'
 */
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
/**
 * Provides battery charging status
 *
 * @return Charging status code.
 */
//--------------------------------------------------------------------------------------------------
ma_battery_ChargingStatus_t ma_battery_GetChargingStatus
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Provides battery health status
 *
 * @return Health status code.
 */
//--------------------------------------------------------------------------------------------------
ma_battery_HealthStatus_t ma_battery_GetHealthStatus
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Get battery voltage (in Volts)
 *
 * @return
 *     - LE_OK on success.
 */
//--------------------------------------------------------------------------------------------------
le_result_t ma_battery_GetVoltage
(
    double* voltagePtr
        ///< [OUT] The battery voltage, in V, if LE_OK is returned.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get battery current (in mA)
 *
 * @return
 *     - LE_OK on success.
 */
//--------------------------------------------------------------------------------------------------
le_result_t ma_battery_GetCurrent
(
    double* currentPtr
        ///< [OUT] The battery current, in mA, if LE_OK is returned.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get battery temperature in degrees Celcius
 *
 * @return
 *     - LE_OK on success.
 */
//--------------------------------------------------------------------------------------------------
le_result_t ma_battery_GetTemp
(
    double* tempPtr
        ///< [OUT] Temperature in degrees Celcius, if LE_OK is returned.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get charge remaining, in percentage
 *
 * @return
 *     - LE_OK on success
 */
//--------------------------------------------------------------------------------------------------
le_result_t ma_battery_GetPercentRemaining
(
    uint16_t* percentPtr
        ///< [OUT] Percentage battery remaining, if LE_OK is returned.
);



//--------------------------------------------------------------------------------------------------
/**
 * Get charge remaining, in mAh
 *
 * @ return
 *    - LE_OK on success
 */
//--------------------------------------------------------------------------------------------------
le_result_t ma_battery_GetChargeRemaining
(
    uint16_t* chargePtr
        ///< [OUT] Charge in mAh remaining, if LE_OK is returned.
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'ma_battery_LevelPercentage'
 *
 * Register a callback function to be called the battery level goes above levelHigh or
 * below levelLow.
 */
//--------------------------------------------------------------------------------------------------
ma_battery_LevelPercentageHandlerRef_t ma_battery_AddLevelPercentageHandler
(
    uint8_t levelLow,
        ///< [IN] Low percentage trigger threshold (0 = no low level alarm)
    uint8_t levelHigh,
        ///< [IN] High percentage trigger threshold (>=100 means no high alm)
    ma_battery_LevelPercentageHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'ma_battery_LevelPercentage'
 */
//--------------------------------------------------------------------------------------------------
void ma_battery_RemoveLevelPercentageHandler
(
    ma_battery_LevelPercentageHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'ma_battery_HealthChange'
 *
 * Register a callback function to be called when there is a change in battery health status.
 */
//--------------------------------------------------------------------------------------------------
ma_battery_HealthChangeHandlerRef_t ma_battery_AddHealthChangeHandler
(
    ma_battery_HealthHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'ma_battery_HealthChange'
 */
//--------------------------------------------------------------------------------------------------
void ma_battery_RemoveHealthChangeHandler
(
    ma_battery_HealthChangeHandlerRef_t handlerRef
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'ma_battery_ChargingStatusChange'
 *
 * Register a callback function to be called when there is a change in battery charging status.
 */
//--------------------------------------------------------------------------------------------------
ma_battery_ChargingStatusChangeHandlerRef_t ma_battery_AddChargingStatusChangeHandler
(
    ma_battery_ChargingStatusHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'ma_battery_ChargingStatusChange'
 */
//--------------------------------------------------------------------------------------------------
void ma_battery_RemoveChargingStatusChangeHandler
(
    ma_battery_ChargingStatusChangeHandlerRef_t handlerRef
        ///< [IN]
);


#endif // MA_BATTERY_INTERFACE_H_INCLUDE_GUARD
