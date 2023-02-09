
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */
#ifndef MA_BATTERY_COMMON_H_INCLUDE_GUARD
#define MA_BATTERY_COMMON_H_INCLUDE_GUARD


#include "legato.h"

#define IFGEN_MA_BATTERY_PROTOCOL_ID "aa9a909b90eed30e9ed2136332950ea8"
#define IFGEN_MA_BATTERY_MSG_SIZE 20



//--------------------------------------------------------------------------------------------------
/**
 * Maximum number of characters in a battery type string (excluding any terminator character).
 */
//--------------------------------------------------------------------------------------------------
#define MA_BATTERY_MAX_BATT_TYPE_STR_LEN 128

//--------------------------------------------------------------------------------------------------
/**
 * Charging status of battery
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    MA_BATTERY_DISCHARGING = 0,
        ///< The battery must be discharging when there is no good power source or
        ///< there is a charge fault.  It could also be discharging when in
        ///< "supplement mode" but there is no way to tell when it's in that mode.
    MA_BATTERY_CHARGING = 1,
        ///< Battery is being charged
    MA_BATTERY_NOT_CHARGING = 2,
        ///< Battery is not being charged or discharged
    MA_BATTERY_FULL = 3,
        ///< Battery is fully charged
    MA_BATTERY_CHARGING_UNKNOWN = 4,
        ///< Battery charging status is not known
    MA_BATTERY_CHARGING_ERROR = 5
        ///< Error in getting the charging status
}
ma_battery_ChargingStatus_t;


//--------------------------------------------------------------------------------------------------
/**
 * Health status of battery
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    MA_BATTERY_OVERVOLTAGE = 0,
        ///< Battery over-voltage limit is clamped at some % above the battery
        ///< regulation voltage. When battery over voltage occurs, the charger device
        ///< immediately disables charge
    MA_BATTERY_GOOD = 1,
        ///< Battery is in good health
    MA_BATTERY_COLD = 2,
        ///< Battery is cold and cannot charge. Check datasheet
    MA_BATTERY_HOT = 3,
        ///< Battery is hot and cannot charge.  Check datasheet
    MA_BATTERY_DISCONNECTED = 4,
        ///< Battery is not connected.
    MA_BATTERY_HEALTH_UNKNOWN = 5,
        ///< Battery health is not known
    MA_BATTERY_HEALTH_ERROR = 6
        ///< Error in getting health
}
ma_battery_HealthStatus_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'ma_battery_LevelPercentage'
 */
//--------------------------------------------------------------------------------------------------
typedef struct ma_battery_LevelPercentageHandler* ma_battery_LevelPercentageHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'ma_battery_HealthChange'
 */
//--------------------------------------------------------------------------------------------------
typedef struct ma_battery_HealthChangeHandler* ma_battery_HealthChangeHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'ma_battery_ChargingStatusChange'
 */
//--------------------------------------------------------------------------------------------------
typedef struct ma_battery_ChargingStatusChangeHandler* ma_battery_ChargingStatusChangeHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Percentage charge level change event handler (callback).
 */
//--------------------------------------------------------------------------------------------------
typedef void (*ma_battery_LevelPercentageHandlerFunc_t)
(
        uint8_t percentage,
        ///< The battery charge percentage.
        uint8_t percentageTrigger,
        ///< The percentage threshold that triggered this notification.
        bool isHighLevel,
        ///< true = percentage is higher than trigger level, false = lower.
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Battery health change event handler (callback).
 */
//--------------------------------------------------------------------------------------------------
typedef void (*ma_battery_HealthHandlerFunc_t)
(
        ma_battery_HealthStatus_t health,
        ///<
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Battery charge change event handler (callback).
 */
//--------------------------------------------------------------------------------------------------
typedef void (*ma_battery_ChargingStatusHandlerFunc_t)
(
        ma_battery_ChargingStatus_t condition,
        ///<
        void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Get if this client bound locally.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_ma_battery_HasLocalBinding
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Init data that is common across all threads
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_ma_battery_InitCommonData
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Perform common initialization and open a session
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_ma_battery_OpenSession
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    bool isBlocking
);

//--------------------------------------------------------------------------------------------------
/**
 * Provides battery charging status
 *
 * @return Charging status code.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED ma_battery_ChargingStatus_t ifgen_ma_battery_GetChargingStatus
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Provides battery health status
 *
 * @return Health status code.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED ma_battery_HealthStatus_t ifgen_ma_battery_GetHealthStatus
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Get battery voltage (in Volts)
 *
 * @return
 *     - LE_OK on success.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_ma_battery_GetVoltage
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED le_result_t ifgen_ma_battery_GetCurrent
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED le_result_t ifgen_ma_battery_GetTemp
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED le_result_t ifgen_ma_battery_GetPercentRemaining
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED le_result_t ifgen_ma_battery_GetChargeRemaining
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED ma_battery_LevelPercentageHandlerRef_t ifgen_ma_battery_AddLevelPercentageHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED void ifgen_ma_battery_RemoveLevelPercentageHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED ma_battery_HealthChangeHandlerRef_t ifgen_ma_battery_AddHealthChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED void ifgen_ma_battery_RemoveHealthChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED ma_battery_ChargingStatusChangeHandlerRef_t ifgen_ma_battery_AddChargingStatusChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED void ifgen_ma_battery_RemoveChargingStatusChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        ma_battery_ChargingStatusChangeHandlerRef_t handlerRef
        ///< [IN]
);

#endif // MA_BATTERY_COMMON_H_INCLUDE_GUARD
