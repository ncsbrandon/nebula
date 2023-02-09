/**
 * @file gpioPin.c
 *
 * This is sample Legato GPIO app by using mangoh_gpioExpander.api on mangOH project.
 *
 * It includes functions to enable/disable IOT card LED1, LED2, LED3, and LED4.
 * Those IOT LEDs will be on or off one by one
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless, Inc. Use of this work is subject to license.
 */

// TODO: Should the functions in this API bother returning a result?  It seems like it is a
//       critical system error if you lose control over the GPIO expander.

/* Legato Framework */
#include "legato.h"
#include "interfaces.h"


//--------------------------------------------------------------------------------------------------
/**
 * Disable UART 1
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_IotAllUart1Off
(
    void
)
{
    if (mangoh_gpioPinUart1Enable_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to disable UART 1");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Enable UART 1 on IoT slot 0
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_Iot0Uart1On
(
    void
)
{
    if (mangoh_gpioPinUart1Select_Activate() != LE_OK)
    {
        LE_ERROR("Failed to select IoT slot 0 for UART 1");
        return LE_FAULT;
    }

    if (mangoh_gpioPinUart1Enable_Activate() != LE_OK)
    {
        LE_ERROR("Failed to enable UART 1");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Enable UART 1 on IoT slot 1
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_Iot1Uart1On
(
    void
)
{
    if (mangoh_gpioPinUart1Select_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to select IoT slot 1 for UART 1");
        return LE_FAULT;
    }

    if (mangoh_gpioPinUart1Enable_Activate() != LE_OK)
    {
        LE_ERROR("Failed to enable UART 1");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Disable SPI
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_IotAllSpiOff
(
    void
)
{
    if (mangoh_gpioPinSpiEnable_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to disable SPI");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Enable SPI on IoT slot 0
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_Iot0Spi1On
(
    void
)
{
    if (mangoh_gpioPinSpiSelect_Activate() != LE_OK)
    {
        LE_ERROR("Failed to select IoT slot 0 for SPI");
        return LE_FAULT;
    }

    if (mangoh_gpioPinSpiEnable_Activate() != LE_OK)
    {
        LE_ERROR("Failed to enable SPI");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Mux Control: mangOH IOT1 SPI1 ON.
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_Iot1Spi1On
(
    void
)
{
    if (mangoh_gpioPinSpiSelect_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to select IoT slot 1 for SPI");
        return LE_FAULT;
    }

    if (mangoh_gpioPinSpiEnable_Activate() != LE_OK)
    {
        LE_ERROR("Failed to enable SPI");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Disable UART 2
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_IotAllUart2Off
(
    void
)
{
    if (mangoh_gpioPinUart2Enable_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to disable UART 2");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Enable UART 2 on IoT slot 2
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_Iot2Uart2On
(
    void
)
{
    if (mangoh_gpioPinUart2Select_Activate() != LE_OK)
    {
        LE_ERROR("Failed to select IoT slot 2 for UART 2");
        return LE_FAULT;
    }

    if (mangoh_gpioPinUart2Enable_Activate() != LE_OK)
    {
        LE_ERROR("Failed to enable UART 2");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Enable UART 2 on the debug port
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_Uart2DebugOn
(
    void
)
{
    if (mangoh_gpioPinUart2Select_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to select the debug port for UART 2");
        return LE_FAULT;
    }

    if (mangoh_gpioPinUart2Enable_Activate() != LE_OK)
    {
        LE_ERROR("Failed to enable UART 2");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Connect the SDIO interface to the MicroSD card slot
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_SdioSelMicroSd
(
    void
)
{
    if (mangoh_gpioPinSdioSelect_Activate() != LE_OK)
    {
        LE_ERROR("Failed to select MicroSD slot for SDIO");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Connect the SDIO interface to IoT slot 0
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_SdioSelIot0
(
    void
)
{
    if (mangoh_gpioPinSdioSelect_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to select IoT slot 0 for SDIO");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Disable the audio path
 *
 * @return
 *      LE_OK on success or LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_AudioDisable
(
    void
)
{
    if (mangoh_gpioPinPcmEnable_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to disable PCM");
        return LE_FAULT;
    }

    if (mangoh_gpioPinPcmAnalogSelect_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to select off-chip codec location");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Route audio via a codec installed in IoT slot 0
 *
 * @return
 *      LE_OK on success or LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_AudioSelectIot0Codec
(
    void
)
{
    if (mangoh_gpioPinPcmSelect_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to select IoT slot 0 for PCM");
        return LE_FAULT;
    }

    if (mangoh_gpioPinPcmAnalogSelect_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to select off-chip codec location");
        return LE_FAULT;
    }

    if (mangoh_gpioPinPcmEnable_Activate() != LE_OK)
    {
        LE_ERROR("Failed to enable PCM");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Route audio via the codec on the mangOH board
 *
 * @return
 *      LE_OK on success or LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_AudioSelectOnboardCodec
(
    void
)
{
    if (mangoh_gpioPinPcmSelect_Activate() != LE_OK)
    {
        LE_ERROR("Failed to select onboard for PCM");
        return LE_FAULT;
    }

    if (mangoh_gpioPinPcmAnalogSelect_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to select off-chip codec location");
        return LE_FAULT;
    }

    if (mangoh_gpioPinPcmEnable_Activate() != LE_OK)
    {
        LE_ERROR("Failed to enable PCM");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Route audio via a codec internal to the CF3 module
 *
 * @return
 *      LE_OK on success or LE_FAULT on failure
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_AudioSelectInternalCodec
(
    void
)
{
    if (mangoh_gpioPinPcmEnable_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to disable PCM");
        return LE_FAULT;
    }

    if (mangoh_gpioPinPcmAnalogSelect_Activate() != LE_OK)
    {
        LE_ERROR("Failed to select on-chip codec location");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Take IoT slot 0 card out of reset
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_IotSlot0DeassertReset
(
    void
)
{
    if (mangoh_gpioPinIot0Reset_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to take IoT slot 0 out of reset");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Take IoT slot 1 card out of reset
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_IotSlot1DeassertReset
(
    void
)
{
    if (mangoh_gpioPinIot1Reset_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to take IoT slot 1 out of reset");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Take IoT slot 2 card out of reset
 *
 * @return
 *      - LE_FAULT
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_IotSlot2DeassertReset
(
    void
)
{
    if (mangoh_gpioPinIot2Reset_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to take IoT slot 2 out of reset");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Put Arduino in reset state
 *
 * @return
 *      - LE_FAULT
 *      - LE_UNSUPPORTED if the board doesn't have an Arduino
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_ArduinoAssertReset
(
)
{
    if (mangoh_gpioPinArduinoReset_Activate() != LE_OK)
    {
        LE_ERROR("Failed to put Arduino reset pin low");
        return LE_FAULT;
    }

    return LE_OK;
}


//--------------------------------------------------------------------------------------------------
/**
 * Put Arduino out of reset state
 *
 * @return
 *      - LE_FAULT
 *      - LE_UNSUPPORTED if the board doesn't have an Arduino
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_ArduinoDeassertReset
(
)
{
    if (mangoh_gpioPinArduinoReset_Deactivate() != LE_OK)
    {
        LE_ERROR("Failed to put Arduino reset pin high");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Perform Arduino reset
 *
 * @return
 *      - LE_FAULT
 *      - LE_UNSUPPORTED if the board doesn't have an Arduino
 *      - LE_OK
 */
//--------------------------------------------------------------------------------------------------
le_result_t mangoh_muxCtrl_ArduinoReset
(
    void
)
{
    le_result_t res;

    res = mangoh_muxCtrl_ArduinoAssertReset();
    if(res != LE_OK)
    {
        return res;
    }

    usleep(300);

    res = mangoh_muxCtrl_ArduinoDeassertReset();

    return res;
}

COMPONENT_INIT
{
    LE_INFO(
        "This is sample mangOH Mux Control API service by using mangoh_gpioExpander.api and "
        "mangoh_muxCtrl.api\n");

    mangoh_gpioPinUart1Enable_SetPushPullOutput(MANGOH_GPIOPINUART1ENABLE_ACTIVE_LOW, false);
    mangoh_gpioPinUart1Select_SetPushPullOutput(MANGOH_GPIOPINUART1SELECT_ACTIVE_HIGH, false);
    mangoh_gpioPinSpiEnable_SetPushPullOutput(MANGOH_GPIOPINSPIENABLE_ACTIVE_LOW, false);
    mangoh_gpioPinSpiSelect_SetPushPullOutput(MANGOH_GPIOPINSPISELECT_ACTIVE_HIGH, false);
    mangoh_gpioPinUart2Enable_SetPushPullOutput(MANGOH_GPIOPINUART2ENABLE_ACTIVE_LOW, true);
    mangoh_gpioPinUart2Select_SetPushPullOutput(MANGOH_GPIOPINUART2SELECT_ACTIVE_HIGH, false);
    mangoh_gpioPinPcmEnable_SetPushPullOutput(MANGOH_GPIOPINPCMENABLE_ACTIVE_LOW, false);
    mangoh_gpioPinPcmSelect_SetPushPullOutput(MANGOH_GPIOPINPCMSELECT_ACTIVE_HIGH, false);
    mangoh_gpioPinSdioSelect_SetPushPullOutput(MANGOH_GPIOPINSDIOSELECT_ACTIVE_HIGH, true);
    mangoh_gpioPinPcmAnalogSelect_SetPushPullOutput(
        MANGOH_GPIOPINPCMANALOGSELECT_ACTIVE_HIGH, false);
    mangoh_gpioPinIot0Reset_SetPushPullOutput(MANGOH_GPIOPINIOT0RESET_ACTIVE_LOW, true);
    mangoh_gpioPinIot1Reset_SetPushPullOutput(MANGOH_GPIOPINIOT1RESET_ACTIVE_LOW, true);
    mangoh_gpioPinIot2Reset_SetPushPullOutput(MANGOH_GPIOPINIOT2RESET_ACTIVE_LOW, true);
    mangoh_gpioPinArduinoReset_SetPushPullOutput(MANGOH_GPIOPINARDUINORESET_ACTIVE_LOW, true);
}
