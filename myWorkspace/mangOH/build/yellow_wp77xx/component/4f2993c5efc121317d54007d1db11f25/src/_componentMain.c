/*
 * AUTO-GENERATED _componentMain.c for the button component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _button_dhubIO_ServiceInstanceName;
const char** dhubIO_ServiceInstanceNamePtr = &_button_dhubIO_ServiceInstanceName;
void dhubIO_ConnectService(void);
extern const char* _button_gpio_ServiceInstanceName;
const char** gpio_ServiceInstanceNamePtr = &_button_gpio_ServiceInstanceName;
void gpio_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t button_LogSession;
le_log_Level_t* button_LogLevelFilterPtr;

// Declare component's COMPONENT_INIT_ONCE function,
// and provide default empty implementation.
__attribute__((weak))
COMPONENT_INIT_ONCE
{
}
// Component initialization function (COMPONENT_INIT).
COMPONENT_INIT;

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _button_Init(void)
{
    LE_DEBUG("Initializing button component library.");

    // Connect client-side IPC interfaces.
    dhubIO_ConnectService();
    gpio_ConnectService();

    // Register the component with the Log Daemon.
    button_LogSession = le_log_RegComponent("button", &button_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
