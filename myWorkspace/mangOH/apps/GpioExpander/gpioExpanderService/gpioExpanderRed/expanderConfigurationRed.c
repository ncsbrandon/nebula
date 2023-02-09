/**
 * @file
 *
 * Prototype GPIO Expander API interface.
 * The GPIO API implementation of GPIO Expander 1, 2, 3 on mangOH board
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */

#include "legato.h"
#include "interfaces.h"
#include "gpioExpander.h"


#define I2C_SX1509_GPIO_EXPANDER_ADDR       0x3E

#define EXPANDER_BUS (PrimaryI2cBusNum + 3)

static uint8_t PrimaryI2cBusNum;

// .i2cBus must be populated in COMPONENT_INIT once PrimaryI2cBusNum is discovered.
static gpioExpander_Identifier_t GpioExpander =
{
    .i2cAddr = I2C_SX1509_GPIO_EXPANDER_ADDR,
};

// Note: will be zeroed by spec, so no need to explicitly initialize the values
static gpioExpander_HandlerRecord_t handlerRecords[16];


//--------------------------------------------------------------------------------------------------
/**
 * Interrupt handler for GPIO expander.
 */
//--------------------------------------------------------------------------------------------------
static void gpioExpander_ExpanderInterruptHandler
(
    bool state,       ///< Current state of the GPIO - true: active, false: inactive
    void *contextPtr  ///< Unused
)
{
    gpioExpander_GenericInterruptHandler(&GpioExpander, handlerRecords);
}


//--------------------------------------------------------------------------------------------------
/**
 * Initialize the three GPIO expanders and configure the interrupts.  GPIO expander #2's interrupt
 * pin is connected to a GPIO in the CF3, but GPIO expander #1 and #3 have their interrupt pins
 * connected to a GPIO of expander #2.
 *
 * @todo
 *      This is not the right place to be controlling the I2C switch.  This needs to be managed
 *      centrally.
 */
//--------------------------------------------------------------------------------------------------
COMPONENT_INIT
{
    LE_FATAL_IF(
        gpioExpander_DiscoverPrimaryI2cBusNum(&PrimaryI2cBusNum) != LE_OK,
        "Couldn't determine the primary I2C bus");
    GpioExpander.i2cBus = EXPANDER_BUS;

    // Reset the GPIO expander
    gpioExpander_Reset(&GpioExpander);

    // Configure the interrupt for expander
    expanderInterrupt_EnablePullUp();
    expanderInterrupt_SetInput(EXPANDERINTERRUPT_ACTIVE_LOW);
    expanderInterrupt_AddChangeEventHandler(
        EXPANDERINTERRUPT_EDGE_RISING,
        &gpioExpander_ExpanderInterruptHandler,
        NULL,
        100);
}

// ----- BEGIN GENERATED CODE

// GPIO expander GPIO 0
le_result_t mangoh_gpioExpPin0_SetInput
(
    mangoh_gpioExpPin0_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 0, polarity);
}

le_result_t mangoh_gpioExpPin0_SetPushPullOutput
(
    mangoh_gpioExpPin0_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 0, polarity, value);
}

le_result_t mangoh_gpioExpPin0_SetTriStateOutput
(
    mangoh_gpioExpPin0_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 0, polarity);
}

le_result_t mangoh_gpioExpPin0_SetOpenDrainOutput
(
    mangoh_gpioExpPin0_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 0, polarity, value);
}

le_result_t mangoh_gpioExpPin0_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 0);
}

le_result_t mangoh_gpioExpPin0_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 0);
}

le_result_t mangoh_gpioExpPin0_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 0);
}

le_result_t mangoh_gpioExpPin0_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 0);
}

le_result_t mangoh_gpioExpPin0_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 0);
}

le_result_t mangoh_gpioExpPin0_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 0);
}

bool mangoh_gpioExpPin0_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 0);
}

le_result_t mangoh_gpioExpPin0_SetEdgeSense
(
    mangoh_gpioExpPin0_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 0, trigger);
}

mangoh_gpioExpPin0_Edge_t mangoh_gpioExpPin0_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 0);
}

le_result_t mangoh_gpioExpPin0_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 0);
}

bool mangoh_gpioExpPin0_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 0);
}

bool mangoh_gpioExpPin0_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 0);
}

mangoh_gpioExpPin0_Polarity_t mangoh_gpioExpPin0_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 0);
}

bool mangoh_gpioExpPin0_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 0);
}

mangoh_gpioExpPin0_PullUpDown_t mangoh_gpioExpPin0_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 0);
}

mangoh_gpioExpPin0_ChangeEventHandlerRef_t mangoh_gpioExpPin0_AddChangeEventHandler
(
    mangoh_gpioExpPin0_Edge_t trigger,
    mangoh_gpioExpPin0_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin0_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        0,
        &handlerRecords[0],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin0_RemoveChangeEventHandler
(
    mangoh_gpioExpPin0_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        0,
        &handlerRecords[0],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 1
le_result_t mangoh_gpioExpPin1_SetInput
(
    mangoh_gpioExpPin1_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 1, polarity);
}

le_result_t mangoh_gpioExpPin1_SetPushPullOutput
(
    mangoh_gpioExpPin1_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 1, polarity, value);
}

le_result_t mangoh_gpioExpPin1_SetTriStateOutput
(
    mangoh_gpioExpPin1_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 1, polarity);
}

le_result_t mangoh_gpioExpPin1_SetOpenDrainOutput
(
    mangoh_gpioExpPin1_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 1, polarity, value);
}

le_result_t mangoh_gpioExpPin1_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 1);
}

le_result_t mangoh_gpioExpPin1_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 1);
}

le_result_t mangoh_gpioExpPin1_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 1);
}

le_result_t mangoh_gpioExpPin1_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 1);
}

le_result_t mangoh_gpioExpPin1_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 1);
}

le_result_t mangoh_gpioExpPin1_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 1);
}

bool mangoh_gpioExpPin1_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 1);
}

le_result_t mangoh_gpioExpPin1_SetEdgeSense
(
    mangoh_gpioExpPin1_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 1, trigger);
}

mangoh_gpioExpPin1_Edge_t mangoh_gpioExpPin1_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 1);
}

le_result_t mangoh_gpioExpPin1_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 1);
}

bool mangoh_gpioExpPin1_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 1);
}

bool mangoh_gpioExpPin1_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 1);
}

mangoh_gpioExpPin1_Polarity_t mangoh_gpioExpPin1_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 1);
}

bool mangoh_gpioExpPin1_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 1);
}

mangoh_gpioExpPin1_PullUpDown_t mangoh_gpioExpPin1_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 1);
}

mangoh_gpioExpPin1_ChangeEventHandlerRef_t mangoh_gpioExpPin1_AddChangeEventHandler
(
    mangoh_gpioExpPin1_Edge_t trigger,
    mangoh_gpioExpPin1_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin1_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        1,
        &handlerRecords[1],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin1_RemoveChangeEventHandler
(
    mangoh_gpioExpPin1_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        1,
        &handlerRecords[1],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 2
le_result_t mangoh_gpioExpPin2_SetInput
(
    mangoh_gpioExpPin2_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 2, polarity);
}

le_result_t mangoh_gpioExpPin2_SetPushPullOutput
(
    mangoh_gpioExpPin2_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 2, polarity, value);
}

le_result_t mangoh_gpioExpPin2_SetTriStateOutput
(
    mangoh_gpioExpPin2_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 2, polarity);
}

le_result_t mangoh_gpioExpPin2_SetOpenDrainOutput
(
    mangoh_gpioExpPin2_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 2, polarity, value);
}

le_result_t mangoh_gpioExpPin2_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 2);
}

le_result_t mangoh_gpioExpPin2_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 2);
}

le_result_t mangoh_gpioExpPin2_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 2);
}

le_result_t mangoh_gpioExpPin2_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 2);
}

le_result_t mangoh_gpioExpPin2_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 2);
}

le_result_t mangoh_gpioExpPin2_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 2);
}

bool mangoh_gpioExpPin2_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 2);
}

le_result_t mangoh_gpioExpPin2_SetEdgeSense
(
    mangoh_gpioExpPin2_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 2, trigger);
}

mangoh_gpioExpPin2_Edge_t mangoh_gpioExpPin2_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 2);
}

le_result_t mangoh_gpioExpPin2_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 2);
}

bool mangoh_gpioExpPin2_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 2);
}

bool mangoh_gpioExpPin2_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 2);
}

mangoh_gpioExpPin2_Polarity_t mangoh_gpioExpPin2_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 2);
}

bool mangoh_gpioExpPin2_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 2);
}

mangoh_gpioExpPin2_PullUpDown_t mangoh_gpioExpPin2_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 2);
}

mangoh_gpioExpPin2_ChangeEventHandlerRef_t mangoh_gpioExpPin2_AddChangeEventHandler
(
    mangoh_gpioExpPin2_Edge_t trigger,
    mangoh_gpioExpPin2_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin2_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        2,
        &handlerRecords[2],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin2_RemoveChangeEventHandler
(
    mangoh_gpioExpPin2_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        2,
        &handlerRecords[2],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 3
le_result_t mangoh_gpioExpPin3_SetInput
(
    mangoh_gpioExpPin3_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 3, polarity);
}

le_result_t mangoh_gpioExpPin3_SetPushPullOutput
(
    mangoh_gpioExpPin3_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 3, polarity, value);
}

le_result_t mangoh_gpioExpPin3_SetTriStateOutput
(
    mangoh_gpioExpPin3_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 3, polarity);
}

le_result_t mangoh_gpioExpPin3_SetOpenDrainOutput
(
    mangoh_gpioExpPin3_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 3, polarity, value);
}

le_result_t mangoh_gpioExpPin3_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 3);
}

le_result_t mangoh_gpioExpPin3_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 3);
}

le_result_t mangoh_gpioExpPin3_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 3);
}

le_result_t mangoh_gpioExpPin3_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 3);
}

le_result_t mangoh_gpioExpPin3_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 3);
}

le_result_t mangoh_gpioExpPin3_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 3);
}

bool mangoh_gpioExpPin3_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 3);
}

le_result_t mangoh_gpioExpPin3_SetEdgeSense
(
    mangoh_gpioExpPin3_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 3, trigger);
}

mangoh_gpioExpPin3_Edge_t mangoh_gpioExpPin3_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 3);
}

le_result_t mangoh_gpioExpPin3_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 3);
}

bool mangoh_gpioExpPin3_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 3);
}

bool mangoh_gpioExpPin3_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 3);
}

mangoh_gpioExpPin3_Polarity_t mangoh_gpioExpPin3_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 3);
}

bool mangoh_gpioExpPin3_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 3);
}

mangoh_gpioExpPin3_PullUpDown_t mangoh_gpioExpPin3_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 3);
}

mangoh_gpioExpPin3_ChangeEventHandlerRef_t mangoh_gpioExpPin3_AddChangeEventHandler
(
    mangoh_gpioExpPin3_Edge_t trigger,
    mangoh_gpioExpPin3_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin3_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        3,
        &handlerRecords[3],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin3_RemoveChangeEventHandler
(
    mangoh_gpioExpPin3_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        3,
        &handlerRecords[3],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 4
le_result_t mangoh_gpioExpPin4_SetInput
(
    mangoh_gpioExpPin4_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 4, polarity);
}

le_result_t mangoh_gpioExpPin4_SetPushPullOutput
(
    mangoh_gpioExpPin4_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 4, polarity, value);
}

le_result_t mangoh_gpioExpPin4_SetTriStateOutput
(
    mangoh_gpioExpPin4_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 4, polarity);
}

le_result_t mangoh_gpioExpPin4_SetOpenDrainOutput
(
    mangoh_gpioExpPin4_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 4, polarity, value);
}

le_result_t mangoh_gpioExpPin4_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 4);
}

le_result_t mangoh_gpioExpPin4_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 4);
}

le_result_t mangoh_gpioExpPin4_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 4);
}

le_result_t mangoh_gpioExpPin4_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 4);
}

le_result_t mangoh_gpioExpPin4_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 4);
}

le_result_t mangoh_gpioExpPin4_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 4);
}

bool mangoh_gpioExpPin4_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 4);
}

le_result_t mangoh_gpioExpPin4_SetEdgeSense
(
    mangoh_gpioExpPin4_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 4, trigger);
}

mangoh_gpioExpPin4_Edge_t mangoh_gpioExpPin4_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 4);
}

le_result_t mangoh_gpioExpPin4_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 4);
}

bool mangoh_gpioExpPin4_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 4);
}

bool mangoh_gpioExpPin4_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 4);
}

mangoh_gpioExpPin4_Polarity_t mangoh_gpioExpPin4_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 4);
}

bool mangoh_gpioExpPin4_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 4);
}

mangoh_gpioExpPin4_PullUpDown_t mangoh_gpioExpPin4_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 4);
}

mangoh_gpioExpPin4_ChangeEventHandlerRef_t mangoh_gpioExpPin4_AddChangeEventHandler
(
    mangoh_gpioExpPin4_Edge_t trigger,
    mangoh_gpioExpPin4_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin4_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        4,
        &handlerRecords[4],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin4_RemoveChangeEventHandler
(
    mangoh_gpioExpPin4_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        4,
        &handlerRecords[4],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 5
le_result_t mangoh_gpioExpPin5_SetInput
(
    mangoh_gpioExpPin5_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 5, polarity);
}

le_result_t mangoh_gpioExpPin5_SetPushPullOutput
(
    mangoh_gpioExpPin5_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 5, polarity, value);
}

le_result_t mangoh_gpioExpPin5_SetTriStateOutput
(
    mangoh_gpioExpPin5_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 5, polarity);
}

le_result_t mangoh_gpioExpPin5_SetOpenDrainOutput
(
    mangoh_gpioExpPin5_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 5, polarity, value);
}

le_result_t mangoh_gpioExpPin5_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 5);
}

le_result_t mangoh_gpioExpPin5_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 5);
}

le_result_t mangoh_gpioExpPin5_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 5);
}

le_result_t mangoh_gpioExpPin5_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 5);
}

le_result_t mangoh_gpioExpPin5_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 5);
}

le_result_t mangoh_gpioExpPin5_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 5);
}

bool mangoh_gpioExpPin5_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 5);
}

le_result_t mangoh_gpioExpPin5_SetEdgeSense
(
    mangoh_gpioExpPin5_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 5, trigger);
}

mangoh_gpioExpPin5_Edge_t mangoh_gpioExpPin5_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 5);
}

le_result_t mangoh_gpioExpPin5_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 5);
}

bool mangoh_gpioExpPin5_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 5);
}

bool mangoh_gpioExpPin5_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 5);
}

mangoh_gpioExpPin5_Polarity_t mangoh_gpioExpPin5_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 5);
}

bool mangoh_gpioExpPin5_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 5);
}

mangoh_gpioExpPin5_PullUpDown_t mangoh_gpioExpPin5_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 5);
}

mangoh_gpioExpPin5_ChangeEventHandlerRef_t mangoh_gpioExpPin5_AddChangeEventHandler
(
    mangoh_gpioExpPin5_Edge_t trigger,
    mangoh_gpioExpPin5_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin5_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        5,
        &handlerRecords[5],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin5_RemoveChangeEventHandler
(
    mangoh_gpioExpPin5_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        5,
        &handlerRecords[5],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 6
le_result_t mangoh_gpioExpPin6_SetInput
(
    mangoh_gpioExpPin6_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 6, polarity);
}

le_result_t mangoh_gpioExpPin6_SetPushPullOutput
(
    mangoh_gpioExpPin6_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 6, polarity, value);
}

le_result_t mangoh_gpioExpPin6_SetTriStateOutput
(
    mangoh_gpioExpPin6_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 6, polarity);
}

le_result_t mangoh_gpioExpPin6_SetOpenDrainOutput
(
    mangoh_gpioExpPin6_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 6, polarity, value);
}

le_result_t mangoh_gpioExpPin6_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 6);
}

le_result_t mangoh_gpioExpPin6_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 6);
}

le_result_t mangoh_gpioExpPin6_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 6);
}

le_result_t mangoh_gpioExpPin6_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 6);
}

le_result_t mangoh_gpioExpPin6_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 6);
}

le_result_t mangoh_gpioExpPin6_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 6);
}

bool mangoh_gpioExpPin6_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 6);
}

le_result_t mangoh_gpioExpPin6_SetEdgeSense
(
    mangoh_gpioExpPin6_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 6, trigger);
}

mangoh_gpioExpPin6_Edge_t mangoh_gpioExpPin6_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 6);
}

le_result_t mangoh_gpioExpPin6_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 6);
}

bool mangoh_gpioExpPin6_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 6);
}

bool mangoh_gpioExpPin6_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 6);
}

mangoh_gpioExpPin6_Polarity_t mangoh_gpioExpPin6_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 6);
}

bool mangoh_gpioExpPin6_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 6);
}

mangoh_gpioExpPin6_PullUpDown_t mangoh_gpioExpPin6_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 6);
}

mangoh_gpioExpPin6_ChangeEventHandlerRef_t mangoh_gpioExpPin6_AddChangeEventHandler
(
    mangoh_gpioExpPin6_Edge_t trigger,
    mangoh_gpioExpPin6_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin6_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        6,
        &handlerRecords[6],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin6_RemoveChangeEventHandler
(
    mangoh_gpioExpPin6_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        6,
        &handlerRecords[6],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 7
le_result_t mangoh_gpioExpPin7_SetInput
(
    mangoh_gpioExpPin7_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 7, polarity);
}

le_result_t mangoh_gpioExpPin7_SetPushPullOutput
(
    mangoh_gpioExpPin7_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 7, polarity, value);
}

le_result_t mangoh_gpioExpPin7_SetTriStateOutput
(
    mangoh_gpioExpPin7_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 7, polarity);
}

le_result_t mangoh_gpioExpPin7_SetOpenDrainOutput
(
    mangoh_gpioExpPin7_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 7, polarity, value);
}

le_result_t mangoh_gpioExpPin7_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 7);
}

le_result_t mangoh_gpioExpPin7_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 7);
}

le_result_t mangoh_gpioExpPin7_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 7);
}

le_result_t mangoh_gpioExpPin7_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 7);
}

le_result_t mangoh_gpioExpPin7_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 7);
}

le_result_t mangoh_gpioExpPin7_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 7);
}

bool mangoh_gpioExpPin7_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 7);
}

le_result_t mangoh_gpioExpPin7_SetEdgeSense
(
    mangoh_gpioExpPin7_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 7, trigger);
}

mangoh_gpioExpPin7_Edge_t mangoh_gpioExpPin7_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 7);
}

le_result_t mangoh_gpioExpPin7_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 7);
}

bool mangoh_gpioExpPin7_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 7);
}

bool mangoh_gpioExpPin7_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 7);
}

mangoh_gpioExpPin7_Polarity_t mangoh_gpioExpPin7_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 7);
}

bool mangoh_gpioExpPin7_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 7);
}

mangoh_gpioExpPin7_PullUpDown_t mangoh_gpioExpPin7_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 7);
}

mangoh_gpioExpPin7_ChangeEventHandlerRef_t mangoh_gpioExpPin7_AddChangeEventHandler
(
    mangoh_gpioExpPin7_Edge_t trigger,
    mangoh_gpioExpPin7_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin7_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        7,
        &handlerRecords[7],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin7_RemoveChangeEventHandler
(
    mangoh_gpioExpPin7_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        7,
        &handlerRecords[7],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 8
le_result_t mangoh_gpioExpPin8_SetInput
(
    mangoh_gpioExpPin8_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 8, polarity);
}

le_result_t mangoh_gpioExpPin8_SetPushPullOutput
(
    mangoh_gpioExpPin8_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 8, polarity, value);
}

le_result_t mangoh_gpioExpPin8_SetTriStateOutput
(
    mangoh_gpioExpPin8_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 8, polarity);
}

le_result_t mangoh_gpioExpPin8_SetOpenDrainOutput
(
    mangoh_gpioExpPin8_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 8, polarity, value);
}

le_result_t mangoh_gpioExpPin8_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 8);
}

le_result_t mangoh_gpioExpPin8_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 8);
}

le_result_t mangoh_gpioExpPin8_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 8);
}

le_result_t mangoh_gpioExpPin8_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 8);
}

le_result_t mangoh_gpioExpPin8_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 8);
}

le_result_t mangoh_gpioExpPin8_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 8);
}

bool mangoh_gpioExpPin8_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 8);
}

le_result_t mangoh_gpioExpPin8_SetEdgeSense
(
    mangoh_gpioExpPin8_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 8, trigger);
}

mangoh_gpioExpPin8_Edge_t mangoh_gpioExpPin8_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 8);
}

le_result_t mangoh_gpioExpPin8_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 8);
}

bool mangoh_gpioExpPin8_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 8);
}

bool mangoh_gpioExpPin8_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 8);
}

mangoh_gpioExpPin8_Polarity_t mangoh_gpioExpPin8_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 8);
}

bool mangoh_gpioExpPin8_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 8);
}

mangoh_gpioExpPin8_PullUpDown_t mangoh_gpioExpPin8_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 8);
}

mangoh_gpioExpPin8_ChangeEventHandlerRef_t mangoh_gpioExpPin8_AddChangeEventHandler
(
    mangoh_gpioExpPin8_Edge_t trigger,
    mangoh_gpioExpPin8_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin8_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        8,
        &handlerRecords[8],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin8_RemoveChangeEventHandler
(
    mangoh_gpioExpPin8_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        8,
        &handlerRecords[8],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 9
le_result_t mangoh_gpioExpPin9_SetInput
(
    mangoh_gpioExpPin9_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 9, polarity);
}

le_result_t mangoh_gpioExpPin9_SetPushPullOutput
(
    mangoh_gpioExpPin9_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 9, polarity, value);
}

le_result_t mangoh_gpioExpPin9_SetTriStateOutput
(
    mangoh_gpioExpPin9_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 9, polarity);
}

le_result_t mangoh_gpioExpPin9_SetOpenDrainOutput
(
    mangoh_gpioExpPin9_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 9, polarity, value);
}

le_result_t mangoh_gpioExpPin9_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 9);
}

le_result_t mangoh_gpioExpPin9_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 9);
}

le_result_t mangoh_gpioExpPin9_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 9);
}

le_result_t mangoh_gpioExpPin9_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 9);
}

le_result_t mangoh_gpioExpPin9_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 9);
}

le_result_t mangoh_gpioExpPin9_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 9);
}

bool mangoh_gpioExpPin9_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 9);
}

le_result_t mangoh_gpioExpPin9_SetEdgeSense
(
    mangoh_gpioExpPin9_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 9, trigger);
}

mangoh_gpioExpPin9_Edge_t mangoh_gpioExpPin9_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 9);
}

le_result_t mangoh_gpioExpPin9_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 9);
}

bool mangoh_gpioExpPin9_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 9);
}

bool mangoh_gpioExpPin9_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 9);
}

mangoh_gpioExpPin9_Polarity_t mangoh_gpioExpPin9_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 9);
}

bool mangoh_gpioExpPin9_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 9);
}

mangoh_gpioExpPin9_PullUpDown_t mangoh_gpioExpPin9_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 9);
}

mangoh_gpioExpPin9_ChangeEventHandlerRef_t mangoh_gpioExpPin9_AddChangeEventHandler
(
    mangoh_gpioExpPin9_Edge_t trigger,
    mangoh_gpioExpPin9_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin9_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        9,
        &handlerRecords[9],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin9_RemoveChangeEventHandler
(
    mangoh_gpioExpPin9_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        9,
        &handlerRecords[9],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 10
le_result_t mangoh_gpioExpPin10_SetInput
(
    mangoh_gpioExpPin10_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 10, polarity);
}

le_result_t mangoh_gpioExpPin10_SetPushPullOutput
(
    mangoh_gpioExpPin10_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 10, polarity, value);
}

le_result_t mangoh_gpioExpPin10_SetTriStateOutput
(
    mangoh_gpioExpPin10_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 10, polarity);
}

le_result_t mangoh_gpioExpPin10_SetOpenDrainOutput
(
    mangoh_gpioExpPin10_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 10, polarity, value);
}

le_result_t mangoh_gpioExpPin10_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 10);
}

le_result_t mangoh_gpioExpPin10_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 10);
}

le_result_t mangoh_gpioExpPin10_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 10);
}

le_result_t mangoh_gpioExpPin10_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 10);
}

le_result_t mangoh_gpioExpPin10_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 10);
}

le_result_t mangoh_gpioExpPin10_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 10);
}

bool mangoh_gpioExpPin10_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 10);
}

le_result_t mangoh_gpioExpPin10_SetEdgeSense
(
    mangoh_gpioExpPin10_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 10, trigger);
}

mangoh_gpioExpPin10_Edge_t mangoh_gpioExpPin10_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 10);
}

le_result_t mangoh_gpioExpPin10_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 10);
}

bool mangoh_gpioExpPin10_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 10);
}

bool mangoh_gpioExpPin10_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 10);
}

mangoh_gpioExpPin10_Polarity_t mangoh_gpioExpPin10_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 10);
}

bool mangoh_gpioExpPin10_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 10);
}

mangoh_gpioExpPin10_PullUpDown_t mangoh_gpioExpPin10_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 10);
}

mangoh_gpioExpPin10_ChangeEventHandlerRef_t mangoh_gpioExpPin10_AddChangeEventHandler
(
    mangoh_gpioExpPin10_Edge_t trigger,
    mangoh_gpioExpPin10_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin10_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        10,
        &handlerRecords[10],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin10_RemoveChangeEventHandler
(
    mangoh_gpioExpPin10_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        10,
        &handlerRecords[10],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 11
le_result_t mangoh_gpioExpPin11_SetInput
(
    mangoh_gpioExpPin11_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 11, polarity);
}

le_result_t mangoh_gpioExpPin11_SetPushPullOutput
(
    mangoh_gpioExpPin11_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 11, polarity, value);
}

le_result_t mangoh_gpioExpPin11_SetTriStateOutput
(
    mangoh_gpioExpPin11_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 11, polarity);
}

le_result_t mangoh_gpioExpPin11_SetOpenDrainOutput
(
    mangoh_gpioExpPin11_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 11, polarity, value);
}

le_result_t mangoh_gpioExpPin11_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 11);
}

le_result_t mangoh_gpioExpPin11_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 11);
}

le_result_t mangoh_gpioExpPin11_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 11);
}

le_result_t mangoh_gpioExpPin11_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 11);
}

le_result_t mangoh_gpioExpPin11_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 11);
}

le_result_t mangoh_gpioExpPin11_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 11);
}

bool mangoh_gpioExpPin11_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 11);
}

le_result_t mangoh_gpioExpPin11_SetEdgeSense
(
    mangoh_gpioExpPin11_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 11, trigger);
}

mangoh_gpioExpPin11_Edge_t mangoh_gpioExpPin11_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 11);
}

le_result_t mangoh_gpioExpPin11_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 11);
}

bool mangoh_gpioExpPin11_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 11);
}

bool mangoh_gpioExpPin11_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 11);
}

mangoh_gpioExpPin11_Polarity_t mangoh_gpioExpPin11_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 11);
}

bool mangoh_gpioExpPin11_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 11);
}

mangoh_gpioExpPin11_PullUpDown_t mangoh_gpioExpPin11_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 11);
}

mangoh_gpioExpPin11_ChangeEventHandlerRef_t mangoh_gpioExpPin11_AddChangeEventHandler
(
    mangoh_gpioExpPin11_Edge_t trigger,
    mangoh_gpioExpPin11_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin11_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        11,
        &handlerRecords[11],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin11_RemoveChangeEventHandler
(
    mangoh_gpioExpPin11_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        11,
        &handlerRecords[11],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 12
le_result_t mangoh_gpioExpPin12_SetInput
(
    mangoh_gpioExpPin12_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 12, polarity);
}

le_result_t mangoh_gpioExpPin12_SetPushPullOutput
(
    mangoh_gpioExpPin12_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 12, polarity, value);
}

le_result_t mangoh_gpioExpPin12_SetTriStateOutput
(
    mangoh_gpioExpPin12_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 12, polarity);
}

le_result_t mangoh_gpioExpPin12_SetOpenDrainOutput
(
    mangoh_gpioExpPin12_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 12, polarity, value);
}

le_result_t mangoh_gpioExpPin12_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 12);
}

le_result_t mangoh_gpioExpPin12_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 12);
}

le_result_t mangoh_gpioExpPin12_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 12);
}

le_result_t mangoh_gpioExpPin12_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 12);
}

le_result_t mangoh_gpioExpPin12_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 12);
}

le_result_t mangoh_gpioExpPin12_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 12);
}

bool mangoh_gpioExpPin12_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 12);
}

le_result_t mangoh_gpioExpPin12_SetEdgeSense
(
    mangoh_gpioExpPin12_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 12, trigger);
}

mangoh_gpioExpPin12_Edge_t mangoh_gpioExpPin12_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 12);
}

le_result_t mangoh_gpioExpPin12_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 12);
}

bool mangoh_gpioExpPin12_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 12);
}

bool mangoh_gpioExpPin12_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 12);
}

mangoh_gpioExpPin12_Polarity_t mangoh_gpioExpPin12_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 12);
}

bool mangoh_gpioExpPin12_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 12);
}

mangoh_gpioExpPin12_PullUpDown_t mangoh_gpioExpPin12_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 12);
}

mangoh_gpioExpPin12_ChangeEventHandlerRef_t mangoh_gpioExpPin12_AddChangeEventHandler
(
    mangoh_gpioExpPin12_Edge_t trigger,
    mangoh_gpioExpPin12_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin12_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        12,
        &handlerRecords[12],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin12_RemoveChangeEventHandler
(
    mangoh_gpioExpPin12_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        12,
        &handlerRecords[12],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 13
le_result_t mangoh_gpioExpPin13_SetInput
(
    mangoh_gpioExpPin13_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 13, polarity);
}

le_result_t mangoh_gpioExpPin13_SetPushPullOutput
(
    mangoh_gpioExpPin13_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 13, polarity, value);
}

le_result_t mangoh_gpioExpPin13_SetTriStateOutput
(
    mangoh_gpioExpPin13_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 13, polarity);
}

le_result_t mangoh_gpioExpPin13_SetOpenDrainOutput
(
    mangoh_gpioExpPin13_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 13, polarity, value);
}

le_result_t mangoh_gpioExpPin13_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 13);
}

le_result_t mangoh_gpioExpPin13_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 13);
}

le_result_t mangoh_gpioExpPin13_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 13);
}

le_result_t mangoh_gpioExpPin13_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 13);
}

le_result_t mangoh_gpioExpPin13_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 13);
}

le_result_t mangoh_gpioExpPin13_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 13);
}

bool mangoh_gpioExpPin13_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 13);
}

le_result_t mangoh_gpioExpPin13_SetEdgeSense
(
    mangoh_gpioExpPin13_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 13, trigger);
}

mangoh_gpioExpPin13_Edge_t mangoh_gpioExpPin13_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 13);
}

le_result_t mangoh_gpioExpPin13_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 13);
}

bool mangoh_gpioExpPin13_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 13);
}

bool mangoh_gpioExpPin13_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 13);
}

mangoh_gpioExpPin13_Polarity_t mangoh_gpioExpPin13_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 13);
}

bool mangoh_gpioExpPin13_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 13);
}

mangoh_gpioExpPin13_PullUpDown_t mangoh_gpioExpPin13_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 13);
}

mangoh_gpioExpPin13_ChangeEventHandlerRef_t mangoh_gpioExpPin13_AddChangeEventHandler
(
    mangoh_gpioExpPin13_Edge_t trigger,
    mangoh_gpioExpPin13_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin13_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        13,
        &handlerRecords[13],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin13_RemoveChangeEventHandler
(
    mangoh_gpioExpPin13_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        13,
        &handlerRecords[13],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 14
le_result_t mangoh_gpioExpPin14_SetInput
(
    mangoh_gpioExpPin14_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 14, polarity);
}

le_result_t mangoh_gpioExpPin14_SetPushPullOutput
(
    mangoh_gpioExpPin14_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 14, polarity, value);
}

le_result_t mangoh_gpioExpPin14_SetTriStateOutput
(
    mangoh_gpioExpPin14_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 14, polarity);
}

le_result_t mangoh_gpioExpPin14_SetOpenDrainOutput
(
    mangoh_gpioExpPin14_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 14, polarity, value);
}

le_result_t mangoh_gpioExpPin14_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 14);
}

le_result_t mangoh_gpioExpPin14_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 14);
}

le_result_t mangoh_gpioExpPin14_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 14);
}

le_result_t mangoh_gpioExpPin14_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 14);
}

le_result_t mangoh_gpioExpPin14_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 14);
}

le_result_t mangoh_gpioExpPin14_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 14);
}

bool mangoh_gpioExpPin14_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 14);
}

le_result_t mangoh_gpioExpPin14_SetEdgeSense
(
    mangoh_gpioExpPin14_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 14, trigger);
}

mangoh_gpioExpPin14_Edge_t mangoh_gpioExpPin14_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 14);
}

le_result_t mangoh_gpioExpPin14_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 14);
}

bool mangoh_gpioExpPin14_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 14);
}

bool mangoh_gpioExpPin14_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 14);
}

mangoh_gpioExpPin14_Polarity_t mangoh_gpioExpPin14_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 14);
}

bool mangoh_gpioExpPin14_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 14);
}

mangoh_gpioExpPin14_PullUpDown_t mangoh_gpioExpPin14_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 14);
}

mangoh_gpioExpPin14_ChangeEventHandlerRef_t mangoh_gpioExpPin14_AddChangeEventHandler
(
    mangoh_gpioExpPin14_Edge_t trigger,
    mangoh_gpioExpPin14_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin14_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        14,
        &handlerRecords[14],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin14_RemoveChangeEventHandler
(
    mangoh_gpioExpPin14_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        14,
        &handlerRecords[14],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander GPIO 15
le_result_t mangoh_gpioExpPin15_SetInput
(
    mangoh_gpioExpPin15_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander, 15, polarity);
}

le_result_t mangoh_gpioExpPin15_SetPushPullOutput
(
    mangoh_gpioExpPin15_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander, 15, polarity, value);
}

le_result_t mangoh_gpioExpPin15_SetTriStateOutput
(
    mangoh_gpioExpPin15_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander, 15, polarity);
}

le_result_t mangoh_gpioExpPin15_SetOpenDrainOutput
(
    mangoh_gpioExpPin15_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander, 15, polarity, value);
}

le_result_t mangoh_gpioExpPin15_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander, 15);
}

le_result_t mangoh_gpioExpPin15_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander, 15);
}

le_result_t mangoh_gpioExpPin15_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander, 15);
}

le_result_t mangoh_gpioExpPin15_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander, 15);
}

le_result_t mangoh_gpioExpPin15_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander, 15);
}

le_result_t mangoh_gpioExpPin15_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander, 15);
}

bool mangoh_gpioExpPin15_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander, 15);
}

le_result_t mangoh_gpioExpPin15_SetEdgeSense
(
    mangoh_gpioExpPin15_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander, 15, trigger);
}

mangoh_gpioExpPin15_Edge_t mangoh_gpioExpPin15_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander, 15);
}

le_result_t mangoh_gpioExpPin15_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander, 15);
}

bool mangoh_gpioExpPin15_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander, 15);
}

bool mangoh_gpioExpPin15_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander, 15);
}

mangoh_gpioExpPin15_Polarity_t mangoh_gpioExpPin15_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander, 15);
}

bool mangoh_gpioExpPin15_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander, 15);
}

mangoh_gpioExpPin15_PullUpDown_t mangoh_gpioExpPin15_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander, 15);
}

mangoh_gpioExpPin15_ChangeEventHandlerRef_t mangoh_gpioExpPin15_AddChangeEventHandler
(
    mangoh_gpioExpPin15_Edge_t trigger,
    mangoh_gpioExpPin15_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExpPin15_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander,
        15,
        &handlerRecords[15],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExpPin15_RemoveChangeEventHandler
(
    mangoh_gpioExpPin15_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander,
        15,
        &handlerRecords[15],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// ----- END GENERATED CODE
