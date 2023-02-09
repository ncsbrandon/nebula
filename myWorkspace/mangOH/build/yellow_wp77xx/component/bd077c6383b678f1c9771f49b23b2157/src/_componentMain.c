/*
 * AUTO-GENERATED _componentMain.c for the posDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _posDaemon_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_posDaemon_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _posDaemon_le_gnss_ServiceInstanceName;
const char** le_gnss_ServiceInstanceNamePtr = &_posDaemon_le_gnss_ServiceInstanceName;
void le_gnss_AdvertiseService(void);
extern const char* _posDaemon_le_pos_ServiceInstanceName;
const char** le_pos_ServiceInstanceNamePtr = &_posDaemon_le_pos_ServiceInstanceName;
void le_pos_AdvertiseService(void);
extern const char* _posDaemon_le_posCtrl_ServiceInstanceName;
const char** le_posCtrl_ServiceInstanceNamePtr = &_posDaemon_le_posCtrl_ServiceInstanceName;
void le_posCtrl_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t posDaemon_LogSession;
le_log_Level_t* posDaemon_LogLevelFilterPtr;

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
__attribute__((constructor)) void _posDaemon_Init(void)
{
    LE_DEBUG("Initializing posDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_gnss_AdvertiseService();
    le_pos_AdvertiseService();
    le_posCtrl_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_cfg_ConnectService();

    // Register the component with the Log Daemon.
    posDaemon_LogSession = le_log_RegComponent("posDaemon", &posDaemon_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
