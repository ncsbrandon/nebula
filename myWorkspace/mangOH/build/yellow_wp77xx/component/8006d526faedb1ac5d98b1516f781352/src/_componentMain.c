/*
 * AUTO-GENERATED _componentMain.c for the gnss component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _gnss_le_gnss_ServiceInstanceName;
const char** le_gnss_ServiceInstanceNamePtr = &_gnss_le_gnss_ServiceInstanceName;
void le_gnss_ConnectService(void);
extern const char* _gnss_le_pos_ServiceInstanceName;
const char** le_pos_ServiceInstanceNamePtr = &_gnss_le_pos_ServiceInstanceName;
void le_pos_ConnectService(void);
extern const char* _gnss_le_posCtrl_ServiceInstanceName;
const char** le_posCtrl_ServiceInstanceNamePtr = &_gnss_le_posCtrl_ServiceInstanceName;
void le_posCtrl_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t gnss_LogSession;
le_log_Level_t* gnss_LogLevelFilterPtr;

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
__attribute__((constructor)) void _gnss_Init(void)
{
    LE_DEBUG("Initializing gnss component library.");

    // Connect client-side IPC interfaces.
    le_gnss_ConnectService();
    le_pos_ConnectService();
    le_posCtrl_ConnectService();

    // Register the component with the Log Daemon.
    gnss_LogSession = le_log_RegComponent("gnss", &gnss_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
