
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */
#ifndef LE_ULPM_COMMON_H_INCLUDE_GUARD
#define LE_ULPM_COMMON_H_INCLUDE_GUARD


#include "legato.h"

#define IFGEN_LE_ULPM_PROTOCOL_ID "9431e37e61d607aea62e91937079db61"
#define IFGEN_LE_ULPM_MSG_SIZE 32



//--------------------------------------------------------------------------------------------------
/**
 * Maximum version length of mcu firmware.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ULPM_MAX_VERS_LEN 8

//--------------------------------------------------------------------------------------------------
/**
 * State of gpio pin. This state will be used to exit from low power state.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ULPM_GPIO_LOW = 0,
        ///< Gpio voltage level low.
    LE_ULPM_GPIO_HIGH = 1,
        ///< Gpio voltage level high.
    LE_ULPM_GPIO_RISING = 2,
        ///< Gpio edge rising.
    LE_ULPM_GPIO_FALLING = 3,
        ///< Gpio edge falling.
    LE_ULPM_GPIO_BOTH = 4,
        ///< Gpio edge either rising or falling.
    LE_ULPM_GPIO_OFF = 5
        ///< Gpio off.
}
le_ulpm_GpioState_t;


//--------------------------------------------------------------------------------------------------
/**
 * Possible ULPS (Ultra Low Power State) configurations to select before shutdown.
 *
 * Value 3 is skipped as it should not be used according to swimcu_pm documentation.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_ULPM_DISABLE_PSM = 0,
        ///< Resquest to disable PSM
    LE_ULPM_PSM_WITH_ULPM_FALLBACK = 1,
        ///< Request enable PSM with ULPM fallback
    LE_ULPM_POWER_OFF = 2,
        ///< Request power off module
    LE_ULPM_NO_REQUEST = 4,
        ///< No request (Default value)
    LE_ULPM_PSM_ONLY = 5,
        ///< Request enable PSM only
    LE_ULPM_ULPM_ONLY = 6
        ///< Request enable ULPM only
}
le_ulpm_ULPSConfiguration_t;



//--------------------------------------------------------------------------------------------------
/**
 * Get if this client bound locally.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_le_ulpm_HasLocalBinding
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Init data that is common across all threads
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_ulpm_InitCommonData
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Perform common initialization and open a session
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_ulpm_OpenSession
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    bool isBlocking
);

//--------------------------------------------------------------------------------------------------
/**
 * Configure the system to boot based on a state change of a given GPIO.
 *
 * @return
 *      - LE_OK if specified gpio is configured as boot source.
 *      - LE_NOT_PERMITTED if the process lacks sufficient permissions to configure the GPIO as a
 *        boot source.
 *      - LE_UNSUPPORTED if the device lacks the ability to boot based on the given GPIO.
 *      - LE_BAD_PARAMETER if the state parameter was rejected.
 *      - LE_FAULT if there is a non-specific failure.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_ulpm_BootOnGpio
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        uint32_t gpioNum,
        ///< [IN] Gpio number to boot.
        le_ulpm_GpioState_t state
        ///< [IN] State which should cause boot.
);

//--------------------------------------------------------------------------------------------------
/**
 * Boot after expiration of timer interval.
 *
 * @return
 *      - LE_OK if timer is configured as boot source.
 *      - LE_NOT_PERMITTED if the process lacks sufficient permissions to configure the timer as a
 *        boot source.
 *      - LE_UNSUPPORTED if the device lacks the ability to boot based on a timer.
 *      - LE_BAD_PARAMETER if the state parameter was rejected.
 *      - LE_FAULT if there is a non-specific failure.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_ulpm_BootOnTimer
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        uint32_t expiryVal
        ///< [IN] Expiration time(in second) to boot. This is relative time from
        ///< modem/app processor shutdown.
);

//--------------------------------------------------------------------------------------------------
/**
 * Configure and enable an ADC as a boot source.
 *
 * It is possible to specify a single range of operation or two ranges of operation with a
 * non-operational range in between.  When bootAboveAdcReading is less than bootBelowAdcReading,
 * then a single range bootAboveReading to bootBelowReading is the configured operational range.
 * However if bootAboveAdcReading is greater than bootBelowAdcReading, then there are two
 * operational ranges.  The first is any reading less than bootBelowAdcReading and the second is any
 * reading greater than bootAboveAdcReading.
 *
 * @return
 *      - LE_OK on success
 *      - LE_NOT_PERMITTED if the process lacks sufficient permissions to configure the adc as a
 *        boot source.
 *      - LE_OVERFLOW if the provided bootAboveAdcReading or bootBelowAdcReading are too large to
 *        convert to fit in the internal string buffer.
 *      - LE_BAD_PARAMETER if the pollIntervalInMs, bootAboveAdcReading or bootBelowAdcReading
 *        parameter were rejected.
 *      - LE_UNSUPPORTED if the device does not support using the given adc as a boot source.
 *      - LE_FAULT if there is a non-specific failure.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_ulpm_BootOnAdc
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        uint32_t adcNum,
        ///< [IN] Number of the ADC to configure
        uint32_t pollIntervalInMs,
        ///< [IN] How frequently to poll the ADC while sleeping
        double bootAboveAdcReading,
        ///< [IN] Reading above which the system should boot
        double bootBelowAdcReading
        ///< [IN] Reading below which the system should boot
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the ultra low power manager firmware version.
 *
 * @return
 *      - LE_OK on success
 *      - LE_OVERFLOW if version string to big to fit in provided buffer
 *      - LE_FAULT for any other errors
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_ulpm_GetFirmwareVersion
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        char* version,
        ///< [OUT] Firmware version string
        size_t versionSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Initiate shutting down of app processor/modem etc.
 *
 * @return
 *      - LE_OK if entry to ultra low power mode initiates properly.
 *      - LE_NOT_POSSIBLE if shutting is not possible now. Try again.
 *      - LE_NOT_PERMITTED if the process lacks sufficient permissions to perform a shutdown.
 *      - LE_UNSUPPORTED if the device lacks the ability to shutdown via ULPM.
 *      - LE_FAULT if there is a non-specific failure.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_ulpm_ShutDown
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Initiate reboot of app processor/modem etc.
 *
 * @return
 *      - LE_OK if reboot initiates properly.
 *      - LE_NOT_POSSIBLE if rebooting is not possible now. Try again.
 *      - LE_NOT_PERMITTED if the process lacks sufficient permissions to perform a reboot.
 *      - LE_UNSUPPORTED if the device lacks the ability to reboot via ULPM.
 *      - LE_FAULT if there is a non-specific failure.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_ulpm_Reboot
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Low Power Mode configuration to apply when le_ulpm_ShutDown is called.
 *
 * @return
 *      - LE_OK on success
 *      - LE_BAD_PARAMETER if the value of a parameter is invalid
 *      - LE_UNSUPPORTED if not supported
 *      - LE_FAULT for any other errors
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_ulpm_SetShutDownStrategy
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_ulpm_ULPSConfiguration_t ulpsConfig
        ///< [IN] Value used to configure ULPS.
);

#endif // LE_ULPM_COMMON_H_INCLUDE_GUARD
