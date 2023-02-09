/*
 * AUTO-GENERATED _componentMain.c for the secStoreDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _secStoreDaemon_le_appInfo_ServiceInstanceName;
const char** le_appInfo_ServiceInstanceNamePtr = &_secStoreDaemon_le_appInfo_ServiceInstanceName;
void le_appInfo_ConnectService(void);
extern const char* _secStoreDaemon_le_update_ServiceInstanceName;
const char** le_update_ServiceInstanceNamePtr = &_secStoreDaemon_le_update_ServiceInstanceName;
void le_update_ConnectService(void);
extern const char* _secStoreDaemon_le_instStat_ServiceInstanceName;
const char** le_instStat_ServiceInstanceNamePtr = &_secStoreDaemon_le_instStat_ServiceInstanceName;
void le_instStat_ConnectService(void);
extern const char* _secStoreDaemon_le_secStore_ServiceInstanceName;
const char** le_secStore_ServiceInstanceNamePtr = &_secStoreDaemon_le_secStore_ServiceInstanceName;
void le_secStore_AdvertiseService(void);
extern const char* _secStoreDaemon_secStoreGlobal_ServiceInstanceName;
const char** secStoreGlobal_ServiceInstanceNamePtr = &_secStoreDaemon_secStoreGlobal_ServiceInstanceName;
void secStoreGlobal_AdvertiseService(void);
extern const char* _secStoreDaemon_secStoreAdmin_ServiceInstanceName;
const char** secStoreAdmin_ServiceInstanceNamePtr = &_secStoreDaemon_secStoreAdmin_ServiceInstanceName;
void secStoreAdmin_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t secStoreDaemon_LogSession;
le_log_Level_t* secStoreDaemon_LogLevelFilterPtr;

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
__attribute__((constructor)) void _secStoreDaemon_Init(void)
{
    LE_DEBUG("Initializing secStoreDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_secStore_AdvertiseService();
    secStoreGlobal_AdvertiseService();
    secStoreAdmin_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_appInfo_ConnectService();
    le_update_ConnectService();
    le_instStat_ConnectService();

    // Register the component with the Log Daemon.
    secStoreDaemon_LogSession = le_log_RegComponent("secStoreDaemon", &secStoreDaemon_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
