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

#define LED_TIMER_IN_MS (500)

static bool LedOn;
static le_timer_Ref_t LedTimerRef;

//--------------------------------------------------------------------------------------------------
/**
 * Toggle the LED when the timer expires
 */
//--------------------------------------------------------------------------------------------------
static void LedTimer
(
    le_timer_Ref_t ledTimerRef
)
{
    if (LedOn)
    {
        mangoh_led_Deactivate();
        LedOn = false;
    }
    else
    {
        mangoh_led_Activate();
        LedOn = true;
    }
}

//--------------------------------------------------------------------------------------------------
/**
 * Turn the LED on and disable the timer while the button is pressed. When the  button is released,
 * turn off the LED and start the timer.
 */
//--------------------------------------------------------------------------------------------------
static void PushButtonHandler
(
    bool state, ///< true if the button is pressed
    void *ctx   ///< context pointer - not used
)
{
    if (state)
    {
        LE_DEBUG("turn on LED due to push button");
        le_timer_Stop(LedTimerRef);
        mangoh_led_Activate();
    }
    else
    {
        LE_DEBUG("turn off LED due to push button");
        mangoh_led_Deactivate();
        LedOn = false;
        le_timer_Start(LedTimerRef);
    }
}

//--------------------------------------------------------------------------------------------------
/**
 * Sets default configuration LED D750 as on
 */
//--------------------------------------------------------------------------------------------------
static void ConfigureGpios(void)
{
    // Set LED GPIO to output and initially turn the LED ON
    LE_FATAL_IF(
        mangoh_led_SetPushPullOutput(MANGOH_LED_ACTIVE_HIGH, true) != LE_OK,
        "Couldn't configure LED GPIO as a push pull output");
    LedOn = true;

    // Set the push-button GPIO as input
    LE_FATAL_IF(
        mangoh_button_SetInput(MANGOH_BUTTON_ACTIVE_LOW) != LE_OK,
        "Couldn't configure push button as input");
    mangoh_button_AddChangeEventHandler(MANGOH_BUTTON_EDGE_BOTH, PushButtonHandler, NULL, 0);
}

COMPONENT_INIT
{
    LedTimerRef = le_timer_Create("LED Timer");
    le_timer_SetMsInterval(LedTimerRef, LED_TIMER_IN_MS);
    le_timer_SetRepeat(LedTimerRef, 0);
    le_timer_SetHandler(LedTimerRef, LedTimer);

    ConfigureGpios();

    le_timer_Start(LedTimerRef);
}
