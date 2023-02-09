/*
 * AUTO-GENERATED _componentMain.c for the appCfg component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _appCfg_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_appCfg_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t appCfg_LogSession;
le_log_Level_t* appCfg_LogLevelFilterPtr;

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
__attribute__((constructor)) void _appCfg_Init(void)
{
    LE_DEBUG("Initializing appCfg component library.");

    // Connect client-side IPC interfaces.
    le_cfg_ConnectService();

    // Register the component with the Log Daemon.
    appCfg_LogSession = le_log_RegComponent("appCfg", &appCfg_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
