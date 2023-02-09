/*
 * AUTO-GENERATED _componentMain.c for the fwupdateDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _fwupdateDaemon_le_pm_ServiceInstanceName;
const char** le_pm_ServiceInstanceNamePtr = &_fwupdateDaemon_le_pm_ServiceInstanceName;
void le_pm_ConnectService(void);
extern const char* _fwupdateDaemon_le_fwupdate_ServiceInstanceName;
const char** le_fwupdate_ServiceInstanceNamePtr = &_fwupdateDaemon_le_fwupdate_ServiceInstanceName;
void le_fwupdate_AdvertiseService(void);
extern const char* _fwupdateDaemon_le_dualsys_ServiceInstanceName;
const char** le_dualsys_ServiceInstanceNamePtr = &_fwupdateDaemon_le_dualsys_ServiceInstanceName;
void le_dualsys_AdvertiseService(void);
extern const char* _fwupdateDaemon_le_flash_ServiceInstanceName;
const char** le_flash_ServiceInstanceNamePtr = &_fwupdateDaemon_le_flash_ServiceInstanceName;
void le_flash_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t fwupdateDaemon_LogSession;
le_log_Level_t* fwupdateDaemon_LogLevelFilterPtr;

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
__attribute__((constructor)) void _fwupdateDaemon_Init(void)
{
    LE_DEBUG("Initializing fwupdateDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_fwupdate_AdvertiseService();
    le_dualsys_AdvertiseService();
    le_flash_AdvertiseService();

    // Connect client-side IPC interfaces.
    // 'le_pm' is [manual-start].

    // Register the component with the Log Daemon.
    fwupdateDaemon_LogSession = le_log_RegComponent("fwupdateDaemon", &fwupdateDaemon_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
