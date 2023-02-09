/*
 * AUTO-GENERATED _componentMain.c for the dcsNet component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _dcsNet_le_appInfo_ServiceInstanceName;
const char** le_appInfo_ServiceInstanceNamePtr = &_dcsNet_le_appInfo_ServiceInstanceName;
void le_appInfo_ConnectService(void);
extern const char* _dcsNet_le_net_ServiceInstanceName;
const char** le_net_ServiceInstanceNamePtr = &_dcsNet_le_net_ServiceInstanceName;
void le_net_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t dcsNet_LogSession;
le_log_Level_t* dcsNet_LogLevelFilterPtr;

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
__attribute__((constructor)) void _dcsNet_Init(void)
{
    LE_DEBUG("Initializing dcsNet component library.");

    // Advertise server-side IPC interfaces.
    le_net_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_appInfo_ConnectService();

    // Register the component with the Log Daemon.
    dcsNet_LogSession = le_log_RegComponent("dcsNet", &dcsNet_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
