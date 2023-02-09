/*
 * AUTO-GENERATED _componentMain.c for the yellowCommandLine component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _yellowCommandLine_dhubAdmin_ServiceInstanceName;
const char** dhubAdmin_ServiceInstanceNamePtr = &_yellowCommandLine_dhubAdmin_ServiceInstanceName;
void dhubAdmin_ConnectService(void);
extern const char* _yellowCommandLine_le_info_ServiceInstanceName;
const char** le_info_ServiceInstanceNamePtr = &_yellowCommandLine_le_info_ServiceInstanceName;
void le_info_ConnectService(void);
extern const char* _yellowCommandLine_le_sim_ServiceInstanceName;
const char** le_sim_ServiceInstanceNamePtr = &_yellowCommandLine_le_sim_ServiceInstanceName;
void le_sim_ConnectService(void);
extern const char* _yellowCommandLine_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_yellowCommandLine_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t yellowCommandLine_LogSession;
le_log_Level_t* yellowCommandLine_LogLevelFilterPtr;

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
__attribute__((constructor)) void _yellowCommandLine_Init(void)
{
    LE_DEBUG("Initializing yellowCommandLine component library.");

    // Connect client-side IPC interfaces.
    dhubAdmin_ConnectService();
    le_info_ConnectService();
    le_sim_ConnectService();
    le_cfg_ConnectService();

    // Register the component with the Log Daemon.
    yellowCommandLine_LogSession = le_log_RegComponent("yellowCommandLine", &yellowCommandLine_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
