/*
 * AUTO-GENERATED _componentMain.c for the portDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _portDaemon_le_atServer_ServiceInstanceName;
const char** le_atServer_ServiceInstanceNamePtr = &_portDaemon_le_atServer_ServiceInstanceName;
void le_atServer_ConnectService(void);
extern const char* _portDaemon_le_port_ServiceInstanceName;
const char** le_port_ServiceInstanceNamePtr = &_portDaemon_le_port_ServiceInstanceName;
void le_port_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t portDaemon_LogSession;
le_log_Level_t* portDaemon_LogLevelFilterPtr;

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
__attribute__((constructor)) void _portDaemon_Init(void)
{
    LE_DEBUG("Initializing portDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_port_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_atServer_ConnectService();

    // Register the component with the Log Daemon.
    portDaemon_LogSession = le_log_RegComponent("portDaemon", &portDaemon_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
