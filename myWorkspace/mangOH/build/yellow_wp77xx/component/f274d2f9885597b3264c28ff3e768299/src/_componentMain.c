/*
 * AUTO-GENERATED _componentMain.c for the dcsDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _dcsDaemon_le_mrc_ServiceInstanceName;
const char** le_mrc_ServiceInstanceNamePtr = &_dcsDaemon_le_mrc_ServiceInstanceName;
void le_mrc_ConnectService(void);
extern const char* _dcsDaemon_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_dcsDaemon_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _dcsDaemon_le_wifiClient_ServiceInstanceName;
const char** le_wifiClient_ServiceInstanceNamePtr = &_dcsDaemon_le_wifiClient_ServiceInstanceName;
void le_wifiClient_ConnectService(void);
extern const char* _dcsDaemon_le_data_ServiceInstanceName;
const char** le_data_ServiceInstanceNamePtr = &_dcsDaemon_le_data_ServiceInstanceName;
void le_data_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t dcsDaemon_LogSession;
le_log_Level_t* dcsDaemon_LogLevelFilterPtr;

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
__attribute__((constructor)) void _dcsDaemon_Init(void)
{
    LE_DEBUG("Initializing dcsDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_data_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_mrc_ConnectService();
    le_cfg_ConnectService();
    // 'le_wifiClient' is [manual-start].

    // Register the component with the Log Daemon.
    dcsDaemon_LogSession = le_log_RegComponent("dcsDaemon", &dcsDaemon_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
