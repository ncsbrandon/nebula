/*
 * AUTO-GENERATED _componentMain.c for the atClient component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _atClient_le_atClient_ServiceInstanceName;
const char** le_atClient_ServiceInstanceNamePtr = &_atClient_le_atClient_ServiceInstanceName;
void le_atClient_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t atClient_LogSession;
le_log_Level_t* atClient_LogLevelFilterPtr;

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
__attribute__((constructor)) void _atClient_Init(void)
{
    LE_DEBUG("Initializing atClient component library.");

    // Advertise server-side IPC interfaces.
    le_atClient_AdvertiseService();

    // Register the component with the Log Daemon.
    atClient_LogSession = le_log_RegComponent("atClient", &atClient_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
