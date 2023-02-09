/*
 * AUTO-GENERATED _componentMain.c for the install component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _install_le_avc_ServiceInstanceName;
const char** le_avc_ServiceInstanceNamePtr = &_install_le_avc_ServiceInstanceName;
void le_avc_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t install_LogSession;
le_log_Level_t* install_LogLevelFilterPtr;

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
__attribute__((constructor)) void _install_Init(void)
{
    LE_DEBUG("Initializing install component library.");

    // Connect client-side IPC interfaces.
    le_avc_ConnectService();

    // Register the component with the Log Daemon.
    install_LogSession = le_log_RegComponent("install", &install_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
