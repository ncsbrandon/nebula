/*
 * AUTO-GENERATED _componentMain.c for the bluetoothServicesComponent component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _bluetoothServicesComponent_dhubAdmin_ServiceInstanceName;
const char** dhubAdmin_ServiceInstanceNamePtr = &_bluetoothServicesComponent_dhubAdmin_ServiceInstanceName;
void dhubAdmin_ConnectService(void);
extern const char* _bluetoothServicesComponent_le_info_ServiceInstanceName;
const char** le_info_ServiceInstanceNamePtr = &_bluetoothServicesComponent_le_info_ServiceInstanceName;
void le_info_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t bluetoothServicesComponent_LogSession;
le_log_Level_t* bluetoothServicesComponent_LogLevelFilterPtr;

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
__attribute__((constructor)) void _bluetoothServicesComponent_Init(void)
{
    LE_DEBUG("Initializing bluetoothServicesComponent component library.");

    // Connect client-side IPC interfaces.
    dhubAdmin_ConnectService();
    le_info_ConnectService();

    // Register the component with the Log Daemon.
    bluetoothServicesComponent_LogSession = le_log_RegComponent("bluetoothServicesComponent", &bluetoothServicesComponent_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
