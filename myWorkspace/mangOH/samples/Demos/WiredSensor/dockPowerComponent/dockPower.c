//--------------------------------------------------------------------------------------------------
/**
 * This sample app reads state of  IoT1_GPIO3 (gpio32)  and determines whether dock power is
 * connected or not.
 */
//--------------------------------------------------------------------------------------------------
#include "legato.h"
#include "interfaces.h"

#define KEY_DOCK_POWER      "a.wr.read.power"


//--------------------------------------------------------------------------------------------------
/**
 * Performs initial configuration of the CF3 gpio (IoT1_GPIO3)
 */
//--------------------------------------------------------------------------------------------------
static void ConfigureSensorGpio
(
    void
)
{
    LE_FATAL_IF(le_sensorGpio_EnablePullDown() != LE_OK,
                "Couldn't configure gpio for dock power as pull down");

    // Configure IoT1_GPIO2 as input and set its initial value as high
    LE_FATAL_IF(le_sensorGpio_SetInput(LE_SENSORGPIO_ACTIVE_HIGH) != LE_OK,
                "Couldn't configure cf3 gpio as default input high");
}

//--------------------------------------------------------------------------------------------------
/**
 * Dock power sensor change handler
 */
//--------------------------------------------------------------------------------------------------
static  void DockPowerSensorChangeHandler
(
    bool state,
    void *ctx
)
{
    const int32_t now = time(NULL);
    dataRouter_WriteBoolean(KEY_DOCK_POWER, state, now);
}

//--------------------------------------------------------------------------------------------------
/**
 * Main program starts here
 */
//--------------------------------------------------------------------------------------------------
COMPONENT_INIT
{
    LE_INFO("=============== Dock Power application has started");

    dataRouter_SessionStart("", "", false, DATAROUTER_CACHE);

    ConfigureSensorGpio();

    // Fake a transition immediately to publish the current value
    DockPowerSensorChangeHandler(le_sensorGpio_Read(), NULL);
    le_sensorGpio_AddChangeEventHandler(LE_SENSORGPIO_EDGE_BOTH,
                                        DockPowerSensorChangeHandler,
                                        NULL,
                                        0);
}
