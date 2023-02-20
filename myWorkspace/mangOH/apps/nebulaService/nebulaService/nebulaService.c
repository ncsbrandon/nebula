#include "legato.h"
#include "interfaces.h"

#include "/home/firstuser/nebula/myWorkspace/mangOH/build/yellow_wp77xx/api/83d1ed450fec70d0341448e59d96f374/client/dhubIO_interface.h"

static const char Led0ResPath[] = "led/0";

COMPONENT_INIT
{
    LE_INFO("Hello NEBULA 20Jan!");

    LE_ASSERT_OK(dhubIO_CreateInput(Led0ResPath, DHUBIO_DATA_TYPE_BOOLEAN, ""));
}