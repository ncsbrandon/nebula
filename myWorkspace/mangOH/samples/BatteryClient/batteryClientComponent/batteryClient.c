/**
 * @file
 *
 * Blinks the user controlled LED at 1Hz. If the push-button is pressed, the LED
 * will remain on until the push-button is released.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless, Inc. Use of this work is subject to license.
 */

#include "legato.h"
#include "interfaces.h"


void percentageHandler
(
    uint8_t percentage,
    uint8_t triggerPercentage,
    bool isHigh,
    void *context
)
{
    LE_INFO(
        "Received a %s battery level event.  Current level is %d%% and trigger percentage is %d%% and context=0x%p",
        isHigh ? "high" : "low", percentage, triggerPercentage, context);
}

void chargeHandler
(
    ma_battery_ChargingStatus_t chargeCondition,
    void *context
)
{
    LE_INFO(
        "Received a charge alarm level event.  Charge Alarm  is %d and context=0x%p",
        chargeCondition, context);
}

void healthHandler
(
    ma_battery_HealthStatus_t healthStatus,
    void *context
)
{
    LE_INFO(
        "Received a health alarm level event.  Health Alarm is %d and context=0x%p",
        healthStatus, context);
}

COMPONENT_INIT
{
    uint16_t percentremaining;
    uint16_t energyremaining;
    double currentvoltage, currenttemp;

    ma_adminbattery_SetTechnology("LIPO", 2000, 3700);

    ma_battery_HealthStatus_t health = ma_battery_GetHealthStatus();
    LE_INFO(" health = %d", health);

    ma_battery_ChargingStatus_t charge = ma_battery_GetChargingStatus();
    LE_INFO("charge = %d", charge);

    le_result_t result1 = ma_battery_GetVoltage(&currentvoltage);
    if (result1 == LE_OK)
    {
        LE_INFO(" current voltage = %lf in millivolts", currentvoltage);
    }

    le_result_t result2 = ma_battery_GetTemp(&currenttemp);
    if (result2 == LE_OK)
    {
        LE_INFO(" current temperature = %lf in Celcius", currenttemp);
    }

    le_result_t result3 = ma_battery_GetPercentRemaining(&percentremaining);
    if (result3 == LE_OK)
    {
        LE_INFO(" current percent = %d", percentremaining);
    }

    le_result_t result4 = ma_battery_GetChargeRemaining(&energyremaining);
    if (result4 == LE_OK)
    {
        LE_INFO(" current energy = %d", energyremaining);
    }


    ma_battery_AddLevelPercentageHandler(80, 90, percentageHandler, (void *)0xdeadbeef);

    ma_battery_AddChargingStatusChangeHandler(chargeHandler, NULL);

    ma_battery_AddHealthChangeHandler(healthHandler, NULL);
}
