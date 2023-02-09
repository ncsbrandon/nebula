/*
 * AUTO-GENERATED _componentMain.c for the deviceManagement component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _deviceManagement_le_data_ServiceInstanceName;
const char** le_data_ServiceInstanceNamePtr = &_deviceManagement_le_data_ServiceInstanceName;
void le_data_ConnectService(void);
extern const char* _deviceManagement_le_ulpm_ServiceInstanceName;
const char** le_ulpm_ServiceInstanceNamePtr = &_deviceManagement_le_ulpm_ServiceInstanceName;
void le_ulpm_ConnectService(void);
extern const char* _deviceManagement_le_info_ServiceInstanceName;
const char** le_info_ServiceInstanceNamePtr = &_deviceManagement_le_info_ServiceInstanceName;
void le_info_ConnectService(void);
extern const char* _deviceManagement_le_ips_ServiceInstanceName;
const char** le_ips_ServiceInstanceNamePtr = &_deviceManagement_le_ips_ServiceInstanceName;
void le_ips_ConnectService(void);
extern const char* _deviceManagement_le_mdc_ServiceInstanceName;
const char** le_mdc_ServiceInstanceNamePtr = &_deviceManagement_le_mdc_ServiceInstanceName;
void le_mdc_ConnectService(void);
extern const char* _deviceManagement_le_mrc_ServiceInstanceName;
const char** le_mrc_ServiceInstanceNamePtr = &_deviceManagement_le_mrc_ServiceInstanceName;
void le_mrc_ConnectService(void);
extern const char* _deviceManagement_le_sim_ServiceInstanceName;
const char** le_sim_ServiceInstanceNamePtr = &_deviceManagement_le_sim_ServiceInstanceName;
void le_sim_ConnectService(void);
extern const char* _deviceManagement_le_sms_ServiceInstanceName;
const char** le_sms_ServiceInstanceNamePtr = &_deviceManagement_le_sms_ServiceInstanceName;
void le_sms_ConnectService(void);
extern const char* _deviceManagement_le_temp_ServiceInstanceName;
const char** le_temp_ServiceInstanceNamePtr = &_deviceManagement_le_temp_ServiceInstanceName;
void le_temp_ConnectService(void);
extern const char* _deviceManagement_le_gnss_ServiceInstanceName;
const char** le_gnss_ServiceInstanceNamePtr = &_deviceManagement_le_gnss_ServiceInstanceName;
void le_gnss_ConnectService(void);
extern const char* _deviceManagement_le_pos_ServiceInstanceName;
const char** le_pos_ServiceInstanceNamePtr = &_deviceManagement_le_pos_ServiceInstanceName;
void le_pos_ConnectService(void);
extern const char* _deviceManagement_le_cellnet_ServiceInstanceName;
const char** le_cellnet_ServiceInstanceNamePtr = &_deviceManagement_le_cellnet_ServiceInstanceName;
void le_cellnet_ConnectService(void);
extern const char* _deviceManagement_le_fwupdate_ServiceInstanceName;
const char** le_fwupdate_ServiceInstanceNamePtr = &_deviceManagement_le_fwupdate_ServiceInstanceName;
void le_fwupdate_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t deviceManagement_LogSession;
le_log_Level_t* deviceManagement_LogLevelFilterPtr;

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
__attribute__((constructor)) void _deviceManagement_Init(void)
{
    LE_DEBUG("Initializing deviceManagement component library.");

    // Connect client-side IPC interfaces.
    le_data_ConnectService();
    le_ulpm_ConnectService();
    le_info_ConnectService();
    le_ips_ConnectService();
    le_mdc_ConnectService();
    le_mrc_ConnectService();
    le_sim_ConnectService();
    le_sms_ConnectService();
    le_temp_ConnectService();
    le_gnss_ConnectService();
    le_pos_ConnectService();
    le_cellnet_ConnectService();
    le_fwupdate_ConnectService();

    // Register the component with the Log Daemon.
    deviceManagement_LogSession = le_log_RegComponent("deviceManagement", &deviceManagement_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
