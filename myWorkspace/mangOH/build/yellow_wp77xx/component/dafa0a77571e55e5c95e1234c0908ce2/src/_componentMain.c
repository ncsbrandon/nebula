/*
 * AUTO-GENERATED _componentMain.c for the adminTool component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _adminTool_admin_ServiceInstanceName;
const char** admin_ServiceInstanceNamePtr = &_adminTool_admin_ServiceInstanceName;
void admin_ConnectService(void);
extern const char* _adminTool_query_ServiceInstanceName;
const char** query_ServiceInstanceNamePtr = &_adminTool_query_ServiceInstanceName;
void query_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t adminTool_LogSession;
le_log_Level_t* adminTool_LogLevelFilterPtr;

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
__attribute__((constructor)) void _adminTool_Init(void)
{
    LE_DEBUG("Initializing adminTool component library.");

    // Connect client-side IPC interfaces.
    // 'admin' is [manual-start].
    // 'query' is [manual-start].

    // Register the component with the Log Daemon.
    adminTool_LogSession = le_log_RegComponent("adminTool", &adminTool_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
