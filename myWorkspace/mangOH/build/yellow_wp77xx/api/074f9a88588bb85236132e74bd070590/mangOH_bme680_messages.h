/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef MANGOH_BME680_MESSAGES_H_INCLUDE_GUARD
#define MANGOH_BME680_MESSAGES_H_INCLUDE_GUARD


#include "mangOH_bme680_common.h"

#define _MAX_MSG_SIZE IFGEN_MANGOH_BME680_MSG_SIZE

// Define the message type for communicating between client and server
typedef struct __attribute__((packed))
{
    uint32_t id;
    uint8_t buffer[_MAX_MSG_SIZE];
}
_Message_t;

#define _MSGID_mangOH_bme680_Configure 0
#define _MSGID_mangOH_bme680_SetAmbientTemperature 1
#define _MSGID_mangOH_bme680_AddSensorReadingHandler 2
#define _MSGID_mangOH_bme680_RemoveSensorReadingHandler 3


// Define type-safe pack/unpack functions for all enums, including included types

static inline bool mangOH_bme680_PackSamplingRate
(
    uint8_t **bufferPtr,
    mangOH_bme680_SamplingRate_t value
)
{
    return le_pack_PackUint32(bufferPtr, (uint32_t)value);
}

static inline bool mangOH_bme680_UnpackSamplingRate
(
    uint8_t **bufferPtr,
    mangOH_bme680_SamplingRate_t* valuePtr
)
{
    bool result;
    uint32_t value = 0;
    result = le_pack_UnpackUint32(bufferPtr, &value);
    if (result)
    {
        *valuePtr = (mangOH_bme680_SamplingRate_t)value;
    }
    return result;
}

// Define pack/unpack functions for all structures, including included types


static inline bool mangOH_bme680_PackReadingIaq
(
    uint8_t **bufferPtr,
    const mangOH_bme680_ReadingIaq_t *valuePtr
)
{
    __attribute__((unused))
    bool subResult, result = true;

    LE_ASSERT(valuePtr);
    subResult = le_pack_PackBool( bufferPtr,
                                                 valuePtr->valid );
    result = result && subResult;
    subResult = le_pack_PackDouble( bufferPtr,
                                                 valuePtr->value );
    result = result && subResult;
    subResult = le_pack_PackUint8( bufferPtr,
                                                 valuePtr->accuracy );
    result = result && subResult;

    return result;
}

static inline bool mangOH_bme680_UnpackReadingIaq
(
    uint8_t **bufferPtr,
    mangOH_bme680_ReadingIaq_t *valuePtr
)
{
    bool result = true;
    if (result)
    {
        result = le_pack_UnpackBool(bufferPtr,
                                                   &valuePtr->valid );
    }
    if (result)
    {
        result = le_pack_UnpackDouble(bufferPtr,
                                                   &valuePtr->value );
    }
    if (result)
    {
        result = le_pack_UnpackUint8(bufferPtr,
                                                   &valuePtr->accuracy );
    }
    return result;
}

static inline bool mangOH_bme680_PackReadingCo2Equivalent
(
    uint8_t **bufferPtr,
    const mangOH_bme680_ReadingCo2Equivalent_t *valuePtr
)
{
    __attribute__((unused))
    bool subResult, result = true;

    LE_ASSERT(valuePtr);
    subResult = le_pack_PackBool( bufferPtr,
                                                 valuePtr->valid );
    result = result && subResult;
    subResult = le_pack_PackDouble( bufferPtr,
                                                 valuePtr->value );
    result = result && subResult;
    subResult = le_pack_PackUint8( bufferPtr,
                                                 valuePtr->accuracy );
    result = result && subResult;

    return result;
}

static inline bool mangOH_bme680_UnpackReadingCo2Equivalent
(
    uint8_t **bufferPtr,
    mangOH_bme680_ReadingCo2Equivalent_t *valuePtr
)
{
    bool result = true;
    if (result)
    {
        result = le_pack_UnpackBool(bufferPtr,
                                                   &valuePtr->valid );
    }
    if (result)
    {
        result = le_pack_UnpackDouble(bufferPtr,
                                                   &valuePtr->value );
    }
    if (result)
    {
        result = le_pack_UnpackUint8(bufferPtr,
                                                   &valuePtr->accuracy );
    }
    return result;
}

static inline bool mangOH_bme680_PackReadingBreathVoc
(
    uint8_t **bufferPtr,
    const mangOH_bme680_ReadingBreathVoc_t *valuePtr
)
{
    __attribute__((unused))
    bool subResult, result = true;

    LE_ASSERT(valuePtr);
    subResult = le_pack_PackBool( bufferPtr,
                                                 valuePtr->valid );
    result = result && subResult;
    subResult = le_pack_PackDouble( bufferPtr,
                                                 valuePtr->value );
    result = result && subResult;
    subResult = le_pack_PackUint8( bufferPtr,
                                                 valuePtr->accuracy );
    result = result && subResult;

    return result;
}

static inline bool mangOH_bme680_UnpackReadingBreathVoc
(
    uint8_t **bufferPtr,
    mangOH_bme680_ReadingBreathVoc_t *valuePtr
)
{
    bool result = true;
    if (result)
    {
        result = le_pack_UnpackBool(bufferPtr,
                                                   &valuePtr->valid );
    }
    if (result)
    {
        result = le_pack_UnpackDouble(bufferPtr,
                                                   &valuePtr->value );
    }
    if (result)
    {
        result = le_pack_UnpackUint8(bufferPtr,
                                                   &valuePtr->accuracy );
    }
    return result;
}

static inline bool mangOH_bme680_PackReadingPressure
(
    uint8_t **bufferPtr,
    const mangOH_bme680_ReadingPressure_t *valuePtr
)
{
    __attribute__((unused))
    bool subResult, result = true;

    LE_ASSERT(valuePtr);
    subResult = le_pack_PackBool( bufferPtr,
                                                 valuePtr->valid );
    result = result && subResult;
    subResult = le_pack_PackDouble( bufferPtr,
                                                 valuePtr->value );
    result = result && subResult;

    return result;
}

static inline bool mangOH_bme680_UnpackReadingPressure
(
    uint8_t **bufferPtr,
    mangOH_bme680_ReadingPressure_t *valuePtr
)
{
    bool result = true;
    if (result)
    {
        result = le_pack_UnpackBool(bufferPtr,
                                                   &valuePtr->valid );
    }
    if (result)
    {
        result = le_pack_UnpackDouble(bufferPtr,
                                                   &valuePtr->value );
    }
    return result;
}

static inline bool mangOH_bme680_PackReadingTemperature
(
    uint8_t **bufferPtr,
    const mangOH_bme680_ReadingTemperature_t *valuePtr
)
{
    __attribute__((unused))
    bool subResult, result = true;

    LE_ASSERT(valuePtr);
    subResult = le_pack_PackBool( bufferPtr,
                                                 valuePtr->valid );
    result = result && subResult;
    subResult = le_pack_PackDouble( bufferPtr,
                                                 valuePtr->value );
    result = result && subResult;

    return result;
}

static inline bool mangOH_bme680_UnpackReadingTemperature
(
    uint8_t **bufferPtr,
    mangOH_bme680_ReadingTemperature_t *valuePtr
)
{
    bool result = true;
    if (result)
    {
        result = le_pack_UnpackBool(bufferPtr,
                                                   &valuePtr->valid );
    }
    if (result)
    {
        result = le_pack_UnpackDouble(bufferPtr,
                                                   &valuePtr->value );
    }
    return result;
}

static inline bool mangOH_bme680_PackReadingHumidity
(
    uint8_t **bufferPtr,
    const mangOH_bme680_ReadingHumidity_t *valuePtr
)
{
    __attribute__((unused))
    bool subResult, result = true;

    LE_ASSERT(valuePtr);
    subResult = le_pack_PackBool( bufferPtr,
                                                 valuePtr->valid );
    result = result && subResult;
    subResult = le_pack_PackDouble( bufferPtr,
                                                 valuePtr->value );
    result = result && subResult;

    return result;
}

static inline bool mangOH_bme680_UnpackReadingHumidity
(
    uint8_t **bufferPtr,
    mangOH_bme680_ReadingHumidity_t *valuePtr
)
{
    bool result = true;
    if (result)
    {
        result = le_pack_UnpackBool(bufferPtr,
                                                   &valuePtr->valid );
    }
    if (result)
    {
        result = le_pack_UnpackDouble(bufferPtr,
                                                   &valuePtr->value );
    }
    return result;
}

static inline bool mangOH_bme680_PackReading
(
    uint8_t **bufferPtr,
    const mangOH_bme680_Reading_t *valuePtr
)
{
    __attribute__((unused))
    bool subResult, result = true;

    LE_ASSERT(valuePtr);
    subResult = le_pack_PackInt64( bufferPtr,
                                                 valuePtr->timestamp );
    result = result && subResult;
    subResult = mangOH_bme680_PackReadingIaq( bufferPtr,
                                                 &valuePtr->iaq );
    result = result && subResult;
    subResult = mangOH_bme680_PackReadingCo2Equivalent( bufferPtr,
                                                 &valuePtr->co2Equivalent );
    result = result && subResult;
    subResult = mangOH_bme680_PackReadingBreathVoc( bufferPtr,
                                                 &valuePtr->breathVoc );
    result = result && subResult;
    subResult = mangOH_bme680_PackReadingPressure( bufferPtr,
                                                 &valuePtr->pressure );
    result = result && subResult;
    subResult = mangOH_bme680_PackReadingTemperature( bufferPtr,
                                                 &valuePtr->temperature );
    result = result && subResult;
    subResult = mangOH_bme680_PackReadingHumidity( bufferPtr,
                                                 &valuePtr->humidity );
    result = result && subResult;

    return result;
}

static inline bool mangOH_bme680_UnpackReading
(
    uint8_t **bufferPtr,
    mangOH_bme680_Reading_t *valuePtr
)
{
    bool result = true;
    if (result)
    {
        result = le_pack_UnpackInt64(bufferPtr,
                                                   &valuePtr->timestamp );
    }
    if (result)
    {
        result = mangOH_bme680_UnpackReadingIaq(bufferPtr,
                                                   &valuePtr->iaq );
    }
    if (result)
    {
        result = mangOH_bme680_UnpackReadingCo2Equivalent(bufferPtr,
                                                   &valuePtr->co2Equivalent );
    }
    if (result)
    {
        result = mangOH_bme680_UnpackReadingBreathVoc(bufferPtr,
                                                   &valuePtr->breathVoc );
    }
    if (result)
    {
        result = mangOH_bme680_UnpackReadingPressure(bufferPtr,
                                                   &valuePtr->pressure );
    }
    if (result)
    {
        result = mangOH_bme680_UnpackReadingTemperature(bufferPtr,
                                                   &valuePtr->temperature );
    }
    if (result)
    {
        result = mangOH_bme680_UnpackReadingHumidity(bufferPtr,
                                                   &valuePtr->humidity );
    }
    return result;
}

#endif // MANGOH_BME680_MESSAGES_H_INCLUDE_GUARD
