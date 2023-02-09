/**
 * @file
 *
 * This is sample application which controls the IoT card detect LEDs based upon whether an IoT
 * card is present in the slot.  The applications demonstrates use of the le_gpio.api as
 * implemented by the GPIO expander.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless, Inc. Use of this work is subject to license.
 */

/* Legato Framework */
#include "legato.h"
#include "interfaces.h"


static void ConfigureGpios
(
    void
);
static void CardDetectEventHandler
(
    bool gpioState,
    void* contextPtr
);


//--------------------------------------------------------------------------------------------------
/**
 * Performs initial configuration of the GPIOs
 */
//--------------------------------------------------------------------------------------------------
static void ConfigureGpios
(
    void
)
{
    // Disable resistors on card detect GPIOs
    LE_FATAL_IF(
        mangoh_cardDetect0Gpio_DisableResistors() != LE_OK,
        "Couldn't disable resistors on card detect 0 input");
    LE_FATAL_IF(
        mangoh_cardDetect1Gpio_DisableResistors() != LE_OK,
        "Couldn't disable resistors on card detect 1 input");
    LE_FATAL_IF(
        mangoh_cardDetect2Gpio_DisableResistors() != LE_OK,
        "Couldn't disable resistors on card detect 2 input");

    // Configure card detect GPIOs as active low inputs since the card detect signal is inverted
    // low when a card is present
    LE_FATAL_IF(
        mangoh_cardDetect0Gpio_SetInput(MANGOH_CARDDETECT0GPIO_ACTIVE_LOW) != LE_OK,
        "Couldn't configure card detect 0 as an input");
    LE_FATAL_IF(
        mangoh_cardDetect1Gpio_SetInput(MANGOH_CARDDETECT1GPIO_ACTIVE_LOW) != LE_OK,
        "Couldn't configure card detect 1 as an input");
    LE_FATAL_IF(
        mangoh_cardDetect2Gpio_SetInput(MANGOH_CARDDETECT2GPIO_ACTIVE_LOW) != LE_OK,
        "Couldn't configure card detect 2 as an input");

    // Add event handlers for when the card detect inputs change
    mangoh_cardDetect0Gpio_AddChangeEventHandler(
        MANGOH_CARDDETECT0GPIO_EDGE_BOTH, &CardDetectEventHandler, (void *)0, 0);
    mangoh_cardDetect1Gpio_AddChangeEventHandler(
        MANGOH_CARDDETECT1GPIO_EDGE_BOTH, &CardDetectEventHandler, (void *)1, 0);
    mangoh_cardDetect2Gpio_AddChangeEventHandler(
        MANGOH_CARDDETECT2GPIO_EDGE_BOTH, &CardDetectEventHandler, (void *)2, 0);

    // Configure the card detect LED GPIOs as outputs and set their initial value based on whether
    // a card is present
    LE_FATAL_IF(
        mangoh_ledCardDetect0Gpio_SetPushPullOutput(
            MANGOH_LEDCARDDETECT0GPIO_ACTIVE_HIGH, mangoh_cardDetect0Gpio_Read()) != LE_OK,
        "Couldn't configure card detect LED 0 as a push pull output");
    LE_FATAL_IF(
        mangoh_ledCardDetect1Gpio_SetPushPullOutput(
            MANGOH_LEDCARDDETECT0GPIO_ACTIVE_HIGH, mangoh_cardDetect1Gpio_Read()) != LE_OK,
        "Couldn't configure card detect LED 1 as a push pull output");
    LE_FATAL_IF(
        mangoh_ledCardDetect2Gpio_SetPushPullOutput(
            MANGOH_LEDCARDDETECT0GPIO_ACTIVE_HIGH, mangoh_cardDetect2Gpio_Read()) != LE_OK,
        "Couldn't configure card detect LED 2 as a push pull output");
}

//--------------------------------------------------------------------------------------------------
/**
 * Turns on/off the card detect LED for each IoT slot as the card is inserted/removed
 */
//--------------------------------------------------------------------------------------------------
static void CardDetectEventHandler
(
    bool gpioState,   ///< [IN] true if the GPIO is active or false if it is inactive
    void* contextPtr  ///< [IN] A context pointer which is actually not a pointer, but rather is
                      ///  being used to store an integer representing the GPIO expander number
                      ///  which caused the event.
)
{
    // We treat contextPtr as an int which tells us which card detect pin the interrupt is for
    const int iotSlotOfEvent = (int)contextPtr;
    LE_INFO("Card detect event handler called for slot %d", iotSlotOfEvent);
    switch (iotSlotOfEvent)
    {
        case 0:
            LE_FATAL_IF(
                (gpioState ?
                    mangoh_ledCardDetect0Gpio_Activate() :
                    mangoh_ledCardDetect0Gpio_Deactivate()) != LE_OK,
                "Couldn't change output on card detect 0 LED");
            break;

        case 1:
            LE_FATAL_IF(
                (gpioState ?
                    mangoh_ledCardDetect1Gpio_Activate() :
                    mangoh_ledCardDetect1Gpio_Deactivate()) != LE_OK,
                "Couldn't change output on card detect 1 LED");
            break;

        case 2:
            LE_FATAL_IF(
                (gpioState ?
                    mangoh_ledCardDetect2Gpio_Activate() :
                    mangoh_ledCardDetect2Gpio_Deactivate()) != LE_OK,
                "Couldn't change output on card detect 2 LED");
            break;

        default:
            LE_FATAL("Handling an event for an unknown card detect input");
            break;
    }
}


COMPONENT_INIT
{
    LE_INFO(
        "This sample app demonstrates GPIO expander functionality by turning on and off the IoT "
        "slot LEDs as a module is inserted or removed\n");

    ConfigureGpios();
}
