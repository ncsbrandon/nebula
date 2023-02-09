
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */
#ifndef MANGOH_BME680_COMMON_H_INCLUDE_GUARD
#define MANGOH_BME680_COMMON_H_INCLUDE_GUARD


#include "legato.h"

#define IFGEN_MANGOH_BME680_PROTOCOL_ID "795dde8cf87272df11aca7182d3e3f7e"
#define IFGEN_MANGOH_BME680_MSG_SIZE 73



//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    MANGOH_BME680_SAMPLING_RATE_DISABLED = 0,
        ///<
    MANGOH_BME680_SAMPLING_RATE_LP = 1,
        ///<
    MANGOH_BME680_SAMPLING_RATE_ULP = 2
        ///<
}
mangOH_bme680_SamplingRate_t;


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    bool valid;
    double value;
    uint8_t accuracy;
}
mangOH_bme680_ReadingIaq_t;


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    bool valid;
    double value;
    uint8_t accuracy;
}
mangOH_bme680_ReadingCo2Equivalent_t;


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    bool valid;
    double value;
    uint8_t accuracy;
}
mangOH_bme680_ReadingBreathVoc_t;


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    bool valid;
    double value;
}
mangOH_bme680_ReadingPressure_t;


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    bool valid;
    double value;
}
mangOH_bme680_ReadingTemperature_t;


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    bool valid;
    double value;
}
mangOH_bme680_ReadingHumidity_t;


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    int64_t timestamp;
    mangOH_bme680_ReadingIaq_t iaq;
    mangOH_bme680_ReadingCo2Equivalent_t co2Equivalent;
    mangOH_bme680_ReadingBreathVoc_t breathVoc;
    mangOH_bme680_ReadingPressure_t pressure;
    mangOH_bme680_ReadingTemperature_t temperature;
    mangOH_bme680_ReadingHumidity_t humidity;
}
mangOH_bme680_Reading_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'mangOH_bme680_SensorReading'
 */
//--------------------------------------------------------------------------------------------------
typedef struct mangOH_bme680_SensorReadingHandler* mangOH_bme680_SensorReadingHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef void (*mangOH_bme680_SensorReadingHandlerFunc_t)
(
        const mangOH_bme680_Reading_t * LE_NONNULL readingPtr,
        ///<
        void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Get if this client bound locally.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_mangOH_bme680_HasLocalBinding
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Init data that is common across all threads
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_mangOH_bme680_InitCommonData
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Perform common initialization and open a session
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_mangOH_bme680_OpenSession
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    bool isBlocking
);

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_mangOH_bme680_Configure
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        mangOH_bme680_SamplingRate_t samplingRate,
        ///< [IN]
        bool enableIaq,
        ///< [IN]
        bool enableCo2Equivalent,
        ///< [IN]
        bool enableBreathVoc,
        ///< [IN]
        bool enablePressure,
        ///< [IN]
        bool enableTemperature,
        ///< [IN]
        bool enableHumidity
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_mangOH_bme680_SetAmbientTemperature
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        double degC
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'mangOH_bme680_SensorReading'
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED mangOH_bme680_SensorReadingHandlerRef_t ifgen_mangOH_bme680_AddSensorReadingHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        mangOH_bme680_SensorReadingHandlerFunc_t handlerPtr,
        ///< [IN]
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'mangOH_bme680_SensorReading'
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_mangOH_bme680_RemoveSensorReadingHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        mangOH_bme680_SensorReadingHandlerRef_t handlerRef
        ///< [IN]
);

#endif // MANGOH_BME680_COMMON_H_INCLUDE_GUARD
