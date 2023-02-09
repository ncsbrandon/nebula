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


#define I2C_SX1509_GPIO_EXPANDER1_ADDR      0x3E
#define I2C_SX1509_GPIO_EXPANDER2_ADDR      0x3F
#define I2C_SX1509_GPIO_EXPANDER3_ADDR      0x70

#define EXPANDER1_BUS (PrimaryI2cBusNum + 5)
#define EXPANDER2_BUS (PrimaryI2cBusNum + 6)
#define EXPANDER3_BUS (PrimaryI2cBusNum + 7)

#define EXPANDER2_PIN_EXPANDER1_INTERRUPT (0)
#define EXPANDER2_PIN_EXPANDER3_INTERRUPT (14)

//--------------------------------------------------------------------------------------------------
/**
 * Indicies of the expanders within the handler and pin spec arrays.
 */
//--------------------------------------------------------------------------------------------------
#define EXPANDER_1_INDEX 0
#define EXPANDER_2_INDEX 1
#define EXPANDER_3_INDEX 2

static uint8_t PrimaryI2cBusNum;

// .i2cBus must be populated in COMPONENT_INIT once PrimaryI2cBusNum is discovered.
static gpioExpander_Identifier_t GpioExpanders[3] =
{
    [EXPANDER_1_INDEX] = {
        .i2cAddr = I2C_SX1509_GPIO_EXPANDER1_ADDR,
    },
    [EXPANDER_2_INDEX] = {
        .i2cAddr = I2C_SX1509_GPIO_EXPANDER2_ADDR,
    },
    [EXPANDER_3_INDEX] = {
        .i2cAddr = I2C_SX1509_GPIO_EXPANDER3_ADDR,
    },
};

// Note: will be zeroed by spec, so no need to explicitly initialize the values
static gpioExpander_HandlerRecord_t handlerRecords[3][16];

static gpioExpander_HandlerRecord_t *expander1InterruptHandlerRecord =
    &(handlerRecords[EXPANDER_2_INDEX][0]);
static gpioExpander_HandlerRecord_t *expander3InterruptHandlerRecord =
    &(handlerRecords[EXPANDER_2_INDEX][14]);


//--------------------------------------------------------------------------------------------------
/**
 * Interrupt handler for GPIO expander #1.
 */
//--------------------------------------------------------------------------------------------------
static void gpioExpander_Expander1InterruptHandler
(
    bool state,       ///< Current state of the GPIO - true: active, false: inactive
    void *contextPtr  ///< Unused
)
{
    gpioExpander_GenericInterruptHandler(
        &GpioExpanders[EXPANDER_1_INDEX], handlerRecords[EXPANDER_1_INDEX]);
}

//--------------------------------------------------------------------------------------------------
/**
 * Interrupt handler for GPIO expander #2.
 */
//--------------------------------------------------------------------------------------------------
static void gpioExpander_Expander2InterruptHandler
(
    bool state,       ///< Current state of the GPIO - true: active, false: inactive
    void *contextPtr  ///< Unused
)
{
    gpioExpander_GenericInterruptHandler(
        &GpioExpanders[EXPANDER_2_INDEX], handlerRecords[EXPANDER_2_INDEX]);
}

//--------------------------------------------------------------------------------------------------
/**
 * Interrupt handler for GPIO expander #3.
 */
//--------------------------------------------------------------------------------------------------
static void gpioExpander_Expander3InterruptHandler
(
    bool state,       ///< Current state of the GPIO - true: active, false: inactive
    void *contextPtr  ///< Unused
)
{
    gpioExpander_GenericInterruptHandler(
        &GpioExpanders[EXPANDER_3_INDEX], handlerRecords[EXPANDER_3_INDEX]);
}


//--------------------------------------------------------------------------------------------------
/**
 * Initialize the three GPIO expanders and configure the interrupts.  GPIO expander #2's interrupt
 * pin is connected to a GPIO in the CF3, but GPIO expander #1 and #3 have their interrupt pins
 * connected to a GPIO of expander #2.
 */
//--------------------------------------------------------------------------------------------------
COMPONENT_INIT
{
    LE_FATAL_IF(
        gpioExpander_DiscoverPrimaryI2cBusNum(&PrimaryI2cBusNum) != LE_OK,
        "Couldn't determine the primary I2C bus");
    GpioExpanders[EXPANDER_1_INDEX].i2cBus = EXPANDER1_BUS;
    GpioExpanders[EXPANDER_2_INDEX].i2cBus = EXPANDER2_BUS;
    GpioExpanders[EXPANDER_3_INDEX].i2cBus = EXPANDER3_BUS;

    // Reset the GPIO expanders
    gpioExpander_Reset(&GpioExpanders[EXPANDER_2_INDEX]);
    gpioExpander_Reset(&GpioExpanders[EXPANDER_1_INDEX]);
    gpioExpander_Reset(&GpioExpanders[EXPANDER_3_INDEX]);

    // Configure the interrupt that run from expander 2 to the CF3
    expander2Interrupt_EnablePullUp();
    expander2Interrupt_SetInput(EXPANDER2INTERRUPT_ACTIVE_LOW);
    expander2Interrupt_AddChangeEventHandler(
        EXPANDER2INTERRUPT_EDGE_RISING,
        &gpioExpander_Expander2InterruptHandler,
        NULL,
        100);

    // Configure the interrupt that run from expander 1 to expander 2
    gpioExpander_DisableResistors(
        &GpioExpanders[EXPANDER_2_INDEX], EXPANDER2_PIN_EXPANDER1_INTERRUPT);
    gpioExpander_SetInput(
        &GpioExpanders[EXPANDER_2_INDEX],
        EXPANDER2_PIN_EXPANDER1_INTERRUPT,
        GPIO_EXPANDER_ACTIVE_LOW);
    gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        EXPANDER2_PIN_EXPANDER1_INTERRUPT,
        expander1InterruptHandlerRecord,
        GPIO_EXPANDER_EDGE_RISING,
        &gpioExpander_Expander1InterruptHandler,
        NULL,
        1);

    // Configure the interrupt that run from expander 3 to expander 2
    gpioExpander_DisableResistors(
        &GpioExpanders[EXPANDER_2_INDEX], EXPANDER2_PIN_EXPANDER3_INTERRUPT);
    gpioExpander_SetInput(
        &GpioExpanders[EXPANDER_2_INDEX],
        EXPANDER2_PIN_EXPANDER3_INTERRUPT,
        GPIO_EXPANDER_ACTIVE_LOW);
    gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        EXPANDER2_PIN_EXPANDER3_INTERRUPT,
        expander3InterruptHandlerRecord,
        GPIO_EXPANDER_EDGE_RISING,
        &gpioExpander_Expander3InterruptHandler,
        NULL,
        1);
}


// ----- BEGIN GENERATED CODE

// GPIO expander #1 GPIO 0
le_result_t mangoh_gpioExp1Pin0_SetInput
(
    mangoh_gpioExp1Pin0_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 0, polarity);
}

le_result_t mangoh_gpioExp1Pin0_SetPushPullOutput
(
    mangoh_gpioExp1Pin0_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 0, polarity, value);
}

le_result_t mangoh_gpioExp1Pin0_SetTriStateOutput
(
    mangoh_gpioExp1Pin0_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 0, polarity);
}

le_result_t mangoh_gpioExp1Pin0_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin0_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 0, polarity, value);
}

le_result_t mangoh_gpioExp1Pin0_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

le_result_t mangoh_gpioExp1Pin0_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

le_result_t mangoh_gpioExp1Pin0_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

le_result_t mangoh_gpioExp1Pin0_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

le_result_t mangoh_gpioExp1Pin0_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

le_result_t mangoh_gpioExp1Pin0_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

bool mangoh_gpioExp1Pin0_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

le_result_t mangoh_gpioExp1Pin0_SetEdgeSense
(
    mangoh_gpioExp1Pin0_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 0, trigger);
}

mangoh_gpioExp1Pin0_Edge_t mangoh_gpioExp1Pin0_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

le_result_t mangoh_gpioExp1Pin0_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

bool mangoh_gpioExp1Pin0_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

bool mangoh_gpioExp1Pin0_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

mangoh_gpioExp1Pin0_Polarity_t mangoh_gpioExp1Pin0_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

bool mangoh_gpioExp1Pin0_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

mangoh_gpioExp1Pin0_PullUpDown_t mangoh_gpioExp1Pin0_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 0);
}

mangoh_gpioExp1Pin0_ChangeEventHandlerRef_t mangoh_gpioExp1Pin0_AddChangeEventHandler
(
    mangoh_gpioExp1Pin0_Edge_t trigger,
    mangoh_gpioExp1Pin0_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin0_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        0,
        &handlerRecords[EXPANDER_1_INDEX][0],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin0_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin0_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        0,
        &handlerRecords[EXPANDER_1_INDEX][0],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 1
le_result_t mangoh_gpioExp1Pin1_SetInput
(
    mangoh_gpioExp1Pin1_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 1, polarity);
}

le_result_t mangoh_gpioExp1Pin1_SetPushPullOutput
(
    mangoh_gpioExp1Pin1_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 1, polarity, value);
}

le_result_t mangoh_gpioExp1Pin1_SetTriStateOutput
(
    mangoh_gpioExp1Pin1_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 1, polarity);
}

le_result_t mangoh_gpioExp1Pin1_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin1_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 1, polarity, value);
}

le_result_t mangoh_gpioExp1Pin1_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

le_result_t mangoh_gpioExp1Pin1_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

le_result_t mangoh_gpioExp1Pin1_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

le_result_t mangoh_gpioExp1Pin1_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

le_result_t mangoh_gpioExp1Pin1_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

le_result_t mangoh_gpioExp1Pin1_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

bool mangoh_gpioExp1Pin1_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

le_result_t mangoh_gpioExp1Pin1_SetEdgeSense
(
    mangoh_gpioExp1Pin1_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 1, trigger);
}

mangoh_gpioExp1Pin1_Edge_t mangoh_gpioExp1Pin1_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

le_result_t mangoh_gpioExp1Pin1_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

bool mangoh_gpioExp1Pin1_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

bool mangoh_gpioExp1Pin1_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

mangoh_gpioExp1Pin1_Polarity_t mangoh_gpioExp1Pin1_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

bool mangoh_gpioExp1Pin1_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

mangoh_gpioExp1Pin1_PullUpDown_t mangoh_gpioExp1Pin1_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 1);
}

mangoh_gpioExp1Pin1_ChangeEventHandlerRef_t mangoh_gpioExp1Pin1_AddChangeEventHandler
(
    mangoh_gpioExp1Pin1_Edge_t trigger,
    mangoh_gpioExp1Pin1_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin1_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        1,
        &handlerRecords[EXPANDER_1_INDEX][1],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin1_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin1_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        1,
        &handlerRecords[EXPANDER_1_INDEX][1],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 2
le_result_t mangoh_gpioExp1Pin2_SetInput
(
    mangoh_gpioExp1Pin2_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 2, polarity);
}

le_result_t mangoh_gpioExp1Pin2_SetPushPullOutput
(
    mangoh_gpioExp1Pin2_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 2, polarity, value);
}

le_result_t mangoh_gpioExp1Pin2_SetTriStateOutput
(
    mangoh_gpioExp1Pin2_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 2, polarity);
}

le_result_t mangoh_gpioExp1Pin2_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin2_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 2, polarity, value);
}

le_result_t mangoh_gpioExp1Pin2_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

le_result_t mangoh_gpioExp1Pin2_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

le_result_t mangoh_gpioExp1Pin2_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

le_result_t mangoh_gpioExp1Pin2_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

le_result_t mangoh_gpioExp1Pin2_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

le_result_t mangoh_gpioExp1Pin2_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

bool mangoh_gpioExp1Pin2_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

le_result_t mangoh_gpioExp1Pin2_SetEdgeSense
(
    mangoh_gpioExp1Pin2_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 2, trigger);
}

mangoh_gpioExp1Pin2_Edge_t mangoh_gpioExp1Pin2_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

le_result_t mangoh_gpioExp1Pin2_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

bool mangoh_gpioExp1Pin2_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

bool mangoh_gpioExp1Pin2_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

mangoh_gpioExp1Pin2_Polarity_t mangoh_gpioExp1Pin2_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

bool mangoh_gpioExp1Pin2_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

mangoh_gpioExp1Pin2_PullUpDown_t mangoh_gpioExp1Pin2_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 2);
}

mangoh_gpioExp1Pin2_ChangeEventHandlerRef_t mangoh_gpioExp1Pin2_AddChangeEventHandler
(
    mangoh_gpioExp1Pin2_Edge_t trigger,
    mangoh_gpioExp1Pin2_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin2_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        2,
        &handlerRecords[EXPANDER_1_INDEX][2],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin2_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin2_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        2,
        &handlerRecords[EXPANDER_1_INDEX][2],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 3
le_result_t mangoh_gpioExp1Pin3_SetInput
(
    mangoh_gpioExp1Pin3_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 3, polarity);
}

le_result_t mangoh_gpioExp1Pin3_SetPushPullOutput
(
    mangoh_gpioExp1Pin3_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 3, polarity, value);
}

le_result_t mangoh_gpioExp1Pin3_SetTriStateOutput
(
    mangoh_gpioExp1Pin3_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 3, polarity);
}

le_result_t mangoh_gpioExp1Pin3_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin3_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 3, polarity, value);
}

le_result_t mangoh_gpioExp1Pin3_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

le_result_t mangoh_gpioExp1Pin3_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

le_result_t mangoh_gpioExp1Pin3_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

le_result_t mangoh_gpioExp1Pin3_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

le_result_t mangoh_gpioExp1Pin3_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

le_result_t mangoh_gpioExp1Pin3_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

bool mangoh_gpioExp1Pin3_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

le_result_t mangoh_gpioExp1Pin3_SetEdgeSense
(
    mangoh_gpioExp1Pin3_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 3, trigger);
}

mangoh_gpioExp1Pin3_Edge_t mangoh_gpioExp1Pin3_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

le_result_t mangoh_gpioExp1Pin3_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

bool mangoh_gpioExp1Pin3_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

bool mangoh_gpioExp1Pin3_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

mangoh_gpioExp1Pin3_Polarity_t mangoh_gpioExp1Pin3_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

bool mangoh_gpioExp1Pin3_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

mangoh_gpioExp1Pin3_PullUpDown_t mangoh_gpioExp1Pin3_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 3);
}

mangoh_gpioExp1Pin3_ChangeEventHandlerRef_t mangoh_gpioExp1Pin3_AddChangeEventHandler
(
    mangoh_gpioExp1Pin3_Edge_t trigger,
    mangoh_gpioExp1Pin3_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin3_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        3,
        &handlerRecords[EXPANDER_1_INDEX][3],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin3_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin3_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        3,
        &handlerRecords[EXPANDER_1_INDEX][3],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 4
le_result_t mangoh_gpioExp1Pin4_SetInput
(
    mangoh_gpioExp1Pin4_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 4, polarity);
}

le_result_t mangoh_gpioExp1Pin4_SetPushPullOutput
(
    mangoh_gpioExp1Pin4_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 4, polarity, value);
}

le_result_t mangoh_gpioExp1Pin4_SetTriStateOutput
(
    mangoh_gpioExp1Pin4_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 4, polarity);
}

le_result_t mangoh_gpioExp1Pin4_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin4_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 4, polarity, value);
}

le_result_t mangoh_gpioExp1Pin4_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

le_result_t mangoh_gpioExp1Pin4_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

le_result_t mangoh_gpioExp1Pin4_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

le_result_t mangoh_gpioExp1Pin4_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

le_result_t mangoh_gpioExp1Pin4_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

le_result_t mangoh_gpioExp1Pin4_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

bool mangoh_gpioExp1Pin4_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

le_result_t mangoh_gpioExp1Pin4_SetEdgeSense
(
    mangoh_gpioExp1Pin4_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 4, trigger);
}

mangoh_gpioExp1Pin4_Edge_t mangoh_gpioExp1Pin4_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

le_result_t mangoh_gpioExp1Pin4_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

bool mangoh_gpioExp1Pin4_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

bool mangoh_gpioExp1Pin4_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

mangoh_gpioExp1Pin4_Polarity_t mangoh_gpioExp1Pin4_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

bool mangoh_gpioExp1Pin4_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

mangoh_gpioExp1Pin4_PullUpDown_t mangoh_gpioExp1Pin4_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 4);
}

mangoh_gpioExp1Pin4_ChangeEventHandlerRef_t mangoh_gpioExp1Pin4_AddChangeEventHandler
(
    mangoh_gpioExp1Pin4_Edge_t trigger,
    mangoh_gpioExp1Pin4_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin4_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        4,
        &handlerRecords[EXPANDER_1_INDEX][4],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin4_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin4_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        4,
        &handlerRecords[EXPANDER_1_INDEX][4],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 5
le_result_t mangoh_gpioExp1Pin5_SetInput
(
    mangoh_gpioExp1Pin5_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 5, polarity);
}

le_result_t mangoh_gpioExp1Pin5_SetPushPullOutput
(
    mangoh_gpioExp1Pin5_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 5, polarity, value);
}

le_result_t mangoh_gpioExp1Pin5_SetTriStateOutput
(
    mangoh_gpioExp1Pin5_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 5, polarity);
}

le_result_t mangoh_gpioExp1Pin5_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin5_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 5, polarity, value);
}

le_result_t mangoh_gpioExp1Pin5_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

le_result_t mangoh_gpioExp1Pin5_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

le_result_t mangoh_gpioExp1Pin5_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

le_result_t mangoh_gpioExp1Pin5_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

le_result_t mangoh_gpioExp1Pin5_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

le_result_t mangoh_gpioExp1Pin5_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

bool mangoh_gpioExp1Pin5_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

le_result_t mangoh_gpioExp1Pin5_SetEdgeSense
(
    mangoh_gpioExp1Pin5_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 5, trigger);
}

mangoh_gpioExp1Pin5_Edge_t mangoh_gpioExp1Pin5_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

le_result_t mangoh_gpioExp1Pin5_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

bool mangoh_gpioExp1Pin5_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

bool mangoh_gpioExp1Pin5_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

mangoh_gpioExp1Pin5_Polarity_t mangoh_gpioExp1Pin5_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

bool mangoh_gpioExp1Pin5_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

mangoh_gpioExp1Pin5_PullUpDown_t mangoh_gpioExp1Pin5_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 5);
}

mangoh_gpioExp1Pin5_ChangeEventHandlerRef_t mangoh_gpioExp1Pin5_AddChangeEventHandler
(
    mangoh_gpioExp1Pin5_Edge_t trigger,
    mangoh_gpioExp1Pin5_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin5_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        5,
        &handlerRecords[EXPANDER_1_INDEX][5],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin5_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin5_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        5,
        &handlerRecords[EXPANDER_1_INDEX][5],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 6
le_result_t mangoh_gpioExp1Pin6_SetInput
(
    mangoh_gpioExp1Pin6_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 6, polarity);
}

le_result_t mangoh_gpioExp1Pin6_SetPushPullOutput
(
    mangoh_gpioExp1Pin6_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 6, polarity, value);
}

le_result_t mangoh_gpioExp1Pin6_SetTriStateOutput
(
    mangoh_gpioExp1Pin6_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 6, polarity);
}

le_result_t mangoh_gpioExp1Pin6_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin6_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 6, polarity, value);
}

le_result_t mangoh_gpioExp1Pin6_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

le_result_t mangoh_gpioExp1Pin6_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

le_result_t mangoh_gpioExp1Pin6_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

le_result_t mangoh_gpioExp1Pin6_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

le_result_t mangoh_gpioExp1Pin6_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

le_result_t mangoh_gpioExp1Pin6_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

bool mangoh_gpioExp1Pin6_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

le_result_t mangoh_gpioExp1Pin6_SetEdgeSense
(
    mangoh_gpioExp1Pin6_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 6, trigger);
}

mangoh_gpioExp1Pin6_Edge_t mangoh_gpioExp1Pin6_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

le_result_t mangoh_gpioExp1Pin6_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

bool mangoh_gpioExp1Pin6_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

bool mangoh_gpioExp1Pin6_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

mangoh_gpioExp1Pin6_Polarity_t mangoh_gpioExp1Pin6_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

bool mangoh_gpioExp1Pin6_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

mangoh_gpioExp1Pin6_PullUpDown_t mangoh_gpioExp1Pin6_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 6);
}

mangoh_gpioExp1Pin6_ChangeEventHandlerRef_t mangoh_gpioExp1Pin6_AddChangeEventHandler
(
    mangoh_gpioExp1Pin6_Edge_t trigger,
    mangoh_gpioExp1Pin6_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin6_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        6,
        &handlerRecords[EXPANDER_1_INDEX][6],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin6_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin6_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        6,
        &handlerRecords[EXPANDER_1_INDEX][6],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 7
le_result_t mangoh_gpioExp1Pin7_SetInput
(
    mangoh_gpioExp1Pin7_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 7, polarity);
}

le_result_t mangoh_gpioExp1Pin7_SetPushPullOutput
(
    mangoh_gpioExp1Pin7_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 7, polarity, value);
}

le_result_t mangoh_gpioExp1Pin7_SetTriStateOutput
(
    mangoh_gpioExp1Pin7_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 7, polarity);
}

le_result_t mangoh_gpioExp1Pin7_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin7_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 7, polarity, value);
}

le_result_t mangoh_gpioExp1Pin7_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

le_result_t mangoh_gpioExp1Pin7_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

le_result_t mangoh_gpioExp1Pin7_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

le_result_t mangoh_gpioExp1Pin7_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

le_result_t mangoh_gpioExp1Pin7_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

le_result_t mangoh_gpioExp1Pin7_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

bool mangoh_gpioExp1Pin7_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

le_result_t mangoh_gpioExp1Pin7_SetEdgeSense
(
    mangoh_gpioExp1Pin7_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 7, trigger);
}

mangoh_gpioExp1Pin7_Edge_t mangoh_gpioExp1Pin7_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

le_result_t mangoh_gpioExp1Pin7_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

bool mangoh_gpioExp1Pin7_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

bool mangoh_gpioExp1Pin7_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

mangoh_gpioExp1Pin7_Polarity_t mangoh_gpioExp1Pin7_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

bool mangoh_gpioExp1Pin7_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

mangoh_gpioExp1Pin7_PullUpDown_t mangoh_gpioExp1Pin7_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 7);
}

mangoh_gpioExp1Pin7_ChangeEventHandlerRef_t mangoh_gpioExp1Pin7_AddChangeEventHandler
(
    mangoh_gpioExp1Pin7_Edge_t trigger,
    mangoh_gpioExp1Pin7_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin7_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        7,
        &handlerRecords[EXPANDER_1_INDEX][7],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin7_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin7_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        7,
        &handlerRecords[EXPANDER_1_INDEX][7],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 8
le_result_t mangoh_gpioExp1Pin8_SetInput
(
    mangoh_gpioExp1Pin8_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 8, polarity);
}

le_result_t mangoh_gpioExp1Pin8_SetPushPullOutput
(
    mangoh_gpioExp1Pin8_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 8, polarity, value);
}

le_result_t mangoh_gpioExp1Pin8_SetTriStateOutput
(
    mangoh_gpioExp1Pin8_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 8, polarity);
}

le_result_t mangoh_gpioExp1Pin8_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin8_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 8, polarity, value);
}

le_result_t mangoh_gpioExp1Pin8_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

le_result_t mangoh_gpioExp1Pin8_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

le_result_t mangoh_gpioExp1Pin8_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

le_result_t mangoh_gpioExp1Pin8_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

le_result_t mangoh_gpioExp1Pin8_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

le_result_t mangoh_gpioExp1Pin8_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

bool mangoh_gpioExp1Pin8_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

le_result_t mangoh_gpioExp1Pin8_SetEdgeSense
(
    mangoh_gpioExp1Pin8_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 8, trigger);
}

mangoh_gpioExp1Pin8_Edge_t mangoh_gpioExp1Pin8_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

le_result_t mangoh_gpioExp1Pin8_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

bool mangoh_gpioExp1Pin8_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

bool mangoh_gpioExp1Pin8_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

mangoh_gpioExp1Pin8_Polarity_t mangoh_gpioExp1Pin8_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

bool mangoh_gpioExp1Pin8_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

mangoh_gpioExp1Pin8_PullUpDown_t mangoh_gpioExp1Pin8_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 8);
}

mangoh_gpioExp1Pin8_ChangeEventHandlerRef_t mangoh_gpioExp1Pin8_AddChangeEventHandler
(
    mangoh_gpioExp1Pin8_Edge_t trigger,
    mangoh_gpioExp1Pin8_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin8_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        8,
        &handlerRecords[EXPANDER_1_INDEX][8],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin8_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin8_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        8,
        &handlerRecords[EXPANDER_1_INDEX][8],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 9
le_result_t mangoh_gpioExp1Pin9_SetInput
(
    mangoh_gpioExp1Pin9_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 9, polarity);
}

le_result_t mangoh_gpioExp1Pin9_SetPushPullOutput
(
    mangoh_gpioExp1Pin9_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 9, polarity, value);
}

le_result_t mangoh_gpioExp1Pin9_SetTriStateOutput
(
    mangoh_gpioExp1Pin9_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 9, polarity);
}

le_result_t mangoh_gpioExp1Pin9_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin9_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 9, polarity, value);
}

le_result_t mangoh_gpioExp1Pin9_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

le_result_t mangoh_gpioExp1Pin9_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

le_result_t mangoh_gpioExp1Pin9_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

le_result_t mangoh_gpioExp1Pin9_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

le_result_t mangoh_gpioExp1Pin9_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

le_result_t mangoh_gpioExp1Pin9_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

bool mangoh_gpioExp1Pin9_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

le_result_t mangoh_gpioExp1Pin9_SetEdgeSense
(
    mangoh_gpioExp1Pin9_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 9, trigger);
}

mangoh_gpioExp1Pin9_Edge_t mangoh_gpioExp1Pin9_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

le_result_t mangoh_gpioExp1Pin9_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

bool mangoh_gpioExp1Pin9_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

bool mangoh_gpioExp1Pin9_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

mangoh_gpioExp1Pin9_Polarity_t mangoh_gpioExp1Pin9_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

bool mangoh_gpioExp1Pin9_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

mangoh_gpioExp1Pin9_PullUpDown_t mangoh_gpioExp1Pin9_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 9);
}

mangoh_gpioExp1Pin9_ChangeEventHandlerRef_t mangoh_gpioExp1Pin9_AddChangeEventHandler
(
    mangoh_gpioExp1Pin9_Edge_t trigger,
    mangoh_gpioExp1Pin9_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin9_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        9,
        &handlerRecords[EXPANDER_1_INDEX][9],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin9_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin9_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        9,
        &handlerRecords[EXPANDER_1_INDEX][9],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 10
le_result_t mangoh_gpioExp1Pin10_SetInput
(
    mangoh_gpioExp1Pin10_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 10, polarity);
}

le_result_t mangoh_gpioExp1Pin10_SetPushPullOutput
(
    mangoh_gpioExp1Pin10_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 10, polarity, value);
}

le_result_t mangoh_gpioExp1Pin10_SetTriStateOutput
(
    mangoh_gpioExp1Pin10_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 10, polarity);
}

le_result_t mangoh_gpioExp1Pin10_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin10_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 10, polarity, value);
}

le_result_t mangoh_gpioExp1Pin10_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

le_result_t mangoh_gpioExp1Pin10_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

le_result_t mangoh_gpioExp1Pin10_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

le_result_t mangoh_gpioExp1Pin10_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

le_result_t mangoh_gpioExp1Pin10_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

le_result_t mangoh_gpioExp1Pin10_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

bool mangoh_gpioExp1Pin10_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

le_result_t mangoh_gpioExp1Pin10_SetEdgeSense
(
    mangoh_gpioExp1Pin10_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 10, trigger);
}

mangoh_gpioExp1Pin10_Edge_t mangoh_gpioExp1Pin10_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

le_result_t mangoh_gpioExp1Pin10_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

bool mangoh_gpioExp1Pin10_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

bool mangoh_gpioExp1Pin10_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

mangoh_gpioExp1Pin10_Polarity_t mangoh_gpioExp1Pin10_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

bool mangoh_gpioExp1Pin10_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

mangoh_gpioExp1Pin10_PullUpDown_t mangoh_gpioExp1Pin10_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 10);
}

mangoh_gpioExp1Pin10_ChangeEventHandlerRef_t mangoh_gpioExp1Pin10_AddChangeEventHandler
(
    mangoh_gpioExp1Pin10_Edge_t trigger,
    mangoh_gpioExp1Pin10_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin10_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        10,
        &handlerRecords[EXPANDER_1_INDEX][10],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin10_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin10_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        10,
        &handlerRecords[EXPANDER_1_INDEX][10],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 11
le_result_t mangoh_gpioExp1Pin11_SetInput
(
    mangoh_gpioExp1Pin11_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 11, polarity);
}

le_result_t mangoh_gpioExp1Pin11_SetPushPullOutput
(
    mangoh_gpioExp1Pin11_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 11, polarity, value);
}

le_result_t mangoh_gpioExp1Pin11_SetTriStateOutput
(
    mangoh_gpioExp1Pin11_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 11, polarity);
}

le_result_t mangoh_gpioExp1Pin11_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin11_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 11, polarity, value);
}

le_result_t mangoh_gpioExp1Pin11_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

le_result_t mangoh_gpioExp1Pin11_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

le_result_t mangoh_gpioExp1Pin11_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

le_result_t mangoh_gpioExp1Pin11_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

le_result_t mangoh_gpioExp1Pin11_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

le_result_t mangoh_gpioExp1Pin11_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

bool mangoh_gpioExp1Pin11_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

le_result_t mangoh_gpioExp1Pin11_SetEdgeSense
(
    mangoh_gpioExp1Pin11_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 11, trigger);
}

mangoh_gpioExp1Pin11_Edge_t mangoh_gpioExp1Pin11_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

le_result_t mangoh_gpioExp1Pin11_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

bool mangoh_gpioExp1Pin11_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

bool mangoh_gpioExp1Pin11_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

mangoh_gpioExp1Pin11_Polarity_t mangoh_gpioExp1Pin11_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

bool mangoh_gpioExp1Pin11_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

mangoh_gpioExp1Pin11_PullUpDown_t mangoh_gpioExp1Pin11_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 11);
}

mangoh_gpioExp1Pin11_ChangeEventHandlerRef_t mangoh_gpioExp1Pin11_AddChangeEventHandler
(
    mangoh_gpioExp1Pin11_Edge_t trigger,
    mangoh_gpioExp1Pin11_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin11_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        11,
        &handlerRecords[EXPANDER_1_INDEX][11],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin11_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin11_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        11,
        &handlerRecords[EXPANDER_1_INDEX][11],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 12
le_result_t mangoh_gpioExp1Pin12_SetInput
(
    mangoh_gpioExp1Pin12_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 12, polarity);
}

le_result_t mangoh_gpioExp1Pin12_SetPushPullOutput
(
    mangoh_gpioExp1Pin12_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 12, polarity, value);
}

le_result_t mangoh_gpioExp1Pin12_SetTriStateOutput
(
    mangoh_gpioExp1Pin12_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 12, polarity);
}

le_result_t mangoh_gpioExp1Pin12_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin12_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 12, polarity, value);
}

le_result_t mangoh_gpioExp1Pin12_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

le_result_t mangoh_gpioExp1Pin12_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

le_result_t mangoh_gpioExp1Pin12_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

le_result_t mangoh_gpioExp1Pin12_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

le_result_t mangoh_gpioExp1Pin12_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

le_result_t mangoh_gpioExp1Pin12_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

bool mangoh_gpioExp1Pin12_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

le_result_t mangoh_gpioExp1Pin12_SetEdgeSense
(
    mangoh_gpioExp1Pin12_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 12, trigger);
}

mangoh_gpioExp1Pin12_Edge_t mangoh_gpioExp1Pin12_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

le_result_t mangoh_gpioExp1Pin12_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

bool mangoh_gpioExp1Pin12_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

bool mangoh_gpioExp1Pin12_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

mangoh_gpioExp1Pin12_Polarity_t mangoh_gpioExp1Pin12_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

bool mangoh_gpioExp1Pin12_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

mangoh_gpioExp1Pin12_PullUpDown_t mangoh_gpioExp1Pin12_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 12);
}

mangoh_gpioExp1Pin12_ChangeEventHandlerRef_t mangoh_gpioExp1Pin12_AddChangeEventHandler
(
    mangoh_gpioExp1Pin12_Edge_t trigger,
    mangoh_gpioExp1Pin12_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin12_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        12,
        &handlerRecords[EXPANDER_1_INDEX][12],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin12_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin12_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        12,
        &handlerRecords[EXPANDER_1_INDEX][12],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 13
le_result_t mangoh_gpioExp1Pin13_SetInput
(
    mangoh_gpioExp1Pin13_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 13, polarity);
}

le_result_t mangoh_gpioExp1Pin13_SetPushPullOutput
(
    mangoh_gpioExp1Pin13_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 13, polarity, value);
}

le_result_t mangoh_gpioExp1Pin13_SetTriStateOutput
(
    mangoh_gpioExp1Pin13_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 13, polarity);
}

le_result_t mangoh_gpioExp1Pin13_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin13_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 13, polarity, value);
}

le_result_t mangoh_gpioExp1Pin13_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

le_result_t mangoh_gpioExp1Pin13_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

le_result_t mangoh_gpioExp1Pin13_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

le_result_t mangoh_gpioExp1Pin13_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

le_result_t mangoh_gpioExp1Pin13_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

le_result_t mangoh_gpioExp1Pin13_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

bool mangoh_gpioExp1Pin13_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

le_result_t mangoh_gpioExp1Pin13_SetEdgeSense
(
    mangoh_gpioExp1Pin13_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 13, trigger);
}

mangoh_gpioExp1Pin13_Edge_t mangoh_gpioExp1Pin13_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

le_result_t mangoh_gpioExp1Pin13_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

bool mangoh_gpioExp1Pin13_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

bool mangoh_gpioExp1Pin13_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

mangoh_gpioExp1Pin13_Polarity_t mangoh_gpioExp1Pin13_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

bool mangoh_gpioExp1Pin13_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

mangoh_gpioExp1Pin13_PullUpDown_t mangoh_gpioExp1Pin13_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 13);
}

mangoh_gpioExp1Pin13_ChangeEventHandlerRef_t mangoh_gpioExp1Pin13_AddChangeEventHandler
(
    mangoh_gpioExp1Pin13_Edge_t trigger,
    mangoh_gpioExp1Pin13_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin13_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        13,
        &handlerRecords[EXPANDER_1_INDEX][13],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin13_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin13_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        13,
        &handlerRecords[EXPANDER_1_INDEX][13],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 14
le_result_t mangoh_gpioExp1Pin14_SetInput
(
    mangoh_gpioExp1Pin14_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 14, polarity);
}

le_result_t mangoh_gpioExp1Pin14_SetPushPullOutput
(
    mangoh_gpioExp1Pin14_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 14, polarity, value);
}

le_result_t mangoh_gpioExp1Pin14_SetTriStateOutput
(
    mangoh_gpioExp1Pin14_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 14, polarity);
}

le_result_t mangoh_gpioExp1Pin14_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin14_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 14, polarity, value);
}

le_result_t mangoh_gpioExp1Pin14_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

le_result_t mangoh_gpioExp1Pin14_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

le_result_t mangoh_gpioExp1Pin14_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

le_result_t mangoh_gpioExp1Pin14_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

le_result_t mangoh_gpioExp1Pin14_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

le_result_t mangoh_gpioExp1Pin14_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

bool mangoh_gpioExp1Pin14_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

le_result_t mangoh_gpioExp1Pin14_SetEdgeSense
(
    mangoh_gpioExp1Pin14_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 14, trigger);
}

mangoh_gpioExp1Pin14_Edge_t mangoh_gpioExp1Pin14_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

le_result_t mangoh_gpioExp1Pin14_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

bool mangoh_gpioExp1Pin14_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

bool mangoh_gpioExp1Pin14_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

mangoh_gpioExp1Pin14_Polarity_t mangoh_gpioExp1Pin14_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

bool mangoh_gpioExp1Pin14_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

mangoh_gpioExp1Pin14_PullUpDown_t mangoh_gpioExp1Pin14_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 14);
}

mangoh_gpioExp1Pin14_ChangeEventHandlerRef_t mangoh_gpioExp1Pin14_AddChangeEventHandler
(
    mangoh_gpioExp1Pin14_Edge_t trigger,
    mangoh_gpioExp1Pin14_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin14_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        14,
        &handlerRecords[EXPANDER_1_INDEX][14],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin14_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin14_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        14,
        &handlerRecords[EXPANDER_1_INDEX][14],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #1 GPIO 15
le_result_t mangoh_gpioExp1Pin15_SetInput
(
    mangoh_gpioExp1Pin15_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_1_INDEX], 15, polarity);
}

le_result_t mangoh_gpioExp1Pin15_SetPushPullOutput
(
    mangoh_gpioExp1Pin15_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 15, polarity, value);
}

le_result_t mangoh_gpioExp1Pin15_SetTriStateOutput
(
    mangoh_gpioExp1Pin15_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_1_INDEX], 15, polarity);
}

le_result_t mangoh_gpioExp1Pin15_SetOpenDrainOutput
(
    mangoh_gpioExp1Pin15_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_1_INDEX], 15, polarity, value);
}

le_result_t mangoh_gpioExp1Pin15_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

le_result_t mangoh_gpioExp1Pin15_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

le_result_t mangoh_gpioExp1Pin15_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

le_result_t mangoh_gpioExp1Pin15_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

le_result_t mangoh_gpioExp1Pin15_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

le_result_t mangoh_gpioExp1Pin15_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

bool mangoh_gpioExp1Pin15_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

le_result_t mangoh_gpioExp1Pin15_SetEdgeSense
(
    mangoh_gpioExp1Pin15_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 15, trigger);
}

mangoh_gpioExp1Pin15_Edge_t mangoh_gpioExp1Pin15_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

le_result_t mangoh_gpioExp1Pin15_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

bool mangoh_gpioExp1Pin15_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

bool mangoh_gpioExp1Pin15_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

mangoh_gpioExp1Pin15_Polarity_t mangoh_gpioExp1Pin15_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

bool mangoh_gpioExp1Pin15_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

mangoh_gpioExp1Pin15_PullUpDown_t mangoh_gpioExp1Pin15_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_1_INDEX], 15);
}

mangoh_gpioExp1Pin15_ChangeEventHandlerRef_t mangoh_gpioExp1Pin15_AddChangeEventHandler
(
    mangoh_gpioExp1Pin15_Edge_t trigger,
    mangoh_gpioExp1Pin15_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp1Pin15_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        15,
        &handlerRecords[EXPANDER_1_INDEX][15],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp1Pin15_RemoveChangeEventHandler
(
    mangoh_gpioExp1Pin15_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_1_INDEX],
        15,
        &handlerRecords[EXPANDER_1_INDEX][15],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 0
le_result_t mangoh_gpioExp2Pin0_SetInput
(
    mangoh_gpioExp2Pin0_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 0, polarity);
}

le_result_t mangoh_gpioExp2Pin0_SetPushPullOutput
(
    mangoh_gpioExp2Pin0_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 0, polarity, value);
}

le_result_t mangoh_gpioExp2Pin0_SetTriStateOutput
(
    mangoh_gpioExp2Pin0_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 0, polarity);
}

le_result_t mangoh_gpioExp2Pin0_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin0_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 0, polarity, value);
}

le_result_t mangoh_gpioExp2Pin0_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

le_result_t mangoh_gpioExp2Pin0_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

le_result_t mangoh_gpioExp2Pin0_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

le_result_t mangoh_gpioExp2Pin0_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

le_result_t mangoh_gpioExp2Pin0_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

le_result_t mangoh_gpioExp2Pin0_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

bool mangoh_gpioExp2Pin0_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

le_result_t mangoh_gpioExp2Pin0_SetEdgeSense
(
    mangoh_gpioExp2Pin0_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 0, trigger);
}

mangoh_gpioExp2Pin0_Edge_t mangoh_gpioExp2Pin0_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

le_result_t mangoh_gpioExp2Pin0_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

bool mangoh_gpioExp2Pin0_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

bool mangoh_gpioExp2Pin0_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

mangoh_gpioExp2Pin0_Polarity_t mangoh_gpioExp2Pin0_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

bool mangoh_gpioExp2Pin0_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

mangoh_gpioExp2Pin0_PullUpDown_t mangoh_gpioExp2Pin0_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 0);
}

mangoh_gpioExp2Pin0_ChangeEventHandlerRef_t mangoh_gpioExp2Pin0_AddChangeEventHandler
(
    mangoh_gpioExp2Pin0_Edge_t trigger,
    mangoh_gpioExp2Pin0_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin0_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        0,
        &handlerRecords[EXPANDER_2_INDEX][0],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin0_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin0_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        0,
        &handlerRecords[EXPANDER_2_INDEX][0],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 1
le_result_t mangoh_gpioExp2Pin1_SetInput
(
    mangoh_gpioExp2Pin1_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 1, polarity);
}

le_result_t mangoh_gpioExp2Pin1_SetPushPullOutput
(
    mangoh_gpioExp2Pin1_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 1, polarity, value);
}

le_result_t mangoh_gpioExp2Pin1_SetTriStateOutput
(
    mangoh_gpioExp2Pin1_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 1, polarity);
}

le_result_t mangoh_gpioExp2Pin1_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin1_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 1, polarity, value);
}

le_result_t mangoh_gpioExp2Pin1_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

le_result_t mangoh_gpioExp2Pin1_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

le_result_t mangoh_gpioExp2Pin1_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

le_result_t mangoh_gpioExp2Pin1_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

le_result_t mangoh_gpioExp2Pin1_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

le_result_t mangoh_gpioExp2Pin1_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

bool mangoh_gpioExp2Pin1_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

le_result_t mangoh_gpioExp2Pin1_SetEdgeSense
(
    mangoh_gpioExp2Pin1_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 1, trigger);
}

mangoh_gpioExp2Pin1_Edge_t mangoh_gpioExp2Pin1_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

le_result_t mangoh_gpioExp2Pin1_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

bool mangoh_gpioExp2Pin1_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

bool mangoh_gpioExp2Pin1_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

mangoh_gpioExp2Pin1_Polarity_t mangoh_gpioExp2Pin1_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

bool mangoh_gpioExp2Pin1_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

mangoh_gpioExp2Pin1_PullUpDown_t mangoh_gpioExp2Pin1_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 1);
}

mangoh_gpioExp2Pin1_ChangeEventHandlerRef_t mangoh_gpioExp2Pin1_AddChangeEventHandler
(
    mangoh_gpioExp2Pin1_Edge_t trigger,
    mangoh_gpioExp2Pin1_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin1_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        1,
        &handlerRecords[EXPANDER_2_INDEX][1],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin1_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin1_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        1,
        &handlerRecords[EXPANDER_2_INDEX][1],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 2
le_result_t mangoh_gpioExp2Pin2_SetInput
(
    mangoh_gpioExp2Pin2_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 2, polarity);
}

le_result_t mangoh_gpioExp2Pin2_SetPushPullOutput
(
    mangoh_gpioExp2Pin2_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 2, polarity, value);
}

le_result_t mangoh_gpioExp2Pin2_SetTriStateOutput
(
    mangoh_gpioExp2Pin2_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 2, polarity);
}

le_result_t mangoh_gpioExp2Pin2_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin2_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 2, polarity, value);
}

le_result_t mangoh_gpioExp2Pin2_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

le_result_t mangoh_gpioExp2Pin2_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

le_result_t mangoh_gpioExp2Pin2_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

le_result_t mangoh_gpioExp2Pin2_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

le_result_t mangoh_gpioExp2Pin2_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

le_result_t mangoh_gpioExp2Pin2_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

bool mangoh_gpioExp2Pin2_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

le_result_t mangoh_gpioExp2Pin2_SetEdgeSense
(
    mangoh_gpioExp2Pin2_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 2, trigger);
}

mangoh_gpioExp2Pin2_Edge_t mangoh_gpioExp2Pin2_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

le_result_t mangoh_gpioExp2Pin2_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

bool mangoh_gpioExp2Pin2_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

bool mangoh_gpioExp2Pin2_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

mangoh_gpioExp2Pin2_Polarity_t mangoh_gpioExp2Pin2_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

bool mangoh_gpioExp2Pin2_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

mangoh_gpioExp2Pin2_PullUpDown_t mangoh_gpioExp2Pin2_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 2);
}

mangoh_gpioExp2Pin2_ChangeEventHandlerRef_t mangoh_gpioExp2Pin2_AddChangeEventHandler
(
    mangoh_gpioExp2Pin2_Edge_t trigger,
    mangoh_gpioExp2Pin2_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin2_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        2,
        &handlerRecords[EXPANDER_2_INDEX][2],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin2_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin2_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        2,
        &handlerRecords[EXPANDER_2_INDEX][2],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 3
le_result_t mangoh_gpioExp2Pin3_SetInput
(
    mangoh_gpioExp2Pin3_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 3, polarity);
}

le_result_t mangoh_gpioExp2Pin3_SetPushPullOutput
(
    mangoh_gpioExp2Pin3_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 3, polarity, value);
}

le_result_t mangoh_gpioExp2Pin3_SetTriStateOutput
(
    mangoh_gpioExp2Pin3_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 3, polarity);
}

le_result_t mangoh_gpioExp2Pin3_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin3_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 3, polarity, value);
}

le_result_t mangoh_gpioExp2Pin3_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

le_result_t mangoh_gpioExp2Pin3_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

le_result_t mangoh_gpioExp2Pin3_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

le_result_t mangoh_gpioExp2Pin3_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

le_result_t mangoh_gpioExp2Pin3_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

le_result_t mangoh_gpioExp2Pin3_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

bool mangoh_gpioExp2Pin3_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

le_result_t mangoh_gpioExp2Pin3_SetEdgeSense
(
    mangoh_gpioExp2Pin3_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 3, trigger);
}

mangoh_gpioExp2Pin3_Edge_t mangoh_gpioExp2Pin3_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

le_result_t mangoh_gpioExp2Pin3_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

bool mangoh_gpioExp2Pin3_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

bool mangoh_gpioExp2Pin3_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

mangoh_gpioExp2Pin3_Polarity_t mangoh_gpioExp2Pin3_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

bool mangoh_gpioExp2Pin3_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

mangoh_gpioExp2Pin3_PullUpDown_t mangoh_gpioExp2Pin3_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 3);
}

mangoh_gpioExp2Pin3_ChangeEventHandlerRef_t mangoh_gpioExp2Pin3_AddChangeEventHandler
(
    mangoh_gpioExp2Pin3_Edge_t trigger,
    mangoh_gpioExp2Pin3_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin3_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        3,
        &handlerRecords[EXPANDER_2_INDEX][3],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin3_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin3_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        3,
        &handlerRecords[EXPANDER_2_INDEX][3],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 4
le_result_t mangoh_gpioExp2Pin4_SetInput
(
    mangoh_gpioExp2Pin4_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 4, polarity);
}

le_result_t mangoh_gpioExp2Pin4_SetPushPullOutput
(
    mangoh_gpioExp2Pin4_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 4, polarity, value);
}

le_result_t mangoh_gpioExp2Pin4_SetTriStateOutput
(
    mangoh_gpioExp2Pin4_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 4, polarity);
}

le_result_t mangoh_gpioExp2Pin4_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin4_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 4, polarity, value);
}

le_result_t mangoh_gpioExp2Pin4_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

le_result_t mangoh_gpioExp2Pin4_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

le_result_t mangoh_gpioExp2Pin4_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

le_result_t mangoh_gpioExp2Pin4_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

le_result_t mangoh_gpioExp2Pin4_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

le_result_t mangoh_gpioExp2Pin4_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

bool mangoh_gpioExp2Pin4_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

le_result_t mangoh_gpioExp2Pin4_SetEdgeSense
(
    mangoh_gpioExp2Pin4_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 4, trigger);
}

mangoh_gpioExp2Pin4_Edge_t mangoh_gpioExp2Pin4_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

le_result_t mangoh_gpioExp2Pin4_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

bool mangoh_gpioExp2Pin4_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

bool mangoh_gpioExp2Pin4_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

mangoh_gpioExp2Pin4_Polarity_t mangoh_gpioExp2Pin4_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

bool mangoh_gpioExp2Pin4_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

mangoh_gpioExp2Pin4_PullUpDown_t mangoh_gpioExp2Pin4_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 4);
}

mangoh_gpioExp2Pin4_ChangeEventHandlerRef_t mangoh_gpioExp2Pin4_AddChangeEventHandler
(
    mangoh_gpioExp2Pin4_Edge_t trigger,
    mangoh_gpioExp2Pin4_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin4_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        4,
        &handlerRecords[EXPANDER_2_INDEX][4],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin4_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin4_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        4,
        &handlerRecords[EXPANDER_2_INDEX][4],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 5
le_result_t mangoh_gpioExp2Pin5_SetInput
(
    mangoh_gpioExp2Pin5_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 5, polarity);
}

le_result_t mangoh_gpioExp2Pin5_SetPushPullOutput
(
    mangoh_gpioExp2Pin5_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 5, polarity, value);
}

le_result_t mangoh_gpioExp2Pin5_SetTriStateOutput
(
    mangoh_gpioExp2Pin5_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 5, polarity);
}

le_result_t mangoh_gpioExp2Pin5_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin5_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 5, polarity, value);
}

le_result_t mangoh_gpioExp2Pin5_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

le_result_t mangoh_gpioExp2Pin5_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

le_result_t mangoh_gpioExp2Pin5_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

le_result_t mangoh_gpioExp2Pin5_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

le_result_t mangoh_gpioExp2Pin5_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

le_result_t mangoh_gpioExp2Pin5_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

bool mangoh_gpioExp2Pin5_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

le_result_t mangoh_gpioExp2Pin5_SetEdgeSense
(
    mangoh_gpioExp2Pin5_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 5, trigger);
}

mangoh_gpioExp2Pin5_Edge_t mangoh_gpioExp2Pin5_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

le_result_t mangoh_gpioExp2Pin5_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

bool mangoh_gpioExp2Pin5_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

bool mangoh_gpioExp2Pin5_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

mangoh_gpioExp2Pin5_Polarity_t mangoh_gpioExp2Pin5_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

bool mangoh_gpioExp2Pin5_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

mangoh_gpioExp2Pin5_PullUpDown_t mangoh_gpioExp2Pin5_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 5);
}

mangoh_gpioExp2Pin5_ChangeEventHandlerRef_t mangoh_gpioExp2Pin5_AddChangeEventHandler
(
    mangoh_gpioExp2Pin5_Edge_t trigger,
    mangoh_gpioExp2Pin5_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin5_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        5,
        &handlerRecords[EXPANDER_2_INDEX][5],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin5_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin5_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        5,
        &handlerRecords[EXPANDER_2_INDEX][5],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 6
le_result_t mangoh_gpioExp2Pin6_SetInput
(
    mangoh_gpioExp2Pin6_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 6, polarity);
}

le_result_t mangoh_gpioExp2Pin6_SetPushPullOutput
(
    mangoh_gpioExp2Pin6_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 6, polarity, value);
}

le_result_t mangoh_gpioExp2Pin6_SetTriStateOutput
(
    mangoh_gpioExp2Pin6_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 6, polarity);
}

le_result_t mangoh_gpioExp2Pin6_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin6_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 6, polarity, value);
}

le_result_t mangoh_gpioExp2Pin6_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

le_result_t mangoh_gpioExp2Pin6_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

le_result_t mangoh_gpioExp2Pin6_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

le_result_t mangoh_gpioExp2Pin6_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

le_result_t mangoh_gpioExp2Pin6_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

le_result_t mangoh_gpioExp2Pin6_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

bool mangoh_gpioExp2Pin6_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

le_result_t mangoh_gpioExp2Pin6_SetEdgeSense
(
    mangoh_gpioExp2Pin6_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 6, trigger);
}

mangoh_gpioExp2Pin6_Edge_t mangoh_gpioExp2Pin6_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

le_result_t mangoh_gpioExp2Pin6_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

bool mangoh_gpioExp2Pin6_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

bool mangoh_gpioExp2Pin6_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

mangoh_gpioExp2Pin6_Polarity_t mangoh_gpioExp2Pin6_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

bool mangoh_gpioExp2Pin6_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

mangoh_gpioExp2Pin6_PullUpDown_t mangoh_gpioExp2Pin6_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 6);
}

mangoh_gpioExp2Pin6_ChangeEventHandlerRef_t mangoh_gpioExp2Pin6_AddChangeEventHandler
(
    mangoh_gpioExp2Pin6_Edge_t trigger,
    mangoh_gpioExp2Pin6_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin6_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        6,
        &handlerRecords[EXPANDER_2_INDEX][6],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin6_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin6_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        6,
        &handlerRecords[EXPANDER_2_INDEX][6],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 7
le_result_t mangoh_gpioExp2Pin7_SetInput
(
    mangoh_gpioExp2Pin7_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 7, polarity);
}

le_result_t mangoh_gpioExp2Pin7_SetPushPullOutput
(
    mangoh_gpioExp2Pin7_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 7, polarity, value);
}

le_result_t mangoh_gpioExp2Pin7_SetTriStateOutput
(
    mangoh_gpioExp2Pin7_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 7, polarity);
}

le_result_t mangoh_gpioExp2Pin7_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin7_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 7, polarity, value);
}

le_result_t mangoh_gpioExp2Pin7_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

le_result_t mangoh_gpioExp2Pin7_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

le_result_t mangoh_gpioExp2Pin7_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

le_result_t mangoh_gpioExp2Pin7_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

le_result_t mangoh_gpioExp2Pin7_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

le_result_t mangoh_gpioExp2Pin7_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

bool mangoh_gpioExp2Pin7_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

le_result_t mangoh_gpioExp2Pin7_SetEdgeSense
(
    mangoh_gpioExp2Pin7_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 7, trigger);
}

mangoh_gpioExp2Pin7_Edge_t mangoh_gpioExp2Pin7_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

le_result_t mangoh_gpioExp2Pin7_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

bool mangoh_gpioExp2Pin7_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

bool mangoh_gpioExp2Pin7_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

mangoh_gpioExp2Pin7_Polarity_t mangoh_gpioExp2Pin7_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

bool mangoh_gpioExp2Pin7_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

mangoh_gpioExp2Pin7_PullUpDown_t mangoh_gpioExp2Pin7_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 7);
}

mangoh_gpioExp2Pin7_ChangeEventHandlerRef_t mangoh_gpioExp2Pin7_AddChangeEventHandler
(
    mangoh_gpioExp2Pin7_Edge_t trigger,
    mangoh_gpioExp2Pin7_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin7_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        7,
        &handlerRecords[EXPANDER_2_INDEX][7],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin7_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin7_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        7,
        &handlerRecords[EXPANDER_2_INDEX][7],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 8
le_result_t mangoh_gpioExp2Pin8_SetInput
(
    mangoh_gpioExp2Pin8_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 8, polarity);
}

le_result_t mangoh_gpioExp2Pin8_SetPushPullOutput
(
    mangoh_gpioExp2Pin8_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 8, polarity, value);
}

le_result_t mangoh_gpioExp2Pin8_SetTriStateOutput
(
    mangoh_gpioExp2Pin8_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 8, polarity);
}

le_result_t mangoh_gpioExp2Pin8_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin8_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 8, polarity, value);
}

le_result_t mangoh_gpioExp2Pin8_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

le_result_t mangoh_gpioExp2Pin8_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

le_result_t mangoh_gpioExp2Pin8_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

le_result_t mangoh_gpioExp2Pin8_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

le_result_t mangoh_gpioExp2Pin8_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

le_result_t mangoh_gpioExp2Pin8_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

bool mangoh_gpioExp2Pin8_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

le_result_t mangoh_gpioExp2Pin8_SetEdgeSense
(
    mangoh_gpioExp2Pin8_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 8, trigger);
}

mangoh_gpioExp2Pin8_Edge_t mangoh_gpioExp2Pin8_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

le_result_t mangoh_gpioExp2Pin8_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

bool mangoh_gpioExp2Pin8_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

bool mangoh_gpioExp2Pin8_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

mangoh_gpioExp2Pin8_Polarity_t mangoh_gpioExp2Pin8_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

bool mangoh_gpioExp2Pin8_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

mangoh_gpioExp2Pin8_PullUpDown_t mangoh_gpioExp2Pin8_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 8);
}

mangoh_gpioExp2Pin8_ChangeEventHandlerRef_t mangoh_gpioExp2Pin8_AddChangeEventHandler
(
    mangoh_gpioExp2Pin8_Edge_t trigger,
    mangoh_gpioExp2Pin8_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin8_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        8,
        &handlerRecords[EXPANDER_2_INDEX][8],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin8_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin8_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        8,
        &handlerRecords[EXPANDER_2_INDEX][8],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 9
le_result_t mangoh_gpioExp2Pin9_SetInput
(
    mangoh_gpioExp2Pin9_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 9, polarity);
}

le_result_t mangoh_gpioExp2Pin9_SetPushPullOutput
(
    mangoh_gpioExp2Pin9_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 9, polarity, value);
}

le_result_t mangoh_gpioExp2Pin9_SetTriStateOutput
(
    mangoh_gpioExp2Pin9_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 9, polarity);
}

le_result_t mangoh_gpioExp2Pin9_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin9_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 9, polarity, value);
}

le_result_t mangoh_gpioExp2Pin9_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

le_result_t mangoh_gpioExp2Pin9_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

le_result_t mangoh_gpioExp2Pin9_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

le_result_t mangoh_gpioExp2Pin9_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

le_result_t mangoh_gpioExp2Pin9_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

le_result_t mangoh_gpioExp2Pin9_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

bool mangoh_gpioExp2Pin9_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

le_result_t mangoh_gpioExp2Pin9_SetEdgeSense
(
    mangoh_gpioExp2Pin9_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 9, trigger);
}

mangoh_gpioExp2Pin9_Edge_t mangoh_gpioExp2Pin9_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

le_result_t mangoh_gpioExp2Pin9_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

bool mangoh_gpioExp2Pin9_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

bool mangoh_gpioExp2Pin9_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

mangoh_gpioExp2Pin9_Polarity_t mangoh_gpioExp2Pin9_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

bool mangoh_gpioExp2Pin9_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

mangoh_gpioExp2Pin9_PullUpDown_t mangoh_gpioExp2Pin9_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 9);
}

mangoh_gpioExp2Pin9_ChangeEventHandlerRef_t mangoh_gpioExp2Pin9_AddChangeEventHandler
(
    mangoh_gpioExp2Pin9_Edge_t trigger,
    mangoh_gpioExp2Pin9_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin9_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        9,
        &handlerRecords[EXPANDER_2_INDEX][9],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin9_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin9_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        9,
        &handlerRecords[EXPANDER_2_INDEX][9],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 10
le_result_t mangoh_gpioExp2Pin10_SetInput
(
    mangoh_gpioExp2Pin10_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 10, polarity);
}

le_result_t mangoh_gpioExp2Pin10_SetPushPullOutput
(
    mangoh_gpioExp2Pin10_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 10, polarity, value);
}

le_result_t mangoh_gpioExp2Pin10_SetTriStateOutput
(
    mangoh_gpioExp2Pin10_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 10, polarity);
}

le_result_t mangoh_gpioExp2Pin10_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin10_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 10, polarity, value);
}

le_result_t mangoh_gpioExp2Pin10_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

le_result_t mangoh_gpioExp2Pin10_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

le_result_t mangoh_gpioExp2Pin10_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

le_result_t mangoh_gpioExp2Pin10_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

le_result_t mangoh_gpioExp2Pin10_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

le_result_t mangoh_gpioExp2Pin10_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

bool mangoh_gpioExp2Pin10_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

le_result_t mangoh_gpioExp2Pin10_SetEdgeSense
(
    mangoh_gpioExp2Pin10_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 10, trigger);
}

mangoh_gpioExp2Pin10_Edge_t mangoh_gpioExp2Pin10_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

le_result_t mangoh_gpioExp2Pin10_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

bool mangoh_gpioExp2Pin10_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

bool mangoh_gpioExp2Pin10_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

mangoh_gpioExp2Pin10_Polarity_t mangoh_gpioExp2Pin10_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

bool mangoh_gpioExp2Pin10_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

mangoh_gpioExp2Pin10_PullUpDown_t mangoh_gpioExp2Pin10_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 10);
}

mangoh_gpioExp2Pin10_ChangeEventHandlerRef_t mangoh_gpioExp2Pin10_AddChangeEventHandler
(
    mangoh_gpioExp2Pin10_Edge_t trigger,
    mangoh_gpioExp2Pin10_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin10_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        10,
        &handlerRecords[EXPANDER_2_INDEX][10],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin10_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin10_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        10,
        &handlerRecords[EXPANDER_2_INDEX][10],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 11
le_result_t mangoh_gpioExp2Pin11_SetInput
(
    mangoh_gpioExp2Pin11_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 11, polarity);
}

le_result_t mangoh_gpioExp2Pin11_SetPushPullOutput
(
    mangoh_gpioExp2Pin11_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 11, polarity, value);
}

le_result_t mangoh_gpioExp2Pin11_SetTriStateOutput
(
    mangoh_gpioExp2Pin11_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 11, polarity);
}

le_result_t mangoh_gpioExp2Pin11_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin11_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 11, polarity, value);
}

le_result_t mangoh_gpioExp2Pin11_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

le_result_t mangoh_gpioExp2Pin11_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

le_result_t mangoh_gpioExp2Pin11_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

le_result_t mangoh_gpioExp2Pin11_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

le_result_t mangoh_gpioExp2Pin11_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

le_result_t mangoh_gpioExp2Pin11_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

bool mangoh_gpioExp2Pin11_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

le_result_t mangoh_gpioExp2Pin11_SetEdgeSense
(
    mangoh_gpioExp2Pin11_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 11, trigger);
}

mangoh_gpioExp2Pin11_Edge_t mangoh_gpioExp2Pin11_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

le_result_t mangoh_gpioExp2Pin11_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

bool mangoh_gpioExp2Pin11_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

bool mangoh_gpioExp2Pin11_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

mangoh_gpioExp2Pin11_Polarity_t mangoh_gpioExp2Pin11_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

bool mangoh_gpioExp2Pin11_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

mangoh_gpioExp2Pin11_PullUpDown_t mangoh_gpioExp2Pin11_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 11);
}

mangoh_gpioExp2Pin11_ChangeEventHandlerRef_t mangoh_gpioExp2Pin11_AddChangeEventHandler
(
    mangoh_gpioExp2Pin11_Edge_t trigger,
    mangoh_gpioExp2Pin11_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin11_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        11,
        &handlerRecords[EXPANDER_2_INDEX][11],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin11_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin11_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        11,
        &handlerRecords[EXPANDER_2_INDEX][11],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 12
le_result_t mangoh_gpioExp2Pin12_SetInput
(
    mangoh_gpioExp2Pin12_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 12, polarity);
}

le_result_t mangoh_gpioExp2Pin12_SetPushPullOutput
(
    mangoh_gpioExp2Pin12_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 12, polarity, value);
}

le_result_t mangoh_gpioExp2Pin12_SetTriStateOutput
(
    mangoh_gpioExp2Pin12_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 12, polarity);
}

le_result_t mangoh_gpioExp2Pin12_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin12_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 12, polarity, value);
}

le_result_t mangoh_gpioExp2Pin12_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

le_result_t mangoh_gpioExp2Pin12_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

le_result_t mangoh_gpioExp2Pin12_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

le_result_t mangoh_gpioExp2Pin12_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

le_result_t mangoh_gpioExp2Pin12_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

le_result_t mangoh_gpioExp2Pin12_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

bool mangoh_gpioExp2Pin12_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

le_result_t mangoh_gpioExp2Pin12_SetEdgeSense
(
    mangoh_gpioExp2Pin12_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 12, trigger);
}

mangoh_gpioExp2Pin12_Edge_t mangoh_gpioExp2Pin12_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

le_result_t mangoh_gpioExp2Pin12_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

bool mangoh_gpioExp2Pin12_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

bool mangoh_gpioExp2Pin12_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

mangoh_gpioExp2Pin12_Polarity_t mangoh_gpioExp2Pin12_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

bool mangoh_gpioExp2Pin12_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

mangoh_gpioExp2Pin12_PullUpDown_t mangoh_gpioExp2Pin12_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 12);
}

mangoh_gpioExp2Pin12_ChangeEventHandlerRef_t mangoh_gpioExp2Pin12_AddChangeEventHandler
(
    mangoh_gpioExp2Pin12_Edge_t trigger,
    mangoh_gpioExp2Pin12_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin12_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        12,
        &handlerRecords[EXPANDER_2_INDEX][12],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin12_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin12_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        12,
        &handlerRecords[EXPANDER_2_INDEX][12],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 13
le_result_t mangoh_gpioExp2Pin13_SetInput
(
    mangoh_gpioExp2Pin13_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 13, polarity);
}

le_result_t mangoh_gpioExp2Pin13_SetPushPullOutput
(
    mangoh_gpioExp2Pin13_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 13, polarity, value);
}

le_result_t mangoh_gpioExp2Pin13_SetTriStateOutput
(
    mangoh_gpioExp2Pin13_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 13, polarity);
}

le_result_t mangoh_gpioExp2Pin13_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin13_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 13, polarity, value);
}

le_result_t mangoh_gpioExp2Pin13_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

le_result_t mangoh_gpioExp2Pin13_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

le_result_t mangoh_gpioExp2Pin13_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

le_result_t mangoh_gpioExp2Pin13_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

le_result_t mangoh_gpioExp2Pin13_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

le_result_t mangoh_gpioExp2Pin13_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

bool mangoh_gpioExp2Pin13_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

le_result_t mangoh_gpioExp2Pin13_SetEdgeSense
(
    mangoh_gpioExp2Pin13_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 13, trigger);
}

mangoh_gpioExp2Pin13_Edge_t mangoh_gpioExp2Pin13_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

le_result_t mangoh_gpioExp2Pin13_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

bool mangoh_gpioExp2Pin13_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

bool mangoh_gpioExp2Pin13_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

mangoh_gpioExp2Pin13_Polarity_t mangoh_gpioExp2Pin13_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

bool mangoh_gpioExp2Pin13_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

mangoh_gpioExp2Pin13_PullUpDown_t mangoh_gpioExp2Pin13_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 13);
}

mangoh_gpioExp2Pin13_ChangeEventHandlerRef_t mangoh_gpioExp2Pin13_AddChangeEventHandler
(
    mangoh_gpioExp2Pin13_Edge_t trigger,
    mangoh_gpioExp2Pin13_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin13_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        13,
        &handlerRecords[EXPANDER_2_INDEX][13],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin13_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin13_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        13,
        &handlerRecords[EXPANDER_2_INDEX][13],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 14
le_result_t mangoh_gpioExp2Pin14_SetInput
(
    mangoh_gpioExp2Pin14_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 14, polarity);
}

le_result_t mangoh_gpioExp2Pin14_SetPushPullOutput
(
    mangoh_gpioExp2Pin14_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 14, polarity, value);
}

le_result_t mangoh_gpioExp2Pin14_SetTriStateOutput
(
    mangoh_gpioExp2Pin14_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 14, polarity);
}

le_result_t mangoh_gpioExp2Pin14_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin14_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 14, polarity, value);
}

le_result_t mangoh_gpioExp2Pin14_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

le_result_t mangoh_gpioExp2Pin14_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

le_result_t mangoh_gpioExp2Pin14_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

le_result_t mangoh_gpioExp2Pin14_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

le_result_t mangoh_gpioExp2Pin14_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

le_result_t mangoh_gpioExp2Pin14_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

bool mangoh_gpioExp2Pin14_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

le_result_t mangoh_gpioExp2Pin14_SetEdgeSense
(
    mangoh_gpioExp2Pin14_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 14, trigger);
}

mangoh_gpioExp2Pin14_Edge_t mangoh_gpioExp2Pin14_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

le_result_t mangoh_gpioExp2Pin14_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

bool mangoh_gpioExp2Pin14_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

bool mangoh_gpioExp2Pin14_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

mangoh_gpioExp2Pin14_Polarity_t mangoh_gpioExp2Pin14_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

bool mangoh_gpioExp2Pin14_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

mangoh_gpioExp2Pin14_PullUpDown_t mangoh_gpioExp2Pin14_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 14);
}

mangoh_gpioExp2Pin14_ChangeEventHandlerRef_t mangoh_gpioExp2Pin14_AddChangeEventHandler
(
    mangoh_gpioExp2Pin14_Edge_t trigger,
    mangoh_gpioExp2Pin14_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin14_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        14,
        &handlerRecords[EXPANDER_2_INDEX][14],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin14_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin14_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        14,
        &handlerRecords[EXPANDER_2_INDEX][14],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #2 GPIO 15
le_result_t mangoh_gpioExp2Pin15_SetInput
(
    mangoh_gpioExp2Pin15_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_2_INDEX], 15, polarity);
}

le_result_t mangoh_gpioExp2Pin15_SetPushPullOutput
(
    mangoh_gpioExp2Pin15_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 15, polarity, value);
}

le_result_t mangoh_gpioExp2Pin15_SetTriStateOutput
(
    mangoh_gpioExp2Pin15_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_2_INDEX], 15, polarity);
}

le_result_t mangoh_gpioExp2Pin15_SetOpenDrainOutput
(
    mangoh_gpioExp2Pin15_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_2_INDEX], 15, polarity, value);
}

le_result_t mangoh_gpioExp2Pin15_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

le_result_t mangoh_gpioExp2Pin15_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

le_result_t mangoh_gpioExp2Pin15_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

le_result_t mangoh_gpioExp2Pin15_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

le_result_t mangoh_gpioExp2Pin15_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

le_result_t mangoh_gpioExp2Pin15_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

bool mangoh_gpioExp2Pin15_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

le_result_t mangoh_gpioExp2Pin15_SetEdgeSense
(
    mangoh_gpioExp2Pin15_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 15, trigger);
}

mangoh_gpioExp2Pin15_Edge_t mangoh_gpioExp2Pin15_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

le_result_t mangoh_gpioExp2Pin15_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

bool mangoh_gpioExp2Pin15_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

bool mangoh_gpioExp2Pin15_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

mangoh_gpioExp2Pin15_Polarity_t mangoh_gpioExp2Pin15_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

bool mangoh_gpioExp2Pin15_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

mangoh_gpioExp2Pin15_PullUpDown_t mangoh_gpioExp2Pin15_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_2_INDEX], 15);
}

mangoh_gpioExp2Pin15_ChangeEventHandlerRef_t mangoh_gpioExp2Pin15_AddChangeEventHandler
(
    mangoh_gpioExp2Pin15_Edge_t trigger,
    mangoh_gpioExp2Pin15_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp2Pin15_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        15,
        &handlerRecords[EXPANDER_2_INDEX][15],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp2Pin15_RemoveChangeEventHandler
(
    mangoh_gpioExp2Pin15_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_2_INDEX],
        15,
        &handlerRecords[EXPANDER_2_INDEX][15],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 0
le_result_t mangoh_gpioExp3Pin0_SetInput
(
    mangoh_gpioExp3Pin0_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 0, polarity);
}

le_result_t mangoh_gpioExp3Pin0_SetPushPullOutput
(
    mangoh_gpioExp3Pin0_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 0, polarity, value);
}

le_result_t mangoh_gpioExp3Pin0_SetTriStateOutput
(
    mangoh_gpioExp3Pin0_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 0, polarity);
}

le_result_t mangoh_gpioExp3Pin0_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin0_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 0, polarity, value);
}

le_result_t mangoh_gpioExp3Pin0_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

le_result_t mangoh_gpioExp3Pin0_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

le_result_t mangoh_gpioExp3Pin0_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

le_result_t mangoh_gpioExp3Pin0_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

le_result_t mangoh_gpioExp3Pin0_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

le_result_t mangoh_gpioExp3Pin0_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

bool mangoh_gpioExp3Pin0_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

le_result_t mangoh_gpioExp3Pin0_SetEdgeSense
(
    mangoh_gpioExp3Pin0_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 0, trigger);
}

mangoh_gpioExp3Pin0_Edge_t mangoh_gpioExp3Pin0_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

le_result_t mangoh_gpioExp3Pin0_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

bool mangoh_gpioExp3Pin0_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

bool mangoh_gpioExp3Pin0_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

mangoh_gpioExp3Pin0_Polarity_t mangoh_gpioExp3Pin0_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

bool mangoh_gpioExp3Pin0_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

mangoh_gpioExp3Pin0_PullUpDown_t mangoh_gpioExp3Pin0_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 0);
}

mangoh_gpioExp3Pin0_ChangeEventHandlerRef_t mangoh_gpioExp3Pin0_AddChangeEventHandler
(
    mangoh_gpioExp3Pin0_Edge_t trigger,
    mangoh_gpioExp3Pin0_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin0_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        0,
        &handlerRecords[EXPANDER_3_INDEX][0],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin0_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin0_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        0,
        &handlerRecords[EXPANDER_3_INDEX][0],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 1
le_result_t mangoh_gpioExp3Pin1_SetInput
(
    mangoh_gpioExp3Pin1_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 1, polarity);
}

le_result_t mangoh_gpioExp3Pin1_SetPushPullOutput
(
    mangoh_gpioExp3Pin1_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 1, polarity, value);
}

le_result_t mangoh_gpioExp3Pin1_SetTriStateOutput
(
    mangoh_gpioExp3Pin1_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 1, polarity);
}

le_result_t mangoh_gpioExp3Pin1_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin1_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 1, polarity, value);
}

le_result_t mangoh_gpioExp3Pin1_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

le_result_t mangoh_gpioExp3Pin1_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

le_result_t mangoh_gpioExp3Pin1_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

le_result_t mangoh_gpioExp3Pin1_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

le_result_t mangoh_gpioExp3Pin1_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

le_result_t mangoh_gpioExp3Pin1_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

bool mangoh_gpioExp3Pin1_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

le_result_t mangoh_gpioExp3Pin1_SetEdgeSense
(
    mangoh_gpioExp3Pin1_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 1, trigger);
}

mangoh_gpioExp3Pin1_Edge_t mangoh_gpioExp3Pin1_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

le_result_t mangoh_gpioExp3Pin1_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

bool mangoh_gpioExp3Pin1_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

bool mangoh_gpioExp3Pin1_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

mangoh_gpioExp3Pin1_Polarity_t mangoh_gpioExp3Pin1_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

bool mangoh_gpioExp3Pin1_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

mangoh_gpioExp3Pin1_PullUpDown_t mangoh_gpioExp3Pin1_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 1);
}

mangoh_gpioExp3Pin1_ChangeEventHandlerRef_t mangoh_gpioExp3Pin1_AddChangeEventHandler
(
    mangoh_gpioExp3Pin1_Edge_t trigger,
    mangoh_gpioExp3Pin1_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin1_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        1,
        &handlerRecords[EXPANDER_3_INDEX][1],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin1_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin1_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        1,
        &handlerRecords[EXPANDER_3_INDEX][1],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 2
le_result_t mangoh_gpioExp3Pin2_SetInput
(
    mangoh_gpioExp3Pin2_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 2, polarity);
}

le_result_t mangoh_gpioExp3Pin2_SetPushPullOutput
(
    mangoh_gpioExp3Pin2_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 2, polarity, value);
}

le_result_t mangoh_gpioExp3Pin2_SetTriStateOutput
(
    mangoh_gpioExp3Pin2_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 2, polarity);
}

le_result_t mangoh_gpioExp3Pin2_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin2_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 2, polarity, value);
}

le_result_t mangoh_gpioExp3Pin2_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

le_result_t mangoh_gpioExp3Pin2_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

le_result_t mangoh_gpioExp3Pin2_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

le_result_t mangoh_gpioExp3Pin2_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

le_result_t mangoh_gpioExp3Pin2_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

le_result_t mangoh_gpioExp3Pin2_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

bool mangoh_gpioExp3Pin2_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

le_result_t mangoh_gpioExp3Pin2_SetEdgeSense
(
    mangoh_gpioExp3Pin2_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 2, trigger);
}

mangoh_gpioExp3Pin2_Edge_t mangoh_gpioExp3Pin2_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

le_result_t mangoh_gpioExp3Pin2_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

bool mangoh_gpioExp3Pin2_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

bool mangoh_gpioExp3Pin2_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

mangoh_gpioExp3Pin2_Polarity_t mangoh_gpioExp3Pin2_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

bool mangoh_gpioExp3Pin2_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

mangoh_gpioExp3Pin2_PullUpDown_t mangoh_gpioExp3Pin2_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 2);
}

mangoh_gpioExp3Pin2_ChangeEventHandlerRef_t mangoh_gpioExp3Pin2_AddChangeEventHandler
(
    mangoh_gpioExp3Pin2_Edge_t trigger,
    mangoh_gpioExp3Pin2_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin2_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        2,
        &handlerRecords[EXPANDER_3_INDEX][2],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin2_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin2_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        2,
        &handlerRecords[EXPANDER_3_INDEX][2],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 3
le_result_t mangoh_gpioExp3Pin3_SetInput
(
    mangoh_gpioExp3Pin3_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 3, polarity);
}

le_result_t mangoh_gpioExp3Pin3_SetPushPullOutput
(
    mangoh_gpioExp3Pin3_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 3, polarity, value);
}

le_result_t mangoh_gpioExp3Pin3_SetTriStateOutput
(
    mangoh_gpioExp3Pin3_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 3, polarity);
}

le_result_t mangoh_gpioExp3Pin3_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin3_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 3, polarity, value);
}

le_result_t mangoh_gpioExp3Pin3_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

le_result_t mangoh_gpioExp3Pin3_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

le_result_t mangoh_gpioExp3Pin3_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

le_result_t mangoh_gpioExp3Pin3_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

le_result_t mangoh_gpioExp3Pin3_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

le_result_t mangoh_gpioExp3Pin3_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

bool mangoh_gpioExp3Pin3_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

le_result_t mangoh_gpioExp3Pin3_SetEdgeSense
(
    mangoh_gpioExp3Pin3_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 3, trigger);
}

mangoh_gpioExp3Pin3_Edge_t mangoh_gpioExp3Pin3_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

le_result_t mangoh_gpioExp3Pin3_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

bool mangoh_gpioExp3Pin3_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

bool mangoh_gpioExp3Pin3_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

mangoh_gpioExp3Pin3_Polarity_t mangoh_gpioExp3Pin3_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

bool mangoh_gpioExp3Pin3_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

mangoh_gpioExp3Pin3_PullUpDown_t mangoh_gpioExp3Pin3_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 3);
}

mangoh_gpioExp3Pin3_ChangeEventHandlerRef_t mangoh_gpioExp3Pin3_AddChangeEventHandler
(
    mangoh_gpioExp3Pin3_Edge_t trigger,
    mangoh_gpioExp3Pin3_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin3_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        3,
        &handlerRecords[EXPANDER_3_INDEX][3],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin3_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin3_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        3,
        &handlerRecords[EXPANDER_3_INDEX][3],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 4
le_result_t mangoh_gpioExp3Pin4_SetInput
(
    mangoh_gpioExp3Pin4_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 4, polarity);
}

le_result_t mangoh_gpioExp3Pin4_SetPushPullOutput
(
    mangoh_gpioExp3Pin4_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 4, polarity, value);
}

le_result_t mangoh_gpioExp3Pin4_SetTriStateOutput
(
    mangoh_gpioExp3Pin4_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 4, polarity);
}

le_result_t mangoh_gpioExp3Pin4_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin4_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 4, polarity, value);
}

le_result_t mangoh_gpioExp3Pin4_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

le_result_t mangoh_gpioExp3Pin4_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

le_result_t mangoh_gpioExp3Pin4_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

le_result_t mangoh_gpioExp3Pin4_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

le_result_t mangoh_gpioExp3Pin4_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

le_result_t mangoh_gpioExp3Pin4_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

bool mangoh_gpioExp3Pin4_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

le_result_t mangoh_gpioExp3Pin4_SetEdgeSense
(
    mangoh_gpioExp3Pin4_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 4, trigger);
}

mangoh_gpioExp3Pin4_Edge_t mangoh_gpioExp3Pin4_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

le_result_t mangoh_gpioExp3Pin4_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

bool mangoh_gpioExp3Pin4_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

bool mangoh_gpioExp3Pin4_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

mangoh_gpioExp3Pin4_Polarity_t mangoh_gpioExp3Pin4_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

bool mangoh_gpioExp3Pin4_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

mangoh_gpioExp3Pin4_PullUpDown_t mangoh_gpioExp3Pin4_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 4);
}

mangoh_gpioExp3Pin4_ChangeEventHandlerRef_t mangoh_gpioExp3Pin4_AddChangeEventHandler
(
    mangoh_gpioExp3Pin4_Edge_t trigger,
    mangoh_gpioExp3Pin4_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin4_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        4,
        &handlerRecords[EXPANDER_3_INDEX][4],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin4_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin4_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        4,
        &handlerRecords[EXPANDER_3_INDEX][4],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 5
le_result_t mangoh_gpioExp3Pin5_SetInput
(
    mangoh_gpioExp3Pin5_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 5, polarity);
}

le_result_t mangoh_gpioExp3Pin5_SetPushPullOutput
(
    mangoh_gpioExp3Pin5_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 5, polarity, value);
}

le_result_t mangoh_gpioExp3Pin5_SetTriStateOutput
(
    mangoh_gpioExp3Pin5_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 5, polarity);
}

le_result_t mangoh_gpioExp3Pin5_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin5_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 5, polarity, value);
}

le_result_t mangoh_gpioExp3Pin5_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

le_result_t mangoh_gpioExp3Pin5_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

le_result_t mangoh_gpioExp3Pin5_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

le_result_t mangoh_gpioExp3Pin5_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

le_result_t mangoh_gpioExp3Pin5_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

le_result_t mangoh_gpioExp3Pin5_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

bool mangoh_gpioExp3Pin5_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

le_result_t mangoh_gpioExp3Pin5_SetEdgeSense
(
    mangoh_gpioExp3Pin5_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 5, trigger);
}

mangoh_gpioExp3Pin5_Edge_t mangoh_gpioExp3Pin5_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

le_result_t mangoh_gpioExp3Pin5_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

bool mangoh_gpioExp3Pin5_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

bool mangoh_gpioExp3Pin5_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

mangoh_gpioExp3Pin5_Polarity_t mangoh_gpioExp3Pin5_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

bool mangoh_gpioExp3Pin5_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

mangoh_gpioExp3Pin5_PullUpDown_t mangoh_gpioExp3Pin5_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 5);
}

mangoh_gpioExp3Pin5_ChangeEventHandlerRef_t mangoh_gpioExp3Pin5_AddChangeEventHandler
(
    mangoh_gpioExp3Pin5_Edge_t trigger,
    mangoh_gpioExp3Pin5_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin5_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        5,
        &handlerRecords[EXPANDER_3_INDEX][5],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin5_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin5_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        5,
        &handlerRecords[EXPANDER_3_INDEX][5],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 6
le_result_t mangoh_gpioExp3Pin6_SetInput
(
    mangoh_gpioExp3Pin6_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 6, polarity);
}

le_result_t mangoh_gpioExp3Pin6_SetPushPullOutput
(
    mangoh_gpioExp3Pin6_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 6, polarity, value);
}

le_result_t mangoh_gpioExp3Pin6_SetTriStateOutput
(
    mangoh_gpioExp3Pin6_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 6, polarity);
}

le_result_t mangoh_gpioExp3Pin6_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin6_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 6, polarity, value);
}

le_result_t mangoh_gpioExp3Pin6_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

le_result_t mangoh_gpioExp3Pin6_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

le_result_t mangoh_gpioExp3Pin6_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

le_result_t mangoh_gpioExp3Pin6_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

le_result_t mangoh_gpioExp3Pin6_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

le_result_t mangoh_gpioExp3Pin6_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

bool mangoh_gpioExp3Pin6_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

le_result_t mangoh_gpioExp3Pin6_SetEdgeSense
(
    mangoh_gpioExp3Pin6_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 6, trigger);
}

mangoh_gpioExp3Pin6_Edge_t mangoh_gpioExp3Pin6_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

le_result_t mangoh_gpioExp3Pin6_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

bool mangoh_gpioExp3Pin6_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

bool mangoh_gpioExp3Pin6_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

mangoh_gpioExp3Pin6_Polarity_t mangoh_gpioExp3Pin6_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

bool mangoh_gpioExp3Pin6_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

mangoh_gpioExp3Pin6_PullUpDown_t mangoh_gpioExp3Pin6_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 6);
}

mangoh_gpioExp3Pin6_ChangeEventHandlerRef_t mangoh_gpioExp3Pin6_AddChangeEventHandler
(
    mangoh_gpioExp3Pin6_Edge_t trigger,
    mangoh_gpioExp3Pin6_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin6_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        6,
        &handlerRecords[EXPANDER_3_INDEX][6],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin6_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin6_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        6,
        &handlerRecords[EXPANDER_3_INDEX][6],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 7
le_result_t mangoh_gpioExp3Pin7_SetInput
(
    mangoh_gpioExp3Pin7_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 7, polarity);
}

le_result_t mangoh_gpioExp3Pin7_SetPushPullOutput
(
    mangoh_gpioExp3Pin7_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 7, polarity, value);
}

le_result_t mangoh_gpioExp3Pin7_SetTriStateOutput
(
    mangoh_gpioExp3Pin7_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 7, polarity);
}

le_result_t mangoh_gpioExp3Pin7_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin7_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 7, polarity, value);
}

le_result_t mangoh_gpioExp3Pin7_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

le_result_t mangoh_gpioExp3Pin7_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

le_result_t mangoh_gpioExp3Pin7_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

le_result_t mangoh_gpioExp3Pin7_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

le_result_t mangoh_gpioExp3Pin7_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

le_result_t mangoh_gpioExp3Pin7_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

bool mangoh_gpioExp3Pin7_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

le_result_t mangoh_gpioExp3Pin7_SetEdgeSense
(
    mangoh_gpioExp3Pin7_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 7, trigger);
}

mangoh_gpioExp3Pin7_Edge_t mangoh_gpioExp3Pin7_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

le_result_t mangoh_gpioExp3Pin7_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

bool mangoh_gpioExp3Pin7_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

bool mangoh_gpioExp3Pin7_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

mangoh_gpioExp3Pin7_Polarity_t mangoh_gpioExp3Pin7_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

bool mangoh_gpioExp3Pin7_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

mangoh_gpioExp3Pin7_PullUpDown_t mangoh_gpioExp3Pin7_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 7);
}

mangoh_gpioExp3Pin7_ChangeEventHandlerRef_t mangoh_gpioExp3Pin7_AddChangeEventHandler
(
    mangoh_gpioExp3Pin7_Edge_t trigger,
    mangoh_gpioExp3Pin7_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin7_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        7,
        &handlerRecords[EXPANDER_3_INDEX][7],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin7_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin7_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        7,
        &handlerRecords[EXPANDER_3_INDEX][7],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 8
le_result_t mangoh_gpioExp3Pin8_SetInput
(
    mangoh_gpioExp3Pin8_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 8, polarity);
}

le_result_t mangoh_gpioExp3Pin8_SetPushPullOutput
(
    mangoh_gpioExp3Pin8_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 8, polarity, value);
}

le_result_t mangoh_gpioExp3Pin8_SetTriStateOutput
(
    mangoh_gpioExp3Pin8_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 8, polarity);
}

le_result_t mangoh_gpioExp3Pin8_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin8_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 8, polarity, value);
}

le_result_t mangoh_gpioExp3Pin8_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

le_result_t mangoh_gpioExp3Pin8_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

le_result_t mangoh_gpioExp3Pin8_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

le_result_t mangoh_gpioExp3Pin8_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

le_result_t mangoh_gpioExp3Pin8_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

le_result_t mangoh_gpioExp3Pin8_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

bool mangoh_gpioExp3Pin8_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

le_result_t mangoh_gpioExp3Pin8_SetEdgeSense
(
    mangoh_gpioExp3Pin8_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 8, trigger);
}

mangoh_gpioExp3Pin8_Edge_t mangoh_gpioExp3Pin8_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

le_result_t mangoh_gpioExp3Pin8_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

bool mangoh_gpioExp3Pin8_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

bool mangoh_gpioExp3Pin8_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

mangoh_gpioExp3Pin8_Polarity_t mangoh_gpioExp3Pin8_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

bool mangoh_gpioExp3Pin8_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

mangoh_gpioExp3Pin8_PullUpDown_t mangoh_gpioExp3Pin8_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 8);
}

mangoh_gpioExp3Pin8_ChangeEventHandlerRef_t mangoh_gpioExp3Pin8_AddChangeEventHandler
(
    mangoh_gpioExp3Pin8_Edge_t trigger,
    mangoh_gpioExp3Pin8_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin8_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        8,
        &handlerRecords[EXPANDER_3_INDEX][8],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin8_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin8_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        8,
        &handlerRecords[EXPANDER_3_INDEX][8],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 9
le_result_t mangoh_gpioExp3Pin9_SetInput
(
    mangoh_gpioExp3Pin9_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 9, polarity);
}

le_result_t mangoh_gpioExp3Pin9_SetPushPullOutput
(
    mangoh_gpioExp3Pin9_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 9, polarity, value);
}

le_result_t mangoh_gpioExp3Pin9_SetTriStateOutput
(
    mangoh_gpioExp3Pin9_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 9, polarity);
}

le_result_t mangoh_gpioExp3Pin9_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin9_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 9, polarity, value);
}

le_result_t mangoh_gpioExp3Pin9_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

le_result_t mangoh_gpioExp3Pin9_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

le_result_t mangoh_gpioExp3Pin9_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

le_result_t mangoh_gpioExp3Pin9_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

le_result_t mangoh_gpioExp3Pin9_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

le_result_t mangoh_gpioExp3Pin9_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

bool mangoh_gpioExp3Pin9_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

le_result_t mangoh_gpioExp3Pin9_SetEdgeSense
(
    mangoh_gpioExp3Pin9_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 9, trigger);
}

mangoh_gpioExp3Pin9_Edge_t mangoh_gpioExp3Pin9_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

le_result_t mangoh_gpioExp3Pin9_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

bool mangoh_gpioExp3Pin9_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

bool mangoh_gpioExp3Pin9_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

mangoh_gpioExp3Pin9_Polarity_t mangoh_gpioExp3Pin9_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

bool mangoh_gpioExp3Pin9_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

mangoh_gpioExp3Pin9_PullUpDown_t mangoh_gpioExp3Pin9_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 9);
}

mangoh_gpioExp3Pin9_ChangeEventHandlerRef_t mangoh_gpioExp3Pin9_AddChangeEventHandler
(
    mangoh_gpioExp3Pin9_Edge_t trigger,
    mangoh_gpioExp3Pin9_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin9_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        9,
        &handlerRecords[EXPANDER_3_INDEX][9],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin9_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin9_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        9,
        &handlerRecords[EXPANDER_3_INDEX][9],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 10
le_result_t mangoh_gpioExp3Pin10_SetInput
(
    mangoh_gpioExp3Pin10_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 10, polarity);
}

le_result_t mangoh_gpioExp3Pin10_SetPushPullOutput
(
    mangoh_gpioExp3Pin10_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 10, polarity, value);
}

le_result_t mangoh_gpioExp3Pin10_SetTriStateOutput
(
    mangoh_gpioExp3Pin10_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 10, polarity);
}

le_result_t mangoh_gpioExp3Pin10_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin10_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 10, polarity, value);
}

le_result_t mangoh_gpioExp3Pin10_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

le_result_t mangoh_gpioExp3Pin10_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

le_result_t mangoh_gpioExp3Pin10_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

le_result_t mangoh_gpioExp3Pin10_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

le_result_t mangoh_gpioExp3Pin10_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

le_result_t mangoh_gpioExp3Pin10_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

bool mangoh_gpioExp3Pin10_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

le_result_t mangoh_gpioExp3Pin10_SetEdgeSense
(
    mangoh_gpioExp3Pin10_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 10, trigger);
}

mangoh_gpioExp3Pin10_Edge_t mangoh_gpioExp3Pin10_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

le_result_t mangoh_gpioExp3Pin10_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

bool mangoh_gpioExp3Pin10_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

bool mangoh_gpioExp3Pin10_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

mangoh_gpioExp3Pin10_Polarity_t mangoh_gpioExp3Pin10_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

bool mangoh_gpioExp3Pin10_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

mangoh_gpioExp3Pin10_PullUpDown_t mangoh_gpioExp3Pin10_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 10);
}

mangoh_gpioExp3Pin10_ChangeEventHandlerRef_t mangoh_gpioExp3Pin10_AddChangeEventHandler
(
    mangoh_gpioExp3Pin10_Edge_t trigger,
    mangoh_gpioExp3Pin10_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin10_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        10,
        &handlerRecords[EXPANDER_3_INDEX][10],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin10_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin10_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        10,
        &handlerRecords[EXPANDER_3_INDEX][10],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 11
le_result_t mangoh_gpioExp3Pin11_SetInput
(
    mangoh_gpioExp3Pin11_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 11, polarity);
}

le_result_t mangoh_gpioExp3Pin11_SetPushPullOutput
(
    mangoh_gpioExp3Pin11_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 11, polarity, value);
}

le_result_t mangoh_gpioExp3Pin11_SetTriStateOutput
(
    mangoh_gpioExp3Pin11_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 11, polarity);
}

le_result_t mangoh_gpioExp3Pin11_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin11_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 11, polarity, value);
}

le_result_t mangoh_gpioExp3Pin11_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

le_result_t mangoh_gpioExp3Pin11_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

le_result_t mangoh_gpioExp3Pin11_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

le_result_t mangoh_gpioExp3Pin11_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

le_result_t mangoh_gpioExp3Pin11_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

le_result_t mangoh_gpioExp3Pin11_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

bool mangoh_gpioExp3Pin11_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

le_result_t mangoh_gpioExp3Pin11_SetEdgeSense
(
    mangoh_gpioExp3Pin11_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 11, trigger);
}

mangoh_gpioExp3Pin11_Edge_t mangoh_gpioExp3Pin11_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

le_result_t mangoh_gpioExp3Pin11_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

bool mangoh_gpioExp3Pin11_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

bool mangoh_gpioExp3Pin11_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

mangoh_gpioExp3Pin11_Polarity_t mangoh_gpioExp3Pin11_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

bool mangoh_gpioExp3Pin11_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

mangoh_gpioExp3Pin11_PullUpDown_t mangoh_gpioExp3Pin11_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 11);
}

mangoh_gpioExp3Pin11_ChangeEventHandlerRef_t mangoh_gpioExp3Pin11_AddChangeEventHandler
(
    mangoh_gpioExp3Pin11_Edge_t trigger,
    mangoh_gpioExp3Pin11_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin11_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        11,
        &handlerRecords[EXPANDER_3_INDEX][11],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin11_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin11_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        11,
        &handlerRecords[EXPANDER_3_INDEX][11],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 12
le_result_t mangoh_gpioExp3Pin12_SetInput
(
    mangoh_gpioExp3Pin12_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 12, polarity);
}

le_result_t mangoh_gpioExp3Pin12_SetPushPullOutput
(
    mangoh_gpioExp3Pin12_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 12, polarity, value);
}

le_result_t mangoh_gpioExp3Pin12_SetTriStateOutput
(
    mangoh_gpioExp3Pin12_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 12, polarity);
}

le_result_t mangoh_gpioExp3Pin12_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin12_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 12, polarity, value);
}

le_result_t mangoh_gpioExp3Pin12_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

le_result_t mangoh_gpioExp3Pin12_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

le_result_t mangoh_gpioExp3Pin12_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

le_result_t mangoh_gpioExp3Pin12_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

le_result_t mangoh_gpioExp3Pin12_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

le_result_t mangoh_gpioExp3Pin12_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

bool mangoh_gpioExp3Pin12_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

le_result_t mangoh_gpioExp3Pin12_SetEdgeSense
(
    mangoh_gpioExp3Pin12_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 12, trigger);
}

mangoh_gpioExp3Pin12_Edge_t mangoh_gpioExp3Pin12_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

le_result_t mangoh_gpioExp3Pin12_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

bool mangoh_gpioExp3Pin12_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

bool mangoh_gpioExp3Pin12_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

mangoh_gpioExp3Pin12_Polarity_t mangoh_gpioExp3Pin12_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

bool mangoh_gpioExp3Pin12_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

mangoh_gpioExp3Pin12_PullUpDown_t mangoh_gpioExp3Pin12_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 12);
}

mangoh_gpioExp3Pin12_ChangeEventHandlerRef_t mangoh_gpioExp3Pin12_AddChangeEventHandler
(
    mangoh_gpioExp3Pin12_Edge_t trigger,
    mangoh_gpioExp3Pin12_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin12_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        12,
        &handlerRecords[EXPANDER_3_INDEX][12],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin12_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin12_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        12,
        &handlerRecords[EXPANDER_3_INDEX][12],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 13
le_result_t mangoh_gpioExp3Pin13_SetInput
(
    mangoh_gpioExp3Pin13_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 13, polarity);
}

le_result_t mangoh_gpioExp3Pin13_SetPushPullOutput
(
    mangoh_gpioExp3Pin13_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 13, polarity, value);
}

le_result_t mangoh_gpioExp3Pin13_SetTriStateOutput
(
    mangoh_gpioExp3Pin13_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 13, polarity);
}

le_result_t mangoh_gpioExp3Pin13_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin13_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 13, polarity, value);
}

le_result_t mangoh_gpioExp3Pin13_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

le_result_t mangoh_gpioExp3Pin13_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

le_result_t mangoh_gpioExp3Pin13_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

le_result_t mangoh_gpioExp3Pin13_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

le_result_t mangoh_gpioExp3Pin13_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

le_result_t mangoh_gpioExp3Pin13_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

bool mangoh_gpioExp3Pin13_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

le_result_t mangoh_gpioExp3Pin13_SetEdgeSense
(
    mangoh_gpioExp3Pin13_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 13, trigger);
}

mangoh_gpioExp3Pin13_Edge_t mangoh_gpioExp3Pin13_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

le_result_t mangoh_gpioExp3Pin13_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

bool mangoh_gpioExp3Pin13_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

bool mangoh_gpioExp3Pin13_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

mangoh_gpioExp3Pin13_Polarity_t mangoh_gpioExp3Pin13_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

bool mangoh_gpioExp3Pin13_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

mangoh_gpioExp3Pin13_PullUpDown_t mangoh_gpioExp3Pin13_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 13);
}

mangoh_gpioExp3Pin13_ChangeEventHandlerRef_t mangoh_gpioExp3Pin13_AddChangeEventHandler
(
    mangoh_gpioExp3Pin13_Edge_t trigger,
    mangoh_gpioExp3Pin13_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin13_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        13,
        &handlerRecords[EXPANDER_3_INDEX][13],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin13_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin13_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        13,
        &handlerRecords[EXPANDER_3_INDEX][13],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 14
le_result_t mangoh_gpioExp3Pin14_SetInput
(
    mangoh_gpioExp3Pin14_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 14, polarity);
}

le_result_t mangoh_gpioExp3Pin14_SetPushPullOutput
(
    mangoh_gpioExp3Pin14_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 14, polarity, value);
}

le_result_t mangoh_gpioExp3Pin14_SetTriStateOutput
(
    mangoh_gpioExp3Pin14_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 14, polarity);
}

le_result_t mangoh_gpioExp3Pin14_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin14_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 14, polarity, value);
}

le_result_t mangoh_gpioExp3Pin14_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

le_result_t mangoh_gpioExp3Pin14_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

le_result_t mangoh_gpioExp3Pin14_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

le_result_t mangoh_gpioExp3Pin14_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

le_result_t mangoh_gpioExp3Pin14_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

le_result_t mangoh_gpioExp3Pin14_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

bool mangoh_gpioExp3Pin14_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

le_result_t mangoh_gpioExp3Pin14_SetEdgeSense
(
    mangoh_gpioExp3Pin14_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 14, trigger);
}

mangoh_gpioExp3Pin14_Edge_t mangoh_gpioExp3Pin14_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

le_result_t mangoh_gpioExp3Pin14_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

bool mangoh_gpioExp3Pin14_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

bool mangoh_gpioExp3Pin14_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

mangoh_gpioExp3Pin14_Polarity_t mangoh_gpioExp3Pin14_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

bool mangoh_gpioExp3Pin14_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

mangoh_gpioExp3Pin14_PullUpDown_t mangoh_gpioExp3Pin14_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 14);
}

mangoh_gpioExp3Pin14_ChangeEventHandlerRef_t mangoh_gpioExp3Pin14_AddChangeEventHandler
(
    mangoh_gpioExp3Pin14_Edge_t trigger,
    mangoh_gpioExp3Pin14_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin14_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        14,
        &handlerRecords[EXPANDER_3_INDEX][14],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin14_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin14_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        14,
        &handlerRecords[EXPANDER_3_INDEX][14],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// GPIO expander #3 GPIO 15
le_result_t mangoh_gpioExp3Pin15_SetInput
(
    mangoh_gpioExp3Pin15_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpanders[EXPANDER_3_INDEX], 15, polarity);
}

le_result_t mangoh_gpioExp3Pin15_SetPushPullOutput
(
    mangoh_gpioExp3Pin15_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 15, polarity, value);
}

le_result_t mangoh_gpioExp3Pin15_SetTriStateOutput
(
    mangoh_gpioExp3Pin15_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpanders[EXPANDER_3_INDEX], 15, polarity);
}

le_result_t mangoh_gpioExp3Pin15_SetOpenDrainOutput
(
    mangoh_gpioExp3Pin15_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpanders[EXPANDER_3_INDEX], 15, polarity, value);
}

le_result_t mangoh_gpioExp3Pin15_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

le_result_t mangoh_gpioExp3Pin15_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

le_result_t mangoh_gpioExp3Pin15_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

le_result_t mangoh_gpioExp3Pin15_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

le_result_t mangoh_gpioExp3Pin15_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

le_result_t mangoh_gpioExp3Pin15_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

bool mangoh_gpioExp3Pin15_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

le_result_t mangoh_gpioExp3Pin15_SetEdgeSense
(
    mangoh_gpioExp3Pin15_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 15, trigger);
}

mangoh_gpioExp3Pin15_Edge_t mangoh_gpioExp3Pin15_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

le_result_t mangoh_gpioExp3Pin15_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

bool mangoh_gpioExp3Pin15_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

bool mangoh_gpioExp3Pin15_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

mangoh_gpioExp3Pin15_Polarity_t mangoh_gpioExp3Pin15_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

bool mangoh_gpioExp3Pin15_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

mangoh_gpioExp3Pin15_PullUpDown_t mangoh_gpioExp3Pin15_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpanders[EXPANDER_3_INDEX], 15);
}

mangoh_gpioExp3Pin15_ChangeEventHandlerRef_t mangoh_gpioExp3Pin15_AddChangeEventHandler
(
    mangoh_gpioExp3Pin15_Edge_t trigger,
    mangoh_gpioExp3Pin15_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp3Pin15_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        15,
        &handlerRecords[EXPANDER_3_INDEX][15],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp3Pin15_RemoveChangeEventHandler
(
    mangoh_gpioExp3Pin15_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpanders[EXPANDER_3_INDEX],
        15,
        &handlerRecords[EXPANDER_3_INDEX][15],
        (gpioExpander_ChangeCallbackRef_t)ref);
}

// ----- END GENERATED CODE
