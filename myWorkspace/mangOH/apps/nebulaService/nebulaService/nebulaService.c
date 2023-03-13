#include "legato.h"
#include "interfaces.h"
#include "i2c-utils.h"
#include "nebula-i2c.h"
#include "nebula-uart.h"

//#include "/home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/api/83d1ed450fec70d0341448e59d96f374/client/dhubIO_interface.h"

#ifndef DHUBIO_DATA_TYPE_BOOLEAN
#define DHUBIO_DATA_TYPE_BOOLEAN 1
#endif

static const char NAME[] = "NAME,Nebula Legato";
static const char VERSION[] = "VERSION,1.0.13Mar2023";

static const char Led0ResPath[] = "led/0";

COMPONENT_INIT
{
    char buffer[48];
    sprintf(buffer, "Starting %s %s", NAME, VERSION);
    LE_INFO(buffer);

    //I2cBusEnable(0x00, 0x71);
    //I2cWriteByte(I2CBUS, 0x3E, 'o');
    //char world[] = " World!";
    //I2cWriteStr(I2CBUS, MCUADDR, (char*)&world, strlen(world));

    // request the version
    //I2cWriteByte(I2CBUS, MCUADDR, REQUEST_VERSION);

    //char version[25];
    //memset(version, 0, 25);
    //I2cReadStr(I2CBUS, MCUADDR, version, 24);

    LE_INFO("Open the port");
    int fd = UARTOpen("/dev/ttyHS0");
    if(fd < 0)
        exit(-1);

    LE_INFO("Writes");
    UARTWriteSentence(fd, (char*)NAME);
    UARTWriteSentence(fd, (char*)VERSION);

    LE_INFO("Reads");
    char input[48];
    UARTReadSentence(fd, input, 48);
    LE_INFO(input);
    UARTReadSentence(fd, input, 48);
    LE_INFO(input);
    
    LE_INFO("Close");
    UARTClose(fd);

    LE_ASSERT_OK(dhubIO_CreateInput(Led0ResPath, DHUBIO_DATA_TYPE_BOOLEAN, ""));
}