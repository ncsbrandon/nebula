#ifndef NEBULA_I2C_H
#define NEBULA_I2C_H

#include "legato.h"

LE_SHARED int I2cAccessBusAddr(
    uint8_t i2cBus,  ////< I2C bus to perform the write on
    uint8_t i2cAddr  ////< I2C device address
);

/*
LE_SHARED void I2cBusEnable
(
    uint8_t i2cBus,  ////< I2C bus to perform the write on
    uint8_t i2cAddr  ////< I2C device address
);
*/

LE_SHARED le_result_t I2cWriteByte(
    uint8_t i2cBus,  ////< I2C bus to perform the write on
    uint8_t i2cAddr, ////< I2C device address
    uint8_t data     ////< Data to write to the given register
);

LE_SHARED le_result_t I2cWriteStr(
    uint8_t i2cBus,  ////< I2C bus to perform the write on
    uint8_t i2cAddr, ////< I2C device address
    char *data,      ////< Data to write to the given register
    uint8_t len      ////< string length
);

LE_SHARED le_result_t I2cReadStr(
    uint8_t i2cBus,  ////< I2C bus to perform the write on
    uint8_t i2cAddr, ////< I2C device address
    char *data,      ////< Data to write to the given register
    uint8_t len      ////< string length
);

#endif // NEBULA_I2C_H