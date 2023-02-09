/*
 * AUTO-GENERATED _componentMain.c for the atThirdParty component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _atThirdParty_le_atServer_ServiceInstanceName;
const char** le_atServer_ServiceInstanceNamePtr = &_atThirdParty_le_atServer_ServiceInstanceName;
void le_atServer_ConnectService(void);
extern const char* _atThirdParty_le_tpf_ServiceInstanceName;
const char** le_tpf_ServiceInstanceNamePtr = &_atThirdParty_le_tpf_ServiceInstanceName;
void le_tpf_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t atThirdParty_LogSession;
le_log_Level_t* atThirdParty_LogLevelFilterPtr;

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
__attribute__((constructor)) void _atThirdParty_Init(void)
{
    LE_DEBUG("Initializing atThirdParty component library.");

    // Connect client-side IPC interfaces.
    le_atServer_ConnectService();
    le_tpf_ConnectService();

    // Register the component with the Log Daemon.
    atThirdParty_LogSession = le_log_RegComponent("atThirdParty", &atThirdParty_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
