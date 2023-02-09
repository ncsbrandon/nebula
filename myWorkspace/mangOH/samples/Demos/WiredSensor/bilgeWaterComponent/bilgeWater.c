//--------------------------------------------------------------------------------------------------
/**
 * This sample app reads state of  IoT1_GPIO2 (gpio7) and determines state of bilge.
 */
//--------------------------------------------------------------------------------------------------
#include "legato.h"
#include "interfaces.h"

#define KEY_BILGE_STATE      "a.wr.read.water"


//--------------------------------------------------------------------------------------------------
/**
 * Performs initial configuration of the CF3 gpio (IoT1_GPIO2)
 */
//--------------------------------------------------------------------------------------------------
static void ConfigureSensorGpio
(
    void
)
{
    // Configure IoT1_GPIO2 as input and set its initial value as high
    LE_FATAL_IF(le_sensorGpio_EnablePullDown() != LE_OK,
                "Couldn't configure gpio for water as pull down");
    LE_FATAL_IF(le_sensorGpio_SetInput(LE_SENSORGPIO_ACTIVE_HIGH) != LE_OK,
                "Couldn't configure cf3 gpio as default input high");
}

//--------------------------------------------------------------------------------------------------
/**
 * Bilge water sensor change handler
 */
//--------------------------------------------------------------------------------------------------
static  void BilgeWaterSensorChangeHandler
(
    bool state,
    void *ctx
)
{
    const int32_t now = time(NULL);
    dataRouter_WriteBoolean(KEY_BILGE_STATE, state, now);
}

//--------------------------------------------------------------------------------------------------
/**
 * Main program starts here
 */
//--------------------------------------------------------------------------------------------------
COMPONENT_INIT
{
    LE_INFO("=============== Bilge Water application has started");

    dataRouter_SessionStart("", "", false, DATAROUTER_CACHE);

    ConfigureSensorGpio();

    // Fake a transition immediately to publish the current value
    BilgeWaterSensorChangeHandler(le_sensorGpio_Read(), NULL);
    le_sensorGpio_AddChangeEventHandler(LE_SENSORGPIO_EDGE_BOTH,
                                        BilgeWaterSensorChangeHandler,
                                        NULL,
                                        0);
}
