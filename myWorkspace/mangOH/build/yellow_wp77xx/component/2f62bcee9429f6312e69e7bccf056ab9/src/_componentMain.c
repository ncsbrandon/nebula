/*
 * AUTO-GENERATED _componentMain.c for the atAirVantage component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _atAirVantage_le_atServer_ServiceInstanceName;
const char** le_atServer_ServiceInstanceNamePtr = &_atAirVantage_le_atServer_ServiceInstanceName;
void le_atServer_ConnectService(void);
extern const char* _atAirVantage_le_avc_ServiceInstanceName;
const char** le_avc_ServiceInstanceNamePtr = &_atAirVantage_le_avc_ServiceInstanceName;
void le_avc_ConnectService(void);
extern const char* _atAirVantage_le_mdc_ServiceInstanceName;
const char** le_mdc_ServiceInstanceNamePtr = &_atAirVantage_le_mdc_ServiceInstanceName;
void le_mdc_ConnectService(void);
extern const char* _atAirVantage_le_mrc_ServiceInstanceName;
const char** le_mrc_ServiceInstanceNamePtr = &_atAirVantage_le_mrc_ServiceInstanceName;
void le_mrc_ConnectService(void);
extern const char* _atAirVantage_le_data_ServiceInstanceName;
const char** le_data_ServiceInstanceNamePtr = &_atAirVantage_le_data_ServiceInstanceName;
void le_data_ConnectService(void);
extern const char* _atAirVantage_le_port_ServiceInstanceName;
const char** le_port_ServiceInstanceNamePtr = &_atAirVantage_le_port_ServiceInstanceName;
void le_port_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t atAirVantage_LogSession;
le_log_Level_t* atAirVantage_LogLevelFilterPtr;

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
__attribute__((constructor)) void _atAirVantage_Init(void)
{
    LE_DEBUG("Initializing atAirVantage component library.");

    // Connect client-side IPC interfaces.
    le_atServer_ConnectService();
    le_avc_ConnectService();
    le_mdc_ConnectService();
    le_mrc_ConnectService();
    le_data_ConnectService();
    le_port_ConnectService();

    // Register the component with the Log Daemon.
    atAirVantage_LogSession = le_log_RegComponent("atAirVantage", &atAirVantage_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
