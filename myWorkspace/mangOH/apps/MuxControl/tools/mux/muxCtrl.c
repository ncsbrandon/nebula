/**
 * @file
 *
 * This file implements a simple command line program for manually controlling
 * the peripheral muxes in the mangOH Green.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless, Inc. Use of this work is subject to license.
 */

/* Legato Framework */
#include "legato.h"
#include "interfaces.h"
#include "le_print.h"

//--------------------------------------------------------------------------------------------------
/**
 * Definition for connect service function pointer
 */
//--------------------------------------------------------------------------------------------------
typedef void (*ConnectServiceFunc_t)
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * A table that lists the commands supported by the mux program
  */
//--------------------------------------------------------------------------------------------------
static struct
{
    le_result_t (* function)(void);
    const char* description;
} commands[] =
{
    {
        .function=mangoh_muxCtrl_IotAllUart1Off,
        .description="Turn UART 1 off for all IoT slots"
    },
    {
        .function=mangoh_muxCtrl_Iot0Uart1On,
        .description="Turn UART 1 on for IoT slot 0"
    },
    {
        .function=mangoh_muxCtrl_Iot1Uart1On,
        .description="Turn UART 1 on for IoT slot 1"
    },
    {
        .function=mangoh_muxCtrl_IotAllSpiOff,
        .description="Turn SPI off for all IoT slots"
    },
    {
        .function=mangoh_muxCtrl_Iot0Spi1On,
        .description="Turn SPI on for IoT slot 0"
    },
    {
        .function=mangoh_muxCtrl_Iot1Spi1On,
        .description="Turn SPI on for IoT slot 1"
    },
    {
        .function=mangoh_muxCtrl_IotAllUart2Off,
        .description="Turn UART 2 off for all IoT slots"
    },
    {
        .function=mangoh_muxCtrl_Iot2Uart2On,
        .description="Turn UART 2 on for IoT slot 2"
    },
    {
        .function=mangoh_muxCtrl_Uart2DebugOn,
        .description="Turn UART 2 on for debug port"
    },
    {
        .function=mangoh_muxCtrl_SdioSelMicroSd,
        .description="Select the microSD card on the SDIO mux"
    },
    {
        .function=mangoh_muxCtrl_SdioSelIot0,
        .description="Select IoT slot 0 on the SDIO mux"
    },
    {
        .function=mangoh_muxCtrl_AudioDisable,
        .description="Disable audio"
    },
    {
        .function=mangoh_muxCtrl_AudioSelectIot0Codec,
        .description="Select the audio codec in IoT slot 0"
    },
    {
        .function=mangoh_muxCtrl_AudioSelectOnboardCodec,
        .description="Select the audio codec on the mangOH board"
    },
    {
        .function=mangoh_muxCtrl_AudioSelectInternalCodec,
        .description="Select the audio codec internal to the CF3 module"
    },
    {
        .function=mangoh_muxCtrl_IotSlot0DeassertReset,
        .description="Reset IoT slot 0"
    },
    {
        .function=mangoh_muxCtrl_IotSlot1DeassertReset,
        .description="Reset IoT slot 1"
    },
    {
        .function=mangoh_muxCtrl_IotSlot2DeassertReset,
        .description="Reset IoT slot 2"
    },
    {
        .function=mangoh_muxCtrl_ArduinoReset,
        .description="Reset Arduino"
    },
};

//--------------------------------------------------------------------------------------------------
/**
 * programOptions holds information about what options were passed to the mux command.
  */
//--------------------------------------------------------------------------------------------------
static struct
{
    bool helpRequested;
    bool commandSupplied;
    bool validCommandSupplied;
    int command;
} programOptions;


//--------------------------------------------------------------------------------------------------
/**
 * Help Message
 */
//--------------------------------------------------------------------------------------------------
static char* HelpMessage = "\
NAME:\n\
    mux - mangOH GPIO Mux Control tool\n\
\n\
SYNOPSIS:\n\
    mux [--help] [<command_num>]\n\
\n\
DESCRIPTION:\n\
    -h, --help\n\
        Display this help and exit.\n\
\n\
    Commands:\n\
";


//--------------------------------------------------------------------------------------------------
/**
 * Print the help message to stdout
 *
 * @return
 *      LE_OK
 *
 * @note
 *      This function exits with EXIT_FAILURE if errorMessage is not NULL.
 */
//--------------------------------------------------------------------------------------------------
static void PrintHelp
(
    const char *errorMessage
)
{
    FILE *fh = stdout;

    if (errorMessage)
    {
        fh = stderr;
        fputs("ERROR: ", fh);
        fputs(errorMessage, fh);
        fputs("\n", fh);
    }

    fputs(HelpMessage, fh);
    for (int i = 0; i < NUM_ARRAY_MEMBERS(commands); i++)
    {
        fprintf(fh, "        %2d. %s\n", i, commands[i].description);
    }
    fputs("\n", fh);

    if (errorMessage)
    {
        exit(EXIT_FAILURE);
    }
}

//--------------------------------------------------------------------------------------------------
/**
 * Thread used to recover from problems connecting to a service, probably because the service is
 * down.  It will timeout after 5 seconds, print an error message, and then exit.
 *
 * Once successfully connected to the service, this thread should be stopped.
 */
//--------------------------------------------------------------------------------------------------
static void* TimeoutThread
(
    void* contextPtr  ///< Not used
)
{
    // This thread doesn't have to do anything else, at least for now, so just sleep.
    sleep(5);

    fputs("Error: Can't connect to muxCtrlService.  Is it running?\n", stderr);
    exit(EXIT_FAILURE);

    return NULL;
}

//--------------------------------------------------------------------------------------------------
/**
 * Try calling the given function to connect to a service.  If can't connect to the service within
 * 5 seconds, then the program will exit.
 */
//--------------------------------------------------------------------------------------------------
static void TryConnect
(
    ConnectServiceFunc_t connectFuncPtr  ///< Function to call to connect to service
)
{
    // Print out message before trying to connect to service to give user some kind of feedback
    printf("Connecting to service ...\n");
    fflush(stdout);

    // Use a separate thread for recovery.  It will be stopped once connected to the service.
    // Make the thread joinable, so we can be sure the thread is stopped before continuing.
    le_thread_Ref_t threadRef = le_thread_Create("timeout thread", TimeoutThread, NULL);
    le_thread_SetJoinable(threadRef);
    le_thread_Start(threadRef);

    // Try connecting to the service
    connectFuncPtr();

    // Connected to the service, so stop the timeout thread
    le_thread_Cancel(threadRef);
    le_thread_Join(threadRef, NULL);
}

//--------------------------------------------------------------------------------------------------
/**
 * Executes the command with the given number.
 *
 * @note
 *      The command number is not validated by this function.
 */
//--------------------------------------------------------------------------------------------------
static void ExecuteCommand
(
    int command  ///< Command number to execute
)
{
    TryConnect(mangoh_muxCtrl_ConnectService);

    printf("Executing %d: %s\n", command, commands[command].description);
    le_result_t result = (*(commands[command].function))();
    if (result != LE_OK)
    {
        fprintf(stderr, "Command failed with result (%d)!\n", result);
    }
    else
    {
        printf("Success\n");
    }
}

//--------------------------------------------------------------------------------------------------
/**
 * Tries to parse a string into a valid command int and updates programOptions accordingly.
 */
//--------------------------------------------------------------------------------------------------
static void ParseCommand(
    const char* cmdPtr  ///< String containing candidate command number
)
{
    programOptions.commandSupplied = true;
    le_result_t parseResult = le_utf8_ParseInt(&programOptions.command, cmdPtr);
    programOptions.validCommandSupplied =
        (parseResult == LE_OK) &&
        programOptions.command >= 0 &&
        programOptions.command < NUM_ARRAY_MEMBERS(commands);
}

//--------------------------------------------------------------------------------------------------
/**
 * Handles errors generated by command line argument processing
 *
 * @return
 *      Causes exit(EXIT_FAILURE), so never returns
 */
//--------------------------------------------------------------------------------------------------
static size_t ArgumentErrorHandler
(
    size_t  argIndex,       ///< Index of argument that is bad (0 = first arg after program name).
    le_result_t errorCode   ///< Code indicating the type of error that was encountered.
)
{
    const char* errorString;
    char formattedErrorString[128];

    switch (errorCode)
    {
        case LE_OUT_OF_RANGE:
            errorString = "Supplied command is invalid\n";
            break;

        case LE_OVERFLOW:
            errorString = "Too many arguments\n";
            break;

        case LE_UNSUPPORTED:
            errorString = formattedErrorString;
            snprintf(
                formattedErrorString,
                NUM_ARRAY_MEMBERS(formattedErrorString),
                "Argument \"%s\" should not have a value\n",
                le_arg_GetArg(argIndex));
            break;

        case LE_BAD_PARAMETER:
            errorString = formattedErrorString;
            snprintf(
                formattedErrorString,
                NUM_ARRAY_MEMBERS(formattedErrorString),
                "Argument \"%s\" is not allowed\n",
                le_arg_GetArg(argIndex));
            break;

        // Should never happen
        case LE_NOT_FOUND:
        case LE_FORMAT_ERROR:
        case LE_UNDERFLOW:
        default:
            LE_CRIT("Unexpected error code %d at argument index %zu.", errorCode, argIndex);
            errorString = "Internal fault: Unexpected error";
            break;
    }

    PrintHelp(errorString);

    return 0;
}

COMPONENT_INIT
{
    // Allow for the possibility that the user didn't specify a command number
    le_arg_AllowLessPositionalArgsThanCallbacks();
    le_arg_SetFlagVar(&programOptions.helpRequested, "h", "help");
    le_arg_AddPositionalCallback(ParseCommand);
    le_arg_SetErrorHandler(ArgumentErrorHandler);
    le_arg_Scan();

    if (programOptions.helpRequested)
    {
        PrintHelp(NULL);
    }
    else if (programOptions.commandSupplied)
    {
        if (programOptions.validCommandSupplied)
        {
            ExecuteCommand(programOptions.command);
        }
        else
        {
            PrintHelp("Supplid command is invalid\n");
        }
    }
    else
    {
        PrintHelp("No command was specified\n");
    }

    exit(0);
}
