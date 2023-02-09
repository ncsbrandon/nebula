/*
 * AUTO-GENERATED _componentMain.c for the imu component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _imu_dhubIO_ServiceInstanceName;
const char** dhubIO_ServiceInstanceNamePtr = &_imu_dhubIO_ServiceInstanceName;
void dhubIO_ConnectService(void);
extern const char* _imu_imu_ServiceInstanceName;
const char** imu_ServiceInstanceNamePtr = &_imu_imu_ServiceInstanceName;
void imu_AdvertiseService(void);
extern const char* _imu_temperature_ServiceInstanceName;
const char** temperature_ServiceInstanceNamePtr = &_imu_temperature_ServiceInstanceName;
void temperature_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t imu_LogSession;
le_log_Level_t* imu_LogLevelFilterPtr;

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
__attribute__((constructor)) void _imu_Init(void)
{
    LE_DEBUG("Initializing imu component library.");

    // Advertise server-side IPC interfaces.
    imu_AdvertiseService();
    temperature_AdvertiseService();

    // Connect client-side IPC interfaces.
    dhubIO_ConnectService();

    // Register the component with the Log Daemon.
    imu_LogSession = le_log_RegComponent("imu", &imu_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
