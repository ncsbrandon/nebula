/*
 * AUTO-GENERATED _componentMain.c for the le_pa_secStoreTz component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _le_pa_secStoreTz_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_le_pa_secStoreTz_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t le_pa_secStoreTz_LogSession;
le_log_Level_t* le_pa_secStoreTz_LogLevelFilterPtr;

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
__attribute__((constructor)) void _le_pa_secStoreTz_Init(void)
{
    LE_DEBUG("Initializing le_pa_secStoreTz component library.");

    // Connect client-side IPC interfaces.
    le_cfg_ConnectService();

    // Register the component with the Log Daemon.
    le_pa_secStoreTz_LogSession = le_log_RegComponent("le_pa_secStoreTz", &le_pa_secStoreTz_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
