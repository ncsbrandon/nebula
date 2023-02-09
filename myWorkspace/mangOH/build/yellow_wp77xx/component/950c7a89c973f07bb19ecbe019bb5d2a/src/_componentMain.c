/*
 * AUTO-GENERATED _componentMain.c for the bsec component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _bsec_io_ServiceInstanceName;
const char** io_ServiceInstanceNamePtr = &_bsec_io_ServiceInstanceName;
void io_ConnectService(void);
extern const char* _bsec_mangOH_bme680_ServiceInstanceName;
const char** mangOH_bme680_ServiceInstanceNamePtr = &_bsec_mangOH_bme680_ServiceInstanceName;
void mangOH_bme680_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t bsec_LogSession;
le_log_Level_t* bsec_LogLevelFilterPtr;

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
__attribute__((constructor)) void _bsec_Init(void)
{
    LE_DEBUG("Initializing bsec component library.");

    // Connect client-side IPC interfaces.
    io_ConnectService();
    mangOH_bme680_ConnectService();

    // Register the component with the Log Daemon.
    bsec_LogSession = le_log_RegComponent("bsec", &bsec_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
