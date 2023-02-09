/*
 * AUTO-GENERATED _componentMain.c for the powerMgr component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _powerMgr_le_framework_ServiceInstanceName;
const char** le_framework_ServiceInstanceNamePtr = &_powerMgr_le_framework_ServiceInstanceName;
void le_framework_ConnectService(void);
extern const char* _powerMgr_le_pm_ServiceInstanceName;
const char** le_pm_ServiceInstanceNamePtr = &_powerMgr_le_pm_ServiceInstanceName;
void le_pm_AdvertiseService(void);
extern const char* _powerMgr_le_ulpm_ServiceInstanceName;
const char** le_ulpm_ServiceInstanceNamePtr = &_powerMgr_le_ulpm_ServiceInstanceName;
void le_ulpm_AdvertiseService(void);
extern const char* _powerMgr_le_bootReason_ServiceInstanceName;
const char** le_bootReason_ServiceInstanceNamePtr = &_powerMgr_le_bootReason_ServiceInstanceName;
void le_bootReason_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t powerMgr_LogSession;
le_log_Level_t* powerMgr_LogLevelFilterPtr;

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
__attribute__((constructor)) void _powerMgr_Init(void)
{
    LE_DEBUG("Initializing powerMgr component library.");

    // Advertise server-side IPC interfaces.
    le_pm_AdvertiseService();
    le_ulpm_AdvertiseService();
    le_bootReason_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_framework_ConnectService();

    // Register the component with the Log Daemon.
    powerMgr_LogSession = le_log_RegComponent("powerMgr", &powerMgr_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
