/*
 * AUTO-GENERATED _componentMain.c for the pmtool component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _pmtool_le_ulpm_ServiceInstanceName;
const char** le_ulpm_ServiceInstanceNamePtr = &_pmtool_le_ulpm_ServiceInstanceName;
void le_ulpm_ConnectService(void);
extern const char* _pmtool_le_bootReason_ServiceInstanceName;
const char** le_bootReason_ServiceInstanceNamePtr = &_pmtool_le_bootReason_ServiceInstanceName;
void le_bootReason_ConnectService(void);
extern const char* _pmtool_le_pm_ServiceInstanceName;
const char** le_pm_ServiceInstanceNamePtr = &_pmtool_le_pm_ServiceInstanceName;
void le_pm_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t pmtool_LogSession;
le_log_Level_t* pmtool_LogLevelFilterPtr;

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
__attribute__((constructor)) void _pmtool_Init(void)
{
    LE_DEBUG("Initializing pmtool component library.");

    // Connect client-side IPC interfaces.
    le_ulpm_ConnectService();
    le_bootReason_ConnectService();
    le_pm_ConnectService();

    // Register the component with the Log Daemon.
    pmtool_LogSession = le_log_RegComponent("pmtool", &pmtool_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
