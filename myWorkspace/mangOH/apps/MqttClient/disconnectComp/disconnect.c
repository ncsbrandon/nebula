//--------------------------------------------------------------------------------------------------
/**
 * @file disconnect.c
 *
 * This component is used to start/stop mqttClient and to send mqtt messages to AirVantage.
 *
 * <hr>
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 *
 */
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"


//--------------------------------------------------------------------------------------------------
/**
 * App init.
 *
 */
//--------------------------------------------------------------------------------------------------
COMPONENT_INIT
{
    LE_INFO("Close MQTT session");
    mqtt_Disconnect();

    exit(EXIT_SUCCESS);
}
