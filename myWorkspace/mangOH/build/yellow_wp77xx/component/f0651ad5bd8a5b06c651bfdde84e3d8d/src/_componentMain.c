/*
 * AUTO-GENERATED _componentMain.c for the avcDaemon component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _avcDaemon_le_instStat_ServiceInstanceName;
const char** le_instStat_ServiceInstanceNamePtr = &_avcDaemon_le_instStat_ServiceInstanceName;
void le_instStat_ConnectService(void);
extern const char* _avcDaemon_le_ulpm_ServiceInstanceName;
const char** le_ulpm_ServiceInstanceNamePtr = &_avcDaemon_le_ulpm_ServiceInstanceName;
void le_ulpm_ConnectService(void);
extern const char* _avcDaemon_le_appRemove_ServiceInstanceName;
const char** le_appRemove_ServiceInstanceNamePtr = &_avcDaemon_le_appRemove_ServiceInstanceName;
void le_appRemove_ConnectService(void);
extern const char* _avcDaemon_le_appCtrl_ServiceInstanceName;
const char** le_appCtrl_ServiceInstanceNamePtr = &_avcDaemon_le_appCtrl_ServiceInstanceName;
void le_appCtrl_ConnectService(void);
extern const char* _avcDaemon_le_cfg_ServiceInstanceName;
const char** le_cfg_ServiceInstanceNamePtr = &_avcDaemon_le_cfg_ServiceInstanceName;
void le_cfg_ConnectService(void);
extern const char* _avcDaemon_le_data_ServiceInstanceName;
const char** le_data_ServiceInstanceNamePtr = &_avcDaemon_le_data_ServiceInstanceName;
void le_data_ConnectService(void);
extern const char* _avcDaemon_le_appInfo_ServiceInstanceName;
const char** le_appInfo_ServiceInstanceNamePtr = &_avcDaemon_le_appInfo_ServiceInstanceName;
void le_appInfo_ConnectService(void);
extern const char* _avcDaemon_le_fwupdate_ServiceInstanceName;
const char** le_fwupdate_ServiceInstanceNamePtr = &_avcDaemon_le_fwupdate_ServiceInstanceName;
void le_fwupdate_ConnectService(void);
extern const char* _avcDaemon_le_update_ServiceInstanceName;
const char** le_update_ServiceInstanceNamePtr = &_avcDaemon_le_update_ServiceInstanceName;
void le_update_ConnectService(void);
extern const char* _avcDaemon_le_framework_ServiceInstanceName;
const char** le_framework_ServiceInstanceNamePtr = &_avcDaemon_le_framework_ServiceInstanceName;
void le_framework_ConnectService(void);
extern const char* _avcDaemon_le_secStore_ServiceInstanceName;
const char** le_secStore_ServiceInstanceNamePtr = &_avcDaemon_le_secStore_ServiceInstanceName;
void le_secStore_ConnectService(void);
extern const char* _avcDaemon_le_sms_ServiceInstanceName;
const char** le_sms_ServiceInstanceNamePtr = &_avcDaemon_le_sms_ServiceInstanceName;
void le_sms_ConnectService(void);
extern const char* _avcDaemon_le_sim_ServiceInstanceName;
const char** le_sim_ServiceInstanceNamePtr = &_avcDaemon_le_sim_ServiceInstanceName;
void le_sim_ConnectService(void);
extern const char* _avcDaemon_le_mdc_ServiceInstanceName;
const char** le_mdc_ServiceInstanceNamePtr = &_avcDaemon_le_mdc_ServiceInstanceName;
void le_mdc_ConnectService(void);
extern const char* _avcDaemon_le_cellnet_ServiceInstanceName;
const char** le_cellnet_ServiceInstanceNamePtr = &_avcDaemon_le_cellnet_ServiceInstanceName;
void le_cellnet_ConnectService(void);
extern const char* _avcDaemon_le_avc_ServiceInstanceName;
const char** le_avc_ServiceInstanceNamePtr = &_avcDaemon_le_avc_ServiceInstanceName;
void le_avc_AdvertiseService(void);
extern const char* _avcDaemon_le_avdata_ServiceInstanceName;
const char** le_avdata_ServiceInstanceNamePtr = &_avcDaemon_le_avdata_ServiceInstanceName;
void le_avdata_AdvertiseService(void);
extern const char* _avcDaemon_le_tpf_ServiceInstanceName;
const char** le_tpf_ServiceInstanceNamePtr = &_avcDaemon_le_tpf_ServiceInstanceName;
void le_tpf_AdvertiseService(void);
extern const char* _avcDaemon_le_coap_ServiceInstanceName;
const char** le_coap_ServiceInstanceNamePtr = &_avcDaemon_le_coap_ServiceInstanceName;
void le_coap_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t avcDaemon_LogSession;
le_log_Level_t* avcDaemon_LogLevelFilterPtr;

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
__attribute__((constructor)) void _avcDaemon_Init(void)
{
    LE_DEBUG("Initializing avcDaemon component library.");

    // Advertise server-side IPC interfaces.
    le_avc_AdvertiseService();
    le_avdata_AdvertiseService();
    le_tpf_AdvertiseService();
    le_coap_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_instStat_ConnectService();
    le_ulpm_ConnectService();
    le_appRemove_ConnectService();
    le_appCtrl_ConnectService();
    le_cfg_ConnectService();
    le_data_ConnectService();
    le_appInfo_ConnectService();
    le_fwupdate_ConnectService();
    le_update_ConnectService();
    le_framework_ConnectService();
    le_secStore_ConnectService();
    le_sms_ConnectService();
    le_sim_ConnectService();
    le_mdc_ConnectService();
    le_cellnet_ConnectService();

    // Register the component with the Log Daemon.
    avcDaemon_LogSession = le_log_RegComponent("avcDaemon", &avcDaemon_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
