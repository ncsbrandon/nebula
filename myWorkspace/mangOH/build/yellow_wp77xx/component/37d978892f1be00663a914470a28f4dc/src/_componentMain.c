/*
 * AUTO-GENERATED _componentMain.c for the dataHub component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _dataHub_le_appInfo_ServiceInstanceName;
const char** le_appInfo_ServiceInstanceNamePtr = &_dataHub_le_appInfo_ServiceInstanceName;
void le_appInfo_ConnectService(void);
extern const char* _dataHub_io_ServiceInstanceName;
const char** io_ServiceInstanceNamePtr = &_dataHub_io_ServiceInstanceName;
void io_AdvertiseService(void);
extern const char* _dataHub_admin_ServiceInstanceName;
const char** admin_ServiceInstanceNamePtr = &_dataHub_admin_ServiceInstanceName;
void admin_AdvertiseService(void);
extern const char* _dataHub_query_ServiceInstanceName;
const char** query_ServiceInstanceNamePtr = &_dataHub_query_ServiceInstanceName;
void query_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t dataHub_LogSession;
le_log_Level_t* dataHub_LogLevelFilterPtr;

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
__attribute__((constructor)) void _dataHub_Init(void)
{
    LE_DEBUG("Initializing dataHub component library.");

    // Advertise server-side IPC interfaces.
    io_AdvertiseService();
    admin_AdvertiseService();
    query_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_appInfo_ConnectService();

    // Register the component with the Log Daemon.
    dataHub_LogSession = le_log_RegComponent("dataHub", &dataHub_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
