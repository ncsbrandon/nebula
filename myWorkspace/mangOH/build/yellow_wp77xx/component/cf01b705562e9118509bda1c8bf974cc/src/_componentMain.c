/*
 * AUTO-GENERATED _componentMain.c for the cellNetService component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _cellNetService_le_sim_ServiceInstanceName;
const char** le_sim_ServiceInstanceNamePtr = &_cellNetService_le_sim_ServiceInstanceName;
void le_sim_ConnectService(void);
extern const char* _cellNetService_le_mrc_ServiceInstanceName;
const char** le_mrc_ServiceInstanceNamePtr = &_cellNetService_le_mrc_ServiceInstanceName;
void le_mrc_ConnectService(void);
extern const char* _cellNetService_le_secStore_ServiceInstanceName;
const char** le_secStore_ServiceInstanceNamePtr = &_cellNetService_le_secStore_ServiceInstanceName;
void le_secStore_ConnectService(void);
extern const char* _cellNetService_le_cellnet_ServiceInstanceName;
const char** le_cellnet_ServiceInstanceNamePtr = &_cellNetService_le_cellnet_ServiceInstanceName;
void le_cellnet_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t cellNetService_LogSession;
le_log_Level_t* cellNetService_LogLevelFilterPtr;

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
__attribute__((constructor)) void _cellNetService_Init(void)
{
    LE_DEBUG("Initializing cellNetService component library.");

    // Advertise server-side IPC interfaces.
    le_cellnet_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_sim_ConnectService();
    le_mrc_ConnectService();
    le_secStore_ConnectService();

    // Register the component with the Log Daemon.
    cellNetService_LogSession = le_log_RegComponent("cellNetService", &cellNetService_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
