/*
 * AUTO-GENERATED _componentMain.c for the daemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _daemon_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_daemon_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _daemon_le_secStore_ServiceInstanceName;
const char** le_secStore_ServiceInstanceNamePtr = &_daemon_le_secStore_ServiceInstanceName;
void le_secStore_ConnectService(void);
extern const char* _daemon_le_wifiClient_ServiceInstanceName;
const char** le_wifiClient_ServiceInstanceNamePtr = &_daemon_le_wifiClient_ServiceInstanceName;
void le_wifiClient_AdvertiseService(void);
extern const char* _daemon_le_wifiAp_ServiceInstanceName;
const char** le_wifiAp_ServiceInstanceNamePtr = &_daemon_le_wifiAp_ServiceInstanceName;
void le_wifiAp_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t daemon_LogSession;
le_log_Level_t* daemon_LogLevelFilterPtr;

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
__attribute__((constructor)) void _daemon_Init(void)
{
    LE_DEBUG("Initializing daemon component library.");

    // Advertise server-side IPC interfaces.
    le_wifiClient_AdvertiseService();
    le_wifiAp_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_cfg_ConnectService();
    le_secStore_ConnectService();

    // Register the component with the Log Daemon.
    daemon_LogSession = le_log_RegComponent("daemon", &daemon_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
