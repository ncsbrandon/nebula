/**
 * @file
 *
 * The core of the driver for the SX1509 GPIO expander chip. This module provides the core
 * functionality of mangoh_gpioExpander.api except that the functions in this module are
 * parameterized with a pin identifier so that the boilerplate required by the 48 wrapper functions
 * can be as small as possible.
 *
 * NOTE:
 * There is a Linux kernel driver for the SX1509.  Once kernel driver loading is better supported
 * by the Legato framework we should probably use the kernel driver.  Then the regular le_gpio.api
 * could be used.  More information here:
 * http://lxr.free-electrons.com/source/include/linux/i2c/sx150x.h
 * http://lxr.free-electrons.com/source/drivers/gpio/gpio-sx150x.c
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */

#include "legato.h"
#include "gpioExpander.h"
#include "i2c-utils.h"

//--------------------------------------------------------------------------------------------------
/**
 * The SX1509 GPIO Register Address - See datasheet for descriptions
 */
//--------------------------------------------------------------------------------------------------
typedef enum {
    // Device and IO Banks
    SX1509_REG_INPUT_DISABLE_B               = 0x00,
    SX1509_REG_INPUT_DISABLE_A               = 0x01,
    SX1509_REG_LONG_SLEW_B                   = 0x02,
    SX1509_REG_LONG_SLEW_A                   = 0x03,
    SX1509_REG_LOW_DRIVE_B                   = 0x04,
    SX1509_REG_LOW_DRIVE_A                   = 0x05,
    SX1509_REG_PULL_UP_B                     = 0x06,
    SX1509_REG_PULL_UP_A                     = 0x07,
    SX1509_REG_PULL_DOWN_B                   = 0x08,
    SX1509_REG_PULL_DOWN_A                   = 0x09,
    SX1509_REG_OPEN_DRAIN_B                  = 0x0A,
    SX1509_REG_OPEN_DRAIN_A                  = 0x0B,
    SX1509_REG_POLARITY_B                    = 0x0C,
    SX1509_REG_POLARITY_A                    = 0x0D,
    SX1509_REG_DIR_B                         = 0x0E,
    SX1509_REG_DIR_A                         = 0x0F,
    SX1509_REG_DATA_B                        = 0x10,
    SX1509_REG_DATA_A                        = 0x11,
    SX1509_REG_INTERRUPT_MASK_B              = 0x12,
    SX1509_REG_INTERRUPT_MASK_A              = 0x13,
    SX1509_REG_SENSE_HIGH_B                  = 0x14,
    SX1509_REG_SENSE_LOW_B                   = 0x15,
    SX1509_REG_SENSE_HIGH_A                  = 0x16,
    SX1509_REG_SENSE_LOW_A                   = 0x17,
    SX1509_REG_INTERRUPT_SOURCE_B            = 0x18,
    SX1509_REG_INTERRUPT_SOURCE_A            = 0x19,
    SX1509_REG_EVENT_STATUS_B                = 0x1A,
    SX1509_REG_EVENT_STATUS_A                = 0x1B,
    SX1509_REG_LEVEL_SHIFTER_1               = 0x1C,
    SX1509_REG_LEVEL_SHIFTER_2               = 0x1D,
    SX1509_REG_CLOCK                         = 0x1E,
    SX1509_REG_MISC                          = 0x1F,
    SX1509_REG_LED_DRIVER_ENABLE_B           = 0x20,
    SX1509_REG_LED_DRIVER_ENABLE_A           = 0x21,
    // Debounce and Keypad Engine
    SX1509_REG_DEBOUNCE_CONFIG               = 0x22,
    SX1509_REG_DEBOUNCE_ENABLE_B             = 0x23,
    SX1509_REG_DEBOUNCE_ENABLE_A             = 0x24,
    SX1509_REG_KEY_CONFIG_1                  = 0x25,
    SX1509_REG_KEY_CONFIG_2                  = 0x26,
    SX1509_REG_KEY_DATA_1                    = 0x27,
    SX1509_REG_KEY_DATA_2                    = 0x28,
    // LED Driver (PWM, blinking, breathing)
    SX1509_REG_T_ON_0                        = 0x29,
    SX1509_REG_I_ON_0                        = 0x2A,
    SX1509_REG_OFF_0                         = 0x2B,
    SX1509_REG_T_ON_1                        = 0x2C,
    SX1509_REG_I_ON_1                        = 0x2D,
    SX1509_REG_OFF_1                         = 0x2E,
    SX1509_REG_T_ON_2                        = 0x2F,
    SX1509_REG_I_ON_2                        = 0x30,
    SX1509_REG_OFF_2                         = 0x31,
    SX1509_REG_T_ON_3                        = 0x32,
    SX1509_REG_I_ON_3                        = 0x33,
    SX1509_REG_OFF_3                         = 0x34,
    SX1509_REG_T_ON_4                        = 0x35,
    SX1509_REG_I_ON_4                        = 0x36,
    SX1509_REG_OFF_4                         = 0x37,
    SX1509_REG_T_RISE_4                      = 0x38,
    SX1509_REG_T_FALL_4                      = 0x39,
    SX1509_REG_T_ON_5                        = 0x3A,
    SX1509_REG_I_ON_5                        = 0x3B,
    SX1509_REG_OFF_5                         = 0x3C,
    SX1509_REG_T_RISE_5                      = 0x3D,
    SX1509_REG_T_FALL_5                      = 0x3E,
    SX1509_REG_T_ON_6                        = 0x3F,
    SX1509_REG_I_ON_6                        = 0x40,
    SX1509_REG_OFF_6                         = 0x41,
    SX1509_REG_T_RISE_6                      = 0x42,
    SX1509_REG_T_FALL_6                      = 0x43,
    SX1509_REG_T_ON_7                        = 0x44,
    SX1509_REG_I_ON_7                        = 0x45,
    SX1509_REG_OFF_7                         = 0x46,
    SX1509_REG_T_RISE_7                      = 0x47,
    SX1509_REG_T_FALL_7                      = 0x48,
    SX1509_REG_T_ON_8                        = 0x49,
    SX1509_REG_I_ON_8                        = 0x4A,
    SX1509_REG_OFF_8                         = 0x4B,
    SX1509_REG_T_ON_9                        = 0x4C,
    SX1509_REG_I_ON_9                        = 0x4D,
    SX1509_REG_OFF_9                         = 0x4E,
    SX1509_REG_T_ON_10                       = 0x4F,
    SX1509_REG_I_ON_10                       = 0x50,
    SX1509_REG_OFF_10                        = 0x51,
    SX1509_REG_T_ON_11                       = 0x52,
    SX1509_REG_I_ON_11                       = 0x53,
    SX1509_REG_OFF_11                        = 0x54,
    SX1509_REG_T_ON_12                       = 0x55,
    SX1509_REG_I_ON_12                       = 0x56,
    SX1509_REG_OFF_12                        = 0x57,
    SX1509_REG_T_RISE_12                     = 0x58,
    SX1509_REG_T_FALL_12                     = 0x59,
    SX1509_REG_T_ON_13                       = 0x5A,
    SX1509_REG_I_ON_13                       = 0x5B,
    SX1509_REG_OFF_13                        = 0x5C,
    SX1509_REG_T_RISE_13                     = 0x5D,
    SX1509_REG_T_FALL_13                     = 0x5E,
    SX1509_REG_T_ON_14                       = 0x5F,
    SX1509_REG_I_ON_14                       = 0x60,
    SX1509_REG_OFF_14                        = 0x61,
    SX1509_REG_T_RISE_14                     = 0x62,
    SX1509_REG_T_FALL_14                     = 0x63,
    SX1509_REG_T_ON_15                       = 0x64,
    SX1509_REG_I_ON_15                       = 0x65,
    SX1509_REG_OFF_15                        = 0x66,
    SX1509_REG_T_RISE_15                     = 0x67,
    SX1509_REG_T_FALL_15                     = 0x68,
    // Miscellaneous
    SX1509_REG_HIGH_INPUT_B                  = 0x69,
    SX1509_REG_HIGH_INPUT_A                  = 0x6A,
    // Software Reset
    SX1509_REG_RESET                         = 0x7D,
    // Test (not to be written)
    SX1509_REG_TEST_1                        = 0x7E,
    SX1509_REG_TEST_2                        = 0x7F,
} Sx1509GpioExpanderReg_t;

typedef enum
{
    SX1509_DIRECTION_OUTPUT = 0,
    SX1509_DIRECTION_INPUT = 1,
} Sx1509_Direction_t;

typedef enum
{
    SX1509_POLARITY_NORMAL = 0,
    SX1509_POLARITY_INVERTED = 1,
} Sx1509_Polarity_t;

typedef enum
{
    GPIO_EXPANDER_OUTPUT_TYPE_PUSH_PULL,
    GPIO_EXPANDER_OUTPUT_TYPE_OPEN_DRAIN,
    GPIO_EXPANDER_OUTPUT_TYPE_TRISTATE,
} gpioExpander_OutputType_t;

//-------------------------------------------------------------------------------------------------
// Static function declarations
//-------------------------------------------------------------------------------------------------

// Wrapper on top of low level linux i2c
static int I2cAccessBusAddr(uint8_t i2cBus, uint8_t i2cAddr);
static le_result_t SmbusReadReg(uint8_t i2cBus, uint8_t i2cAddr, uint8_t reg, uint8_t *data);
static le_result_t SmbusWriteReg(uint8_t i2cBus, uint8_t i2cAddr, uint8_t reg, uint8_t data);

// Low level helper
static le_result_t SmbusReadModifyWrite(
    uint8_t i2cBus, uint8_t i2cAddr, uint8_t reg, uint8_t writeData, uint8_t writeMask);

// Mid-level helpers
static void Sx1509ComputePinFieldAccessParameters(
    uint8_t baseReg,
    uint8_t pinNum,
    uint8_t fieldWidth,
    uint8_t *reg,
    uint8_t *offsetInReg);
static uint8_t CreateMask(uint8_t width);
static uint8_t ExtractField(uint8_t data, uint8_t offset, uint8_t width);

// High level functions
static le_result_t Sx1509ReadPinField(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    uint8_t baseReg,
    uint8_t fieldWidth,
    uint8_t *fieldData);
static le_result_t Sx1509WritePinField(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    uint8_t baseReg,
    uint8_t fieldWidth,
    uint8_t fieldData);

// Helper functions used to implement the public functions
static le_result_t EnableInterrupt(
    const gpioExpander_Identifier_t *expander, uint8_t pin, bool enable);
static le_result_t WriteData(const gpioExpander_Identifier_t *expander, uint8_t pin, bool active);
static le_result_t SetOutput(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_OutputType_t outputType,
    gpioExpander_Polarity_t polarity,
    bool value);
static le_result_t SetPullType(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_PullUpDown_t pullType);
static le_result_t SetPolarity(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_Polarity_t polarity);
static le_result_t SetOutputType(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_OutputType_t outputType);
static le_result_t SetDirection(
    const gpioExpander_Identifier_t *expander, uint8_t pin, bool isInput);


//-------------------------------------------------------------------------------------------------
// Public function definitions
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/**
 * Set the given GPIO as an input with the provided polarity
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t gpioExpander_SetInput
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_Polarity_t polarity
)
{
    if (SetPolarity(expander, pin, polarity) != LE_OK)
    {
        return LE_FAULT;
    }

    if (SetDirection(expander, pin, true) != LE_OK)
    {
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Set the given GPIO as a push pull output with the provided polarity
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t gpioExpander_SetPushPullOutput
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_Polarity_t polarity,
    bool value
)
{
    return SetOutput(expander, pin, GPIO_EXPANDER_OUTPUT_TYPE_PUSH_PULL, polarity, value);
}

//--------------------------------------------------------------------------------------------------
/**
 * Set the given GPIO as a tristate output with the provided polarity
 *
 * @return
 *      - LE_NOT_IMPLEMENTED
 *
 * @note
 *      Tristate output type is not currently supported.  It should be possible to implement by
 *      using the push-pull capability of the hardware for the active and inactive states and
 *      configuring as an input with neither pull-up nor pull-down resistors for the high-z state.
 */
//--------------------------------------------------------------------------------------------------
le_result_t gpioExpander_SetTriStateOutput
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_Polarity_t polarity
)
{
    // TODO: implement tristate
    //return SetOutput(expander, pin, GPIO_EXPANDER_OUTPUT_TYPE_TRISTATE, polarity, value);
    return LE_NOT_IMPLEMENTED;
}

//--------------------------------------------------------------------------------------------------
/**
 * Set the given GPIO as an open drain output with the provided polarity
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t gpioExpander_SetOpenDrainOutput
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_Polarity_t polarity,
    bool value
)
{
    return SetOutput(expander, pin, GPIO_EXPANDER_OUTPUT_TYPE_OPEN_DRAIN, polarity, value);
}

//--------------------------------------------------------------------------------------------------
/**
 * Enable a 42 kΩ pull-up resistor on the GPIO
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t gpioExpander_EnablePullUp
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    return SetPullType(expander, pin, GPIO_EXPANDER_PULL_UP);
}

//--------------------------------------------------------------------------------------------------
/**
 * Enable a 42 kΩ pull-down resistor on the GPIO
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t gpioExpander_EnablePullDown
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    return SetPullType(expander, pin, GPIO_EXPANDER_PULL_DOWN);
}

//--------------------------------------------------------------------------------------------------
/**
 * Disable the pull-up and pull-down resistors on the GPIO
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t gpioExpander_DisableResistors
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    return SetPullType(expander, pin, GPIO_EXPANDER_PULL_OFF);
}

//--------------------------------------------------------------------------------------------------
/**
 * Activate the given GPIO
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t gpioExpander_Activate
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    return WriteData(expander, pin, true);
}

//--------------------------------------------------------------------------------------------------
/**
 * Deactivate the given GPIO
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t gpioExpander_Deactivate
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    return WriteData(expander, pin, false);
}

//--------------------------------------------------------------------------------------------------
/**
 * Set the given GPIO to high-Z
 *
 * @return
 *      - LE_NOT_IMPLEMENTED
 *
 * @note
 *      Once this is implemented, it will only be valid for tristate outputs.
 */
//--------------------------------------------------------------------------------------------------
le_result_t gpioExpander_SetHighZ
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    // TODO: implement
    LE_WARN("SetHighZ API not implemented in GPIO expander service");
    return LE_NOT_IMPLEMENTED;
}

//--------------------------------------------------------------------------------------------------
/**
 * Reads a value for the input GPIO specified
 *
 * @return
 *      - true if the input is active
 *      - false if the input is inactive
 */
//--------------------------------------------------------------------------------------------------
bool gpioExpander_Read
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    const uint8_t dataFieldWidth = 1;
    uint8_t readVal;
    const le_result_t r = Sx1509ReadPinField(
        expander,
        pin,
        SX1509_REG_DATA_A,
        dataFieldWidth,
        &readVal);

    if (r != LE_OK)
    {
        // TODO: There is no way to give fail a read in le_gpio.api
        LE_FATAL("Fault while reading GPIO");
    }

    return readVal == 1;
}

//--------------------------------------------------------------------------------------------------
/**
 * Register the given handler for an edge transition of a GPIO configured as an input
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 *
 * @note
 *      - The sampleMs parameter is not used in the SX1509 GPIO expander implementation
 *      - Only one handler may be registered for each GPIO
 */
//--------------------------------------------------------------------------------------------------
gpioExpander_ChangeCallbackRef_t gpioExpander_AddChangeEventHandler
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_HandlerRecord_t *handlerRecord,
    gpioExpander_Edge_t edge,
    gpioExpander_ChangeCallbackFunc_t handlerPtr,
    void* contextPtr,
    int32_t sampleMs
)
{
    if (handlerRecord->handlerPtr != NULL)
    {
        LE_KILL_CLIENT(
            "Attempted to register a second handler for GPIO expander on I2C bus %d at address "
            "0x%x for pin %d",
            expander->i2cBus,
            expander->i2cAddr,
            pin);
    }
    handlerRecord->handlerPtr = handlerPtr;
    handlerRecord->contextPtr = contextPtr;

    // TODO: We need to find a better way to deal with the unlikely event of a failure.  The
    // function can't return anything except an opaque reference, so we have no way of signalling
    // failure to the client.
    LE_FATAL_IF(
        gpioExpander_SetEdgeSense(expander, pin, edge) != LE_OK,
        "Failed to set edge sense during event handler registration");
    LE_FATAL_IF(
        EnableInterrupt(expander, pin, true) != LE_OK,
        "Failed to enable interrupt during event handler registration");

    return (gpioExpander_ChangeCallbackRef_t)handlerRecord;
}

//--------------------------------------------------------------------------------------------------
/**
 * Deregisters the event handler for changes in the specified pin
 *
 * The calling client will be killed if the provided ref parameter is not the reference that was
 * returned by the call to gpioExpander_AddChangeEventHandler.
 */
//--------------------------------------------------------------------------------------------------
void gpioExpander_RemoveChangeEventHandler
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_HandlerRecord_t *handlerRecord,
    gpioExpander_ChangeCallbackRef_t ref
)
{
    // Sanity check to make sure that the client held the reference and called with the correct
    // one.
    if ((gpioExpander_HandlerRecord_t *)ref != handlerRecord)
    {
        LE_KILL_CLIENT("Invalid handler reference");
    }
    handlerRecord->handlerPtr = NULL;
    handlerRecord->contextPtr = NULL;

    // TODO: As above, need a better way to signal failure to the user
    LE_FATAL_IF(
        EnableInterrupt(expander, pin, false) != LE_OK,
        "Failed to disable interrupt during event handler deregistration");
}

//--------------------------------------------------------------------------------------------------
/**
 * Sets the edge sensitivity of an input
 *
 * The edge sensitivity of an input is also set when the change event handler is registered, so
 * this function is only useful for changing the edge sensitivity of an input pin with a handler
 * already registered.
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t gpioExpander_SetEdgeSense
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_Edge_t trigger ///< Change(s) that should trigger the callback to be called.
)
{
    const uint8_t edgeSenseFieldWidth = 2;
    le_result_t r = Sx1509WritePinField(
        expander,
        pin,
        SX1509_REG_SENSE_LOW_A,
        edgeSenseFieldWidth,
        trigger);
    if (r != LE_OK)
    {
        LE_ERROR("Could not set edge sense");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Gets the current edge sensitivity setting of a GPIO
 *
 * @return
 *      - The edge sensitivity of the GPIO
 *
 * @note
 *      There is no check performed to ensure that a handler has been registered nor is there a
 *      check to verify that the specified GPIO is an input.
 */
//--------------------------------------------------------------------------------------------------
gpioExpander_Edge_t gpioExpander_GetEdgeSense
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    const uint8_t edgeSenseFieldWidth = 2;
    gpioExpander_Edge_t edge = 0;
    const le_result_t r = Sx1509ReadPinField(
        expander,
        pin,
        SX1509_REG_SENSE_LOW_A,
        edgeSenseFieldWidth,
        (uint8_t *)&edge);

    // TODO: There is no way to signal failure in the le_gpio.api
    LE_FATAL_IF(r != LE_OK, "Failed to get edge sense")

    return edge;
}

//--------------------------------------------------------------------------------------------------
/**
 * Disables edge sensing for the given GPIO
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t gpioExpander_DisableEdgeSense
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    return gpioExpander_SetEdgeSense(expander, pin, GPIO_EXPANDER_EDGE_NONE);
}

//--------------------------------------------------------------------------------------------------
/**
 * Checks if the given GPIO is an output
 *
 * @return
 *      true if the specified GPIO is an output or false otherwise
 */
//--------------------------------------------------------------------------------------------------
bool gpioExpander_IsOutput
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    const uint8_t directionFieldWidth = 1;
    Sx1509_Direction_t direction;
    const le_result_t r = Sx1509ReadPinField(
        expander,
        pin,
        SX1509_REG_DIR_A,
        directionFieldWidth,
        (uint8_t *)&direction);

    // TODO: There is no way to signal failure in the le_gpio.api
    LE_FATAL_IF (r != LE_OK, "Failed to check if GPIO direction")

    return direction == SX1509_DIRECTION_OUTPUT;
}

//--------------------------------------------------------------------------------------------------
/**
 * Checks if the given GPIO is an input
 *
 * @return
 *      true if the specified GPIO is an input or false otherwise
 */
//--------------------------------------------------------------------------------------------------
bool gpioExpander_IsInput
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    return !gpioExpander_IsOutput(expander, pin);
}

//--------------------------------------------------------------------------------------------------
/**
 * Gets the polarity setting of the given GPIO
 *
 * @return
 *      The polarity of the specified GPIO
 */
//--------------------------------------------------------------------------------------------------
gpioExpander_Polarity_t gpioExpander_GetPolarity
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    const uint8_t polarityFieldWidth = 1;
    Sx1509_Polarity_t polarity = 0;
    const le_result_t r = Sx1509ReadPinField(
        expander,
        pin,
        SX1509_REG_POLARITY_A,
        polarityFieldWidth,
        (uint8_t *)&polarity);

    // TODO: There is no way to signal failure in the le_gpio.api
    LE_FATAL_IF (r != LE_OK, "Failed to get GPIO polarity")

    return (polarity == SX1509_POLARITY_NORMAL) ?
        GPIO_EXPANDER_ACTIVE_HIGH :
        GPIO_EXPANDER_ACTIVE_LOW;
}

//--------------------------------------------------------------------------------------------------
/**
 * Checks if the given GPIO is active or inactive
 *
 * @return
 *      true if the given GPIO is active or false otherwise
 *
 * @note
 *      No check is performed to ensure that the GPIO is an output
 */
//--------------------------------------------------------------------------------------------------
bool gpioExpander_IsActive
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    return gpioExpander_Read(expander, pin);
}

//--------------------------------------------------------------------------------------------------
/**
 * Gets the resistor settings of the given GPIO
 *
 * @return
 *      The resistor settings of the given GPIO
 */
//--------------------------------------------------------------------------------------------------
gpioExpander_PullUpDown_t gpioExpander_GetPullUpDown
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin
)
{
    const uint8_t pullFieldWidth = 1;
    uint8_t pullUpEnabled;
    uint8_t pullDownEnabled;
    const le_result_t pullUpResult = Sx1509ReadPinField(
        expander,
        pin,
        SX1509_REG_PULL_UP_A,
        pullFieldWidth,
        &pullUpEnabled);
    const le_result_t pullDownResult = Sx1509ReadPinField(
        expander,
        pin,
        SX1509_REG_PULL_DOWN_A,
        pullFieldWidth,
        &pullDownEnabled);

    // TODO: There is no way to signal failure using le_gpio.api
    LE_FATAL_IF(
        (pullUpResult != LE_OK || pullDownResult != LE_OK),
        "Failed while checking pullup/pulldown settings");

    if (pullUpEnabled == 0 && pullDownEnabled == 0)
    {
        return GPIO_EXPANDER_PULL_OFF;
    }
    else if (pullUpEnabled == 1 && pullDownEnabled == 0)
    {
        return GPIO_EXPANDER_PULL_UP;
    }
    else if (pullUpEnabled == 0 && pullDownEnabled == 1)
    {
        return GPIO_EXPANDER_PULL_DOWN;
    }
    else
    {
        // TODO: There is no way to signal failure using le_gpio.api
        LE_FATAL("Pullup and pulldown are enabled simultaneously");
    }
}

//--------------------------------------------------------------------------------------------------
/**
 * Performs a software reset of the specified SX1509 GPIO expander
 *
 * Upon completion, the SX1509 will be in a state equivalent to if the POR pin had been asserted.
 */
//--------------------------------------------------------------------------------------------------
void gpioExpander_Reset
(
    const gpioExpander_Identifier_t *expander  ///< I2C identifier for the GPIO expander
)
{
    const uint8_t magicResetVals[] = { 0x12, 0x34 };
    for (int i = 0; i < NUM_ARRAY_MEMBERS(magicResetVals); i++)
    {
        LE_FATAL_IF(
            SmbusWriteReg(
                expander->i2cBus, expander->i2cAddr, SX1509_REG_RESET, magicResetVals[i]) != LE_OK,
            "Failed to reset GPIO expander on I2C bus %d at address 0x%x",
            expander->i2cBus,
            expander->i2cAddr);
    }
}

//--------------------------------------------------------------------------------------------------
/**
 * An event handler which is parameterized to handle the interrupts of all GPIO expanders
 *
 * The handler will call the handler for each GPIO which has triggered due to an edge condition.
 */
//--------------------------------------------------------------------------------------------------
void gpioExpander_GenericInterruptHandler
(
    const gpioExpander_Identifier_t *expander,
    const gpioExpander_HandlerRecord_t *handlers
)
{
    // Determine which GPIOs of the expander have generated interrupts
    uint8_t statusA;
    uint8_t statusB;
    le_result_t result = SmbusReadReg(
        expander->i2cBus, expander->i2cAddr, SX1509_REG_EVENT_STATUS_B, &statusB);
    if (result != LE_OK)
    {
        // TODO: What should we do if the read fails?
    }
    result = SmbusReadReg(
        expander->i2cBus, expander->i2cAddr, SX1509_REG_EVENT_STATUS_A, &statusA);
    if (result != LE_OK)
    {
        // TODO: What should we do if the read fails?
    }
    const uint16_t status = ((statusB << 8) | statusA);

    // Clear the interrupt status for all GPIOs on the expander
    result = SmbusWriteReg(
        expander->i2cBus,
        expander->i2cAddr,
        SX1509_REG_EVENT_STATUS_B,
        statusB);
    if (result != LE_OK)
    {
        // TODO: What should we do if the write fails?
    }
    result = SmbusWriteReg(
        expander->i2cBus,
        expander->i2cAddr,
        SX1509_REG_EVENT_STATUS_A,
        statusA);
    if (result != LE_OK)
    {
        // TODO: What should we do if the write fails?
    }

    // Read the current input value of the GPIOs
    uint8_t dataB = 0;
    uint8_t dataA = 0;
    if (statusB != 0)
    {
        result = SmbusReadReg(
            expander->i2cBus, expander->i2cAddr, SX1509_REG_DATA_B, &dataB);
        if (result != LE_OK)
        {
            // TODO: What should we do if the read fails?
        }
    }
    if (statusA != 0)
    {
        result = SmbusReadReg(
            expander->i2cBus, expander->i2cAddr, SX1509_REG_DATA_A, &dataA);
        if (result != LE_OK)
        {
            // TODO: What should we do if the read fails?
        }
    }
    const uint16_t data = ((dataB << 8) | dataA);

    // Call the registered handlers for each of the interrupts
    for (int i = 0; i <= 15; i++)
    {
        if (status & (1 << i))
        {
            const gpioExpander_HandlerRecord_t *handler = &handlers[i];
            LE_FATAL_IF(
                handler->handlerPtr == NULL, "Interrupt has fired, but no handler is registered");
            const bool gpioActive = ((data >> i) & 1) == 1;
            (*(handler->handlerPtr))(gpioActive, handler->contextPtr);
        }
    }
}

le_result_t gpioExpander_DiscoverPrimaryI2cBusNum
(
    uint8_t *busNum  ///< [OUT] Primary I2C bus number
)
{
    const char *prefix = "/dev/i2c-";
    const uint8_t numsToTry[] = {0, 4};
    char path[16];

    for (int i = 0; i < NUM_ARRAY_MEMBERS(numsToTry); i++)
    {
        struct stat statBuf = {};
        const int formattedStringLen = snprintf(path, sizeof(path), "%s%d", prefix, numsToTry[i]);
        LE_ASSERT(formattedStringLen < NUM_ARRAY_MEMBERS(path));
        if (stat(path, &statBuf) == 0)
        {
            if (S_ISCHR(statBuf.st_mode))
            {
                *busNum = numsToTry[i];
                return LE_OK;
            }
        }
    }

    return LE_NOT_FOUND;
}


//-------------------------------------------------------------------------------------------------
// Static functions
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/**
 * Get the file handle for the given I2C bus and configure it for access to the given I2C address.
 *
 * @return
 *      - LE_FAULT on failure
 *      - A filehandle to the I2C bus
 */
//--------------------------------------------------------------------------------------------------
static int I2cAccessBusAddr
(
    uint8_t i2cBus,
    uint8_t i2cAddr
)
{
    const size_t filenameSize = 32;
    char filename[filenameSize];

    snprintf(filename, filenameSize, "/dev/i2c/%d", i2cBus);

    LE_DEBUG("Opening I2C bus: '%s'", filename);
    int fd = open(filename, O_RDWR);
    if (fd < 0 && (errno == ENOENT || errno == ENOTDIR))
    {
        snprintf(filename, filenameSize, "/dev/i2c-%d", i2cBus);
        LE_DEBUG("Opening I2C bus: '%s'", filename);
        fd = open(filename, O_RDWR);
    }

    if (fd < 0)
    {
        if (errno == ENOENT)
        {
            LE_ERROR(
                "Could not open file /dev/i2c-%d or /dev/i2c/%d: %s\n",
                i2cBus,
                i2cBus,
                strerror(ENOENT));
        }
        else
        {
            LE_ERROR("Could not open file %s': %s\n", filename, strerror(errno));
        }

        return LE_FAULT;
    }

    if (ioctl(fd, I2C_SLAVE_FORCE, i2cAddr) < 0)
    {
        LE_ERROR("Could not set address to 0x%02x: %s\n", i2cAddr, strerror(errno));
        return LE_FAULT;
    }

    return fd;
}

//--------------------------------------------------------------------------------------------------
/**
 * Performs an SMBUS read of a 1 byte register
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
static le_result_t SmbusReadReg
(
    uint8_t i2cBus,  ///< [IN] I2C bus to perform the read on
    uint8_t i2cAddr, ///< [IN] I2C address to read from
    uint8_t reg,     ///< [IN] Register within the I2C device to read
    uint8_t *data    ///< [OUT] Value stored within the register.  This value is only valid if the
                     ///  function returned LE_OK.
)
{
    int i2cFd = I2cAccessBusAddr(i2cBus, i2cAddr);
    if (i2cFd == LE_FAULT) {
        LE_ERROR("failed to open i2c bus %d for access to address %d\n", i2cBus, i2cAddr);
        return  LE_FAULT;
    }

    le_result_t result;
    const int readResult = i2c_smbus_read_byte_data(i2cFd, reg);
    if (readResult < 0)
    {
        LE_ERROR("smbus read failed with error %d", readResult);
        result = LE_FAULT;
    }
    else
    {
        *data = readResult;
        result = LE_OK;
    }
    close(i2cFd);

    LE_DEBUG("SMBUS READ addr=0x%x, reg=0x%x, data=0x%x", i2cAddr, reg, *data);

    return result;
}

//--------------------------------------------------------------------------------------------------
/**
 * Performs an SMBUS write of a 1 byte register
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
static le_result_t SmbusWriteReg
(
    uint8_t i2cBus,  ///< [IN] I2C bus to perform the write on
    uint8_t i2cAddr, ///< [IN] Address of the I2C device to write
    uint8_t reg,     ///< [IN] Register within the I2C device to write
    uint8_t data     ///< [IN] Data to write to the given register
)
{
    int i2cFd = I2cAccessBusAddr(i2cBus, i2cAddr);
    if (i2cFd == LE_FAULT) {
        LE_ERROR("failed to open i2c bus %d for access to address %d\n", i2cBus, i2cAddr);
        return LE_FAULT;
    }

    le_result_t result;
    const int writeResult = i2c_smbus_write_byte_data(i2cFd, reg, data);
    if (writeResult < 0)
    {
        LE_ERROR("smbus write failed with error %d", writeResult);
        result = LE_FAULT;
    }
    else
    {
        result = LE_OK;
    }
    close(i2cFd);

    LE_DEBUG("SMBUS WRITE addr=0x%x, reg=0x%x, data=0x%x", i2cAddr, reg, data);

    return result;
}

//--------------------------------------------------------------------------------------------------
/**
 * Performs a masked SMBUS write of a 1 byte register
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
static le_result_t SmbusReadModifyWrite
(
    uint8_t i2cBus,    ///< [IN] I2C bus to perform the masked write on
    uint8_t i2cAddr,   ///< [IN] Address of the I2C device to perform the masked write on
    uint8_t reg,       ///< [IN] Register within the I2C device to perform the masked write on
    uint8_t writeData, ///< [IN] Value to write into the register
    uint8_t writeMask  ///< [IN] Mask to apply to write.  Only bits which are set in the mask will
                       ///  be written from the writeData parameter into the given register.
)
{
    uint8_t data;
    le_result_t r = SmbusReadReg(i2cBus, i2cAddr, reg, &data);
    if (r != LE_OK)
    {
        LE_ERROR("Failed during read portion of SMBUS read-modify-write");
        return LE_FAULT;
    }

    data = (data & ~writeMask) | (writeData & writeMask);

    r = SmbusWriteReg(i2cBus, i2cAddr, reg, data);
    if (r != LE_OK)
    {
        LE_ERROR("Failed during write portion of SMBUS read-modify-write");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Computes access offsets for a register which has pin fields inside it
 *
 * In the SX1509 GPIO expander, there are 16 pins and fields are either 1 or 2 bits wide.  For 1
 * bit wide fields, 2 8-bit registers are required to accomodate all 16 GPIOs.  For 2 bit wide
 * fields, 4 8-bit registers are required.  The registers are arranged consistently such that
 * Register N contains the field for GPIO 15 in it's most significant bit (or 2 bits if it is a 2
 * bit wide field).  This is followed by the field for GPIO 14 and so on.  This function gets the
 * register address and bit offset of a target field based upon the register address which contains
 * the field for pin 0 and the width of each field.
 */
//--------------------------------------------------------------------------------------------------
static void Sx1509ComputePinFieldAccessParameters(
    uint8_t baseReg,      ///< [IN] Register which contains the field for GPIO 0
    uint8_t pinNum,       ///< [IN] Pin number of the field that the client wishes to access
    uint8_t fieldWidth,   ///< [IN] Field width.  Should be 1 or 2.
    uint8_t *reg,         ///< [OUT] Register which contains the field of interest
    uint8_t *offsetInReg  ///< [OUT] Bit offset of the field within the reg output parameter
)
{
    // TODO: should we check that pinNum is in range 0..15 or that fieldWidth is 1 or 2?
    const uint8_t totalOffset = pinNum * fieldWidth;
    const uint8_t regOffset = totalOffset / 8;
    *reg = baseReg - regOffset;
    *offsetInReg = totalOffset % 8;
}

//--------------------------------------------------------------------------------------------------
/**
 * Create a mask of a fixed width
 *
 * @return
 *      A mask with the least width significant bits set
 *
 * @note
 *      It is invalid to specify a width > 8, but the width is not validated by this function
 */
//--------------------------------------------------------------------------------------------------
static uint8_t CreateMask(
    uint8_t width  ///< [IN] Width int bits of the mask to create
)
{
    return (1 << width) - 1;
}

//--------------------------------------------------------------------------------------------------
/**
 * Extract a bit-field from a uint8_t
 *
 * @return
 *      The bit-field value
 *
 * @note
 *      No validation of the width or offset is performed
 */
//--------------------------------------------------------------------------------------------------
static uint8_t ExtractField(
    uint8_t data,   ///< [IN] value to extract the bit-field from
    uint8_t offset, ///< [IN] bit offset of the field within the data
    uint8_t width   ///< [IN] width of the bit-field
)
{
    return (data >> offset) & CreateMask(width);
}

//--------------------------------------------------------------------------------------------------
/**
 * Reads the field associated with a single pin from a register
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
static le_result_t Sx1509ReadPinField(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    uint8_t baseReg,                       ///< [IN] Register containing the field for pin 0
    uint8_t fieldWidth,                    ///< [IN] Width of the field in bits
    uint8_t *fieldData                     ///< [OUT] The extracted field data shifted into the
                                           ///  least significant bit(s).
)
{
    uint8_t reg;
    uint8_t fieldOffset;
    Sx1509ComputePinFieldAccessParameters(baseReg, pin, fieldWidth, &reg, &fieldOffset);

    uint8_t data;
    le_result_t r = SmbusReadReg(expander->i2cBus, expander->i2cAddr, reg, &data);
    if (r != LE_OK)
    {
        LE_ERROR("Failed to read pin field");
        return LE_FAULT;
    }

    *fieldData = ExtractField(data, fieldOffset, fieldWidth);
    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Writes the field associated with a signle pin from a register
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
static le_result_t Sx1509WritePinField(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    uint8_t baseReg,                       ///< [IN] Register containing the field for pin 0
    uint8_t fieldWidth,                    ///< [IN] Width of the field in bits
    uint8_t fieldData                      ///< [IN] Data to write into the field.  The data should
                                           ///  be in the least significant bit(s).
)
{
    uint8_t reg;
    uint8_t fieldOffset;
    Sx1509ComputePinFieldAccessParameters(baseReg, pin, fieldWidth, &reg, &fieldOffset);

    le_result_t r = SmbusReadModifyWrite(
        expander->i2cBus,
        expander->i2cAddr,
        reg,
        fieldData << fieldOffset,
        CreateMask(fieldWidth) << fieldOffset);
    if (r != LE_OK)
    {
        LE_ERROR("Failed to read pin field");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Enable (or disable) interrupt generation for the given GPIO
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
static le_result_t EnableInterrupt
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    bool enable
)
{
    const uint8_t interruptMaskFieldWidth = 1;
    le_result_t r = Sx1509WritePinField(
        expander,
        pin,
        SX1509_REG_INTERRUPT_MASK_A,
        interruptMaskFieldWidth,
        enable ? 0 : 1);
    if (r != LE_OK)
    {
        LE_ERROR("Could not change interrupt enable");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Set the output of a GPIO to a specified value
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
static le_result_t WriteData
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    bool active                            ///< [IN] If true set output to active otherwise set the
                                           ///  output to incactive
)
{
    const uint8_t dataFieldWidth = 1;
    le_result_t r = Sx1509WritePinField(
        expander,
        pin,
        SX1509_REG_DATA_A,
        dataFieldWidth,
        active ? 1 : 0);

    if (r != LE_OK)
    {
        LE_ERROR("Set output %d failure", active);
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Configure a GPIO as output and set the output type, polarity and initial value
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
static le_result_t SetOutput
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_OutputType_t outputType,  ///< [IN] Type of output to configure the GPIO as
    gpioExpander_Polarity_t polarity,      ///< [IN] Active-high or active-low.
    bool value                             ///< [IN] Initial value to drive (true=active,
                                           ///  false=inactive)
)
{
    if (SetOutputType(expander, pin, outputType) != LE_OK)
    {
        return LE_FAULT;
    }

    if (SetPolarity(expander, pin, polarity) != LE_OK)
    {
        return LE_FAULT;
    }

    if (WriteData(expander, pin, value) != LE_OK)
    {
        return LE_FAULT;
    }

    if (SetDirection(expander, pin, false) != LE_OK)
    {
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Sets the internal resistors for given GPIO
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 *
 * @note
 *      The API intentionally prevents a client from being able to enable the pull-up and pull-down
 *      resistors simultaneously.
 */
//--------------------------------------------------------------------------------------------------
static le_result_t SetPullType
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_PullUpDown_t pullType
)
{
    uint8_t pullUpVal = 0;
    uint8_t pullDownVal = 0;
    if (pullType == GPIO_EXPANDER_PULL_UP)
    {
        pullUpVal = 1;
    }
    else if (pullType == GPIO_EXPANDER_PULL_DOWN)
    {
        pullDownVal = 1;
    }

    const uint8_t pullFieldWidth = 1;
    const le_result_t pullUpResult = Sx1509WritePinField(
        expander,
        pin,
        SX1509_REG_PULL_UP_A,
        pullFieldWidth,
        pullUpVal);
    const le_result_t pullDownResult = Sx1509WritePinField(
        expander,
        pin,
        SX1509_REG_PULL_DOWN_A,
        pullFieldWidth,
        pullDownVal);

    if (pullUpResult != LE_OK || pullDownResult != LE_OK)
    {
        LE_ERROR("Failure configuring pullup/pulldown resistors.");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Set the polarity of the given GPIO
 *
 * @return
 *      - LE_OK
 *      - LE_ERROR
 */
//--------------------------------------------------------------------------------------------------
static le_result_t SetPolarity
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_Polarity_t polarity       ///< [IN] Polarity to set the GPIO to
)
{
    const uint8_t polarityFieldWidth = 1;
    const le_result_t r = Sx1509WritePinField(
        expander,
        pin,
        SX1509_REG_POLARITY_A,
        polarityFieldWidth,
        polarity == GPIO_EXPANDER_ACTIVE_HIGH ? SX1509_POLARITY_NORMAL : SX1509_POLARITY_INVERTED);

    if (r != LE_OK)
    {
        LE_ERROR("Failure configuring GPIO polarity.");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Sets the output type of the given GPIO
 *
 * @return
 *      - LE_NOT_IMPLEMENTED if the output type requested was tristate
 *      - LE_OK
 *      - LE_FAULT
 *
 * @note
 *      Tristate output is currently unsupported
 */
//--------------------------------------------------------------------------------------------------
static le_result_t SetOutputType
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    gpioExpander_OutputType_t outputType   ///< [IN] Output type to configure the GPIO as
)
{
    const uint8_t openDrainFieldWidth = 1;

    if (outputType == GPIO_EXPANDER_OUTPUT_TYPE_TRISTATE)
    {
        LE_WARN("Tri-State API not implemented in sysfs GPIO");
        return LE_NOT_IMPLEMENTED;
    }
    else if (
            outputType != GPIO_EXPANDER_OUTPUT_TYPE_OPEN_DRAIN &&
            outputType != GPIO_EXPANDER_OUTPUT_TYPE_PUSH_PULL)
    {
        LE_ERROR("Unsupported output type");
        return LE_FAULT;
    }

    const le_result_t r = Sx1509WritePinField(
        expander,
        pin,
        SX1509_REG_OPEN_DRAIN_A,
        openDrainFieldWidth,
        outputType == GPIO_EXPANDER_OUTPUT_TYPE_OPEN_DRAIN ? 1 : 0);

    if (r != LE_OK)
    {
        LE_ERROR("Failure configuring output type");
        return LE_FAULT;
    }

    return LE_OK;
}

//--------------------------------------------------------------------------------------------------
/**
 * Sets the direction of the given GPIO
 *
 * @return
 *      - LE_OK
 *      - LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
static le_result_t SetDirection
(
    const gpioExpander_Identifier_t *expander,
    uint8_t pin,
    bool isInput                           ///< [IN] true if the GPIO is to be configured as an
                                           ///  input or false if the GPIO is to be configured as
                                           ///  an output
)
{
    const uint8_t directionFieldWidth = 1;
    const le_result_t r = Sx1509WritePinField(
        expander,
        pin,
        SX1509_REG_DIR_A,
        directionFieldWidth,
        isInput ? SX1509_DIRECTION_INPUT : SX1509_DIRECTION_OUTPUT);

    if (r != LE_OK)
    {
        LE_ERROR("Failure configuring GPIO direction.");
        return LE_FAULT;
    }

    return LE_OK;
}


COMPONENT_INIT
{
}
