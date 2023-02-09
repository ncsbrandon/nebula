/*
 * AUTO-GENERATED _componentMain.c for the bsecIntegrationComponent component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _bsecIntegrationComponent_mangOH_bme680_ServiceInstanceName;
const char** mangOH_bme680_ServiceInstanceNamePtr = &_bsecIntegrationComponent_mangOH_bme680_ServiceInstanceName;
void mangOH_bme680_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t bsecIntegrationComponent_LogSession;
le_log_Level_t* bsecIntegrationComponent_LogLevelFilterPtr;

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
__attribute__((constructor)) void _bsecIntegrationComponent_Init(void)
{
    LE_DEBUG("Initializing bsecIntegrationComponent component library.");

    // Advertise server-side IPC interfaces.
    mangOH_bme680_AdvertiseService();

    // Register the component with the Log Daemon.
    bsecIntegrationComponent_LogSession = le_log_RegComponent("bsecIntegrationComponent", &bsecIntegrationComponent_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
