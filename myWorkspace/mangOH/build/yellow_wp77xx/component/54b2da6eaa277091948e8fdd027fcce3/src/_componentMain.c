/*
 * AUTO-GENERATED _componentMain.c for the audio component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _audio_le_pm_ServiceInstanceName;
const char** le_pm_ServiceInstanceNamePtr = &_audio_le_pm_ServiceInstanceName;
void le_pm_ConnectService(void);
extern const char* _audio_le_audio_ServiceInstanceName;
const char** le_audio_ServiceInstanceNamePtr = &_audio_le_audio_ServiceInstanceName;
void le_audio_AdvertiseService(void);
// Component log session variables.
le_log_SessionRef_t audio_LogSession;
le_log_Level_t* audio_LogLevelFilterPtr;

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
__attribute__((constructor)) void _audio_Init(void)
{
    LE_DEBUG("Initializing audio component library.");

    // Advertise server-side IPC interfaces.
    le_audio_AdvertiseService();

    // Connect client-side IPC interfaces.
    le_pm_ConnectService();

    // Register the component with the Log Daemon.
    audio_LogSession = le_log_RegComponent("audio", &audio_LogLevelFilterPtr);

    // Queue the default component's COMPONENT_INIT_ONCE to Event Loop.
    le_event_QueueFunction(&COMPONENT_INIT_ONCE_NAME, NULL, NULL);

    // Queue the COMPONENT_INIT function to be called by the event loop
    le_event_QueueFunction(&COMPONENT_INIT_NAME, NULL, NULL);
}


#ifdef __cplusplus
}
#endif
