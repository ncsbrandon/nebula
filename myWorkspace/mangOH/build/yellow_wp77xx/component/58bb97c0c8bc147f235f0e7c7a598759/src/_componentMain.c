/*
 * AUTO-GENERATED _componentMain.c for the fwupdate component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _fwupdate_le_fwupdate_ServiceInstanceName;
const char** le_fwupdate_ServiceInstanceNamePtr = &_fwupdate_le_fwupdate_ServiceInstanceName;
void le_fwupdate_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t fwupdate_LogSession;
le_log_Level_t* fwupdate_LogLevelFilterPtr;

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
__attribute__((constructor)) void _fwupdate_Init(void)
{
    LE_DEBUG("Initializing fwupdate component library.");

    // Connect client-side IPC interfaces.
    // 'le_fwupdate' is [manual-start].

    // Register the component with the Log Daemon.
    fwupdate_LogSession = le_log_RegComponent("fwupdate", &fwupdate_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
