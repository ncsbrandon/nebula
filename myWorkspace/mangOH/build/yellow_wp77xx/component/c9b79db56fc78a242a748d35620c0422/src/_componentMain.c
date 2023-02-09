/*
 * AUTO-GENERATED _componentMain.c for the rSimDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _rSimDaemon_le_rsim_ServiceInstanceName;
const char** le_rsim_ServiceInstanceNamePtr = &_rSimDaemon_le_rsim_ServiceInstanceName;
void le_rsim_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t rSimDaemon_LogSession;
le_log_Level_t* rSimDaemon_LogLevelFilterPtr;

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
__attribute__((constructor)) void _rSimDaemon_Init(void)
{
    LE_DEBUG("Initializing rSimDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_rsim_AdvertiseService();

    // Register the component with the Log Daemon.
    rSimDaemon_LogSession = le_log_RegComponent("rSimDaemon", &rSimDaemon_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
