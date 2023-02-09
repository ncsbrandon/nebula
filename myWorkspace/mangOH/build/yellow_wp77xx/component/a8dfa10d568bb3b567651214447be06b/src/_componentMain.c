/*
 * AUTO-GENERATED _componentMain.c for the NtagDhubIfComponent component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _NtagDhubIfComponent_fieldDetectGpio_ServiceInstanceName;
const char** fieldDetectGpio_ServiceInstanceNamePtr = &_NtagDhubIfComponent_fieldDetectGpio_ServiceInstanceName;
void fieldDetectGpio_ConnectService(void);
extern const char* _NtagDhubIfComponent_dhubIO_ServiceInstanceName;
const char** dhubIO_ServiceInstanceNamePtr = &_NtagDhubIfComponent_dhubIO_ServiceInstanceName;
void dhubIO_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t NtagDhubIfComponent_LogSession;
le_log_Level_t* NtagDhubIfComponent_LogLevelFilterPtr;

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
__attribute__((constructor)) void _NtagDhubIfComponent_Init(void)
{
    LE_DEBUG("Initializing NtagDhubIfComponent component library.");

    // Connect client-side IPC interfaces.
    fieldDetectGpio_ConnectService();
    dhubIO_ConnectService();

    // Register the component with the Log Daemon.
    NtagDhubIfComponent_LogSession = le_log_RegComponent("NtagDhubIfComponent", &NtagDhubIfComponent_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
