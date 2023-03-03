#include "legato.h"
#include "nebula-i2c.h"
#include "i2c-utils.h"

int I2cAccessBusAddr
(
    uint8_t i2cBus,   ////< I2C bus to perform the write on
    uint8_t i2cAddr  ////< I2C device address
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
                "Could not open file /dev/i2c-%d or /dev/i2c/%d: %s",
                i2cBus,
                i2cBus,
                strerror(ENOENT));
        }
        else
        {
            LE_ERROR("Could not open file %s': %s", filename, strerror(errno));
        }

        return LE_FAULT;
    }

    if (ioctl(fd, I2C_SLAVE_FORCE, i2cAddr) < 0)
    {
        LE_ERROR("Could not set address to 0x%02x: %s", i2cAddr, strerror(errno));
        close(fd);
        return LE_FAULT;
    }

    return fd;
}

/*
void I2cBusEnable
(
    uint8_t i2cBus,   ////< I2C bus to perform the write on
    uint8_t i2cAddr  ////< I2C device address
)
{
    LE_DEBUG("Enabling TCA9546A I2C switch...");
    const int i2cdev_fd = I2cAccessBusAddr(i2cBus, i2cAddr);
    LE_FATAL_IF(i2cdev_fd == LE_FAULT, "failed to open i2cbus %d", i2cBus);

    const uint8_t enableAllPorts = 0xff;
    LE_FATAL_IF(i2c_smbus_write_byte(i2cdev_fd, enableAllPorts) == -1, "failed to write i2c data");

    close(i2cdev_fd);
}
*/

le_result_t I2cWriteByte
(
    uint8_t i2cBus,   ////< I2C bus to perform the write on
    uint8_t i2cAddr,  ////< I2C device address
    uint8_t data      ////< Data to write to the given register
)
{
    int i2cFd = I2cAccessBusAddr(i2cBus, i2cAddr);
    if (i2cFd < 0)
    {
        LE_ERROR("failed [%d] to open i2c bus %d for access to address %d", i2cFd, i2cBus, i2cAddr);
        return LE_FAULT;
    }

    le_result_t result;

    const int writeResult = i2c_smbus_write_byte(i2cFd, data);
    if (writeResult < 0)
    {
        LE_ERROR("i2c write failed with error %d", writeResult);
        result = LE_FAULT;
    }
    else
    {
        LE_DEBUG("i2c write addr 0x%x, data=0x%x", i2cAddr, data);
        result = LE_OK;
    }

    close(i2cFd);

    return result;
}

le_result_t I2cWriteStr
(
    uint8_t i2cBus,   ////< I2C bus to perform the write on
    uint8_t i2cAddr,  ////< I2C device address
    char* data,      ////< Data to write to the given register
    uint8_t len
)
{
    int i2cFd = I2cAccessBusAddr(i2cBus, i2cAddr);
    if (i2cFd < 0)
    {
        LE_ERROR("failed [%d] to open i2c bus %d for access to address %d", i2cFd, i2cBus, i2cAddr);
        return LE_FAULT;
    }

    le_result_t result = LE_OK;

    for(int i=0; i<len; i++)
    {
        const int writeResult = i2c_smbus_write_byte(i2cFd, data[i]);
        if (writeResult < 0)
        {
            LE_ERROR("i2c write failed with error %d", writeResult);
            result = LE_FAULT;
            break;
        }

        LE_DEBUG("i2c write addr 0x%x, data=0x%x", i2cAddr, data[i]);
    }   

    close(i2cFd);

    return result;
}

le_result_t I2cReadStr(
    uint8_t i2cBus,  ////< I2C bus to perform the write on
    uint8_t i2cAddr, ////< I2C device address
    char *data,      ////< Data to write to the given register
    uint8_t len      ////< string length
)
{
     int i2cFd = I2cAccessBusAddr(i2cBus, i2cAddr);
    if (i2cFd < 0)
    {
        LE_ERROR("failed [%d] to open i2c bus %d for access to address %d", i2cFd, i2cBus, i2cAddr);
        return LE_FAULT;
    }

    le_result_t result = LE_OK;

    for(int i=0; i<len; i++)
    {
        const int readResult = i2c_smbus_read_byte(i2cFd);
        if (readResult < 0)
        {
            LE_ERROR("i2c read failed with error %d", readResult);
            result = LE_FAULT;
            break;
        }
        
        data[i] = readResult;
        LE_DEBUG("i2c read addr 0x%x, data=0x%x", i2cAddr, readResult);
    }

    close(i2cFd);

    return result;
}