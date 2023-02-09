
//--------------------------------------------------------------------------------------------------
/**
 * The socialSerice twitter command line app.  While not strictly required, it allows the user to
 * handle the required authentication of the service without having to build a custom application
 * that makes use of the serviceConfig API.
 *
 * Refer to the help text for more details on how to run this utility.
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include "legato.h"
#include "interfaces.h"




//--------------------------------------------------------------------------------------------------
/**
 * ID of the command being executed.
 */
//--------------------------------------------------------------------------------------------------
enum class Command_t
{
    Unset,
    SetKeys,
    RequestAuthentication,
    AuthenticatePin,
    Tweet
};




//--------------------------------------------------------------------------------------------------
/**
 * Print a useful help message to the user and then quit.
 */
//--------------------------------------------------------------------------------------------------
static void PrintHelp
(
    void
)
//--------------------------------------------------------------------------------------------------
{
    std::cout << "There are two things this command line client can do." << std::endl
              << std::endl
              << "  1) Authenticate the Twitter client service." << std::endl
              << "  2) Send out tweets." << std::endl
              << std::endl
              << "There are two things this command line client can do." << std::endl
              << "To authenticate the service, first you must install" << std::endl
              << "the consumer keys.  These keys are given to you by" << std::endl
              << "twitter and are supplied to the service thusly:" << std::endl
              << "" << std::endl
              << "    twitter --consumerKey --public=<consumer-public> --secret=<consumer-secret>"
              << std::endl
              << "" << std::endl
              << "Once that is done, you can request a PIN URL:" << std::endl
              << "" << std::endl
              << "    twitter --requestAuthentication" << std::endl
              << "" << std::endl
              << "If successful a PIN URL will be printed on the" << std::endl
              << "console.  Go to this URL and authorize the" << std::endl
              << "application.  Once done Twitter will give you a PIN," << std::endl
              << "so enter this PIN into the service:" << std::endl
              << "" << std::endl
              << "    twitter --authenticatePin=<pin>" << std::endl
              << "" << std::endl
              << "Once successfully registered you will be able to tweet" << std::endl
              << "either from the command line, or through the Twitter" << std::endl
              << "API." << std::endl
              << "" << std::endl
              << "-----------------------------------------------------" << std::endl
              << "" << std::endl
              << "To send out a Tweet, simply do:" << std::endl
              << "" << std::endl
              << "    twitter --tweet Hello world, how are you today?" << std::endl
              << "" << std::endl
              << "  * Note the lack of required quotes, \"." << std::endl
              << "" << std::endl
              << "-----------------------------------------------------" << std::endl
              << "" << std::endl
              << "Summary:" << std::endl
              << "" << std::endl
              << "    Set consumer public and private key." << std::endl
              << "    --consumerKey, -k" << std::endl
              << "        --public, -p" << std::endl
              << "        --secret, -s" << std::endl
              << "" << std::endl
              << "    Request an authentication URL." << std::endl
              << "    --requestAuthentication, -r" << std::endl
              << "" << std::endl
              << "    Authenticate the PIN." << std::endl
              << "    --authenticatePin, -a" << std::endl
              << "" << std::endl
              << "    Send a Tweet to the world." << std::endl
              << "    --tweet, -t" << std::endl;

    exit(EXIT_SUCCESS);
}




//--------------------------------------------------------------------------------------------------
/**
 * Called to make sure that the user hasn't requested two separate top level commands.
 */
//--------------------------------------------------------------------------------------------------
static void CheckUnique
(
    Command_t command  ///< [IN] The current command selected.
)
//--------------------------------------------------------------------------------------------------
{
    if (command != Command_t::Unset)
    {
        std::cerr << "Multiple top level commands requested.  Refer to "
                  << "twitter --help for more information."
                  << std::endl;

        exit(EXIT_FAILURE);
    }
}




//--------------------------------------------------------------------------------------------------
/**
 * Set the consumer keys for the twitter client service.
 *
 * @return Either EXIT_SUCCESS, or EXIT_FAILURE.
 */
//--------------------------------------------------------------------------------------------------
static int SetKeys
(
    const std::string& publicKey,  ///< [IN] Key to set.
    const std::string& secretKey   ///< [IN] Key to set.
)
//--------------------------------------------------------------------------------------------------
{
    int exitValue = EXIT_SUCCESS;
    le_result_t result = serviceConfig_SetConsumerKeys(publicKey.c_str(), secretKey.c_str());

    if (result != LE_OK)
    {
        exitValue = EXIT_FAILURE;
        std::cerr << "Failed to write the consumer keys: " << LE_RESULT_TXT(result) << "."
                  << std::endl;
    }

    return exitValue;
}




//--------------------------------------------------------------------------------------------------
/**
 * Request an authentication URL from the Twitter server.  Once received, print a helpful message.
 *
 * @return Either EXIT_SUCCESS, or EXIT_FAILURE.
 */
//--------------------------------------------------------------------------------------------------
static int RequestAuthentication
(
    void
)
//--------------------------------------------------------------------------------------------------
{
    int exitValue = EXIT_SUCCESS;

    char urlStr[SERVICECONFIG_URL_LEN + 1];
    le_result_t result = serviceConfig_GetPinUrl(urlStr, sizeof(urlStr));

    if (result == LE_COMM_ERROR)
    {
        std::cerr << "Could not communicate with Twitter." << std::endl;
        exitValue = EXIT_FAILURE;
    }
    else if (result == LE_UNAVAILABLE)
    {
        std::cerr << "Could not save the OAuth keys." << std::endl;
        exitValue = EXIT_FAILURE;
    }
    else
    {
        std::cout << "To authenticate this device go to:" << std::endl
                  << std::endl
                  << "    " << urlStr << std::endl
                  << std::endl
                  << "Once there you should receive a PIN.  Record that PIN and run:" << std::endl
                  << std::endl
                  << "    twitter --authenticatePin=<pin-#>" << std::endl
                  << std::endl
                  << "    Where <pin-#> is the PIN you received from Twitter." << std::endl;
    }

    return exitValue;
}




//--------------------------------------------------------------------------------------------------
/**
 * Send the user entered PIN back to the Twitter server.
 *
 * @return Either EXIT_SUCCESS, or EXIT_FAILURE.
 */
//--------------------------------------------------------------------------------------------------
static int AuthenticatePin
(
    const std::string& newPin
)
//--------------------------------------------------------------------------------------------------
{
    le_result_t result = serviceConfig_TransmitUserPin(newPin.c_str());

    if (result != LE_OK)
    {
        std::cerr << "Failed to confirm pin, " << LE_RESULT_TXT(result) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Pin confirmation successful." << std::endl;
    return EXIT_SUCCESS;
}




//--------------------------------------------------------------------------------------------------
/**
 * Send a tweet out into the world.
 */
//--------------------------------------------------------------------------------------------------
static int SendTweet
(
    const std::string& message  ///< [IN] The message to send.
)
//--------------------------------------------------------------------------------------------------
{
    le_result_t result = twitter_Tweet(message.c_str());

    if (result == LE_NOT_PERMITTED)
    {
        std::cerr << "The social service has not been authenticated with Twitter." << std::endl;
        return EXIT_FAILURE;
    }
    if (result != LE_OK)
    {
        std::cerr << "Message transmission failed." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}




static Command_t TheCommand = Command_t::Unset;

static const char* PublicKey;
static const char* SecretKey;

static std::string NewPin;
static std::string Message;



COMPONENT_INIT
{
    // Register our command line parameter handlers.
    le_arg_SetFlagCallback(PrintHelp, NULL, "help");

    le_arg_SetFlagCallback(
        []()
        {
            CheckUnique(TheCommand);

            TheCommand = Command_t::SetKeys;

            le_arg_SetStringVar(&PublicKey, "p", "public");
            le_arg_SetStringVar(&SecretKey, "s", "secret");
        },
        "k",
        "consumerKey");

    le_arg_SetFlagCallback(
        []()
        {
            CheckUnique(TheCommand);
            TheCommand = Command_t::RequestAuthentication;
        },
        "r",
        "requestAuthentication");

    le_arg_SetStringCallback(
        [](const char* value)
        {
            CheckUnique(TheCommand);
            TheCommand = Command_t::AuthenticatePin;
            NewPin = value;
        },
        "a",
        "authenticatePin");

    le_arg_SetFlagCallback(
        []()
        {
            CheckUnique(TheCommand);
            TheCommand = Command_t::Tweet;

            le_arg_AllowMorePositionalArgsThanCallbacks();

            le_arg_AddPositionalCallback(
                [](const char* message)
                {
                    if (Message.size() != 0)
                    {
                        Message += " ";
                    }

                    Message += message;
                });
        },
        "t",
        "tweet");

    // Call the argument scanner to process the command line arguments.
    le_arg_Scan();

    // Now that we know what command was requested, attempt to perform the requested action.
    switch (TheCommand)
    {
        case Command_t::Unset:
            std::cerr << "No top level commands requested.  Refer to "
                      << le_arg_GetProgramName() << " --help for more information."
                      << std::endl;
            exit(EXIT_FAILURE);

        case Command_t::SetKeys:                exit(SetKeys(PublicKey, SecretKey));
        case Command_t::RequestAuthentication:  exit(RequestAuthentication());
        case Command_t::AuthenticatePin:        exit(AuthenticatePin(NewPin));
        case Command_t::Tweet:                  exit(SendTweet(Message));
    }
}
