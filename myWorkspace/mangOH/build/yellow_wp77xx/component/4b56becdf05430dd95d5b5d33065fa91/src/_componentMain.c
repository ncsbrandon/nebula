/*
 * AUTO-GENERATED _componentMain.c for the moduleLoad component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _moduleLoad_le_kernelModule_ServiceInstanceName;
const char** le_kernelModule_ServiceInstanceNamePtr = &_moduleLoad_le_kernelModule_ServiceInstanceName;
void le_kernelModule_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t moduleLoad_LogSession;
le_log_Level_t* moduleLoad_LogLevelFilterPtr;

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
__attribute__((constructor)) void _moduleLoad_Init(void)
{
    LE_DEBUG("Initializing moduleLoad component library.");

    // Connect client-side IPC interfaces.
    le_kernelModule_ConnectService();

    // Register the component with the Log Daemon.
    moduleLoad_LogSession = le_log_RegComponent("moduleLoad", &moduleLoad_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
