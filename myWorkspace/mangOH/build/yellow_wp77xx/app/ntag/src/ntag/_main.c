
// Startup code for the executable 'ntag'.
// This is a generated file, do not edit.

#include "legato.h"

#include <dlfcn.h>


// Define IPC API interface names.
LE_SHARED const char* _arduinoNtag_dhubIO_ServiceInstanceName = "dhubIO";
LE_SHARED const char* _ntagCmdApp_dhubIO_ServiceInstanceName = "dhubio2";

// Define default component's log session variables.
LE_SHARED le_log_SessionRef_t ntag_exe_LogSession;
LE_SHARED le_log_Level_t* ntag_exe_LogLevelFilterPtr;

// Loads a library using dlopen().
__attribute__((unused)) static void LoadLib
(
    const char* libName
)
{
    dlopen(libName, RTLD_LAZY | RTLD_GLOBAL);
    const char* errorMsg = dlerror();
    LE_FATAL_IF(errorMsg != NULL,
                "Failed to load library '%s' (%s)",                libName,
                errorMsg);
}


int main(int argc, const char* argv[])
{
    // Pass the args to the Command Line Arguments API.
    le_arg_SetArgs((size_t)argc, argv);
    // Make stdout line buffered so printf shows up in logs without flushing.
    setlinebuf(stdout);

    ntag_exe_LogSession = le_log_RegComponent("ntag_exe", &ntag_exe_LogLevelFilterPtr);

    // Connect to the log control daemon.
    // Note that there are some rare cases where we don't want the
    // process to try to connect to the Log Control Daemon (e.g.,
    // the Supervisor and the Service Directory shouldn't).
    // The NO_LOG_CONTROL macro can be used to control that.
    #ifndef NO_LOG_CONTROL
        le_log_ConnectToControlDaemon();
    #else
        LE_DEBUG("Not connecting to the Log Control Daemon.");
    #endif

    // Load dynamic libraries.
    LoadLib("libComponent_arduinoLibs.so");
    LoadLib("libComponent_arduinoNtag.so");
    LoadLib("libComponent_ntagCmdApp.so");

    // Set the Signal Fault handler
    le_sig_InstallShowStackHandler();

    // Set the Signal Term handler
    le_sig_InstallDefaultTermHandler();

    LE_DEBUG("== Starting Event Processing Loop ==");
    le_event_RunLoop();
    LE_FATAL("== SHOULDN'T GET HERE! ==");
}
