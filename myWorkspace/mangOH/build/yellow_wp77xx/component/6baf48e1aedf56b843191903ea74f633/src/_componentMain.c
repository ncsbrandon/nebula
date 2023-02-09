/*
 * AUTO-GENERATED _componentMain.c for the spiService component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _spiService_le_spi_ServiceInstanceName;
const char** le_spi_ServiceInstanceNamePtr = &_spiService_le_spi_ServiceInstanceName;
void le_spi_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t spiService_LogSession;
le_log_Level_t* spiService_LogLevelFilterPtr;

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
__attribute__((constructor)) void _spiService_Init(void)
{
    LE_DEBUG("Initializing spiService component library.");

    // Advertise server-side IPC interfaces.
    le_spi_AdvertiseService();

    // Register the component with the Log Daemon.
    spiService_LogSession = le_log_RegComponent("spiService", &spiService_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
