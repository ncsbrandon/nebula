/*
 * AUTO-GENERATED _componentMain.c for the dcsWifi component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _dcsWifi_le_wifiClient_ServiceInstanceName;
const char** le_wifiClient_ServiceInstanceNamePtr = &_dcsWifi_le_wifiClient_ServiceInstanceName;
void le_wifiClient_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t dcsWifi_LogSession;
le_log_Level_t* dcsWifi_LogLevelFilterPtr;

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
__attribute__((constructor)) void _dcsWifi_Init(void)
{
    LE_DEBUG("Initializing dcsWifi component library.");

    // Connect client-side IPC interfaces.
    // 'le_wifiClient' is [manual-start].

    // Register the component with the Log Daemon.
    dcsWifi_LogSession = le_log_RegComponent("dcsWifi", &dcsWifi_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
