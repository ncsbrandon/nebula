
//--------------------------------------------------------------------------------------------------
/**
 * Implementation of the socialConfig and twitter APIs.
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#include <sstream>
#include "legato.h"
#include "interfaces.h"
#include "json.h"
#include "http.h"



// Names to refer to the application credentials in secure storage.
static const std::string ConsumerKeyName = "ConsumerKeys";
static const std::string OAuthKeyName = "OAuthKeys";
static const std::string OAuthTempKeyName = "OAuthTempKeys";




//--------------------------------------------------------------------------------------------------
/**
 * Load a public/secret key set from the device secure storage.
 *
 * @return - LE_OK if successful.
 *         - LE_NOT_FOUND if the item doesn't exist.
 *         - LE_UNAVAILABLE if the secure storage is currently unavailable.
 *         - LE_FAULT if there was some other error.
 */
//--------------------------------------------------------------------------------------------------
static le_result_t LoadKeys
(
    const std::string& keyName,   ///< [IN]  Name to load the keys from.
    http::KeyPair_t& credentials  ///< [OUT] Fill in this credential structure.
)
//--------------------------------------------------------------------------------------------------
{
    le_cfg_IteratorRef_t iterRef = le_cfg_CreateReadTxn(keyName.c_str());

    char buffer[LE_CFG_STR_LEN_BYTES] = "";
    le_result_t result = le_cfg_GetString(iterRef, "./public", buffer, sizeof(buffer), "");

    LE_ASSERT(result != LE_OVERFLOW);

    credentials.publicKey = buffer;

    result = le_cfg_GetString(iterRef, "./secret", buffer, sizeof(buffer), "");

    LE_ASSERT(result != LE_OVERFLOW);

    if (result == LE_OK)
    {
        credentials.secretKey = buffer;
    }

    le_cfg_CancelTxn(iterRef);

    return result;
}




//--------------------------------------------------------------------------------------------------
/**
 * Save a public/secret key set in the secure storage.
 *
 * @return - LE_OK if successful.
 *         - LE_NOT_FOUND if the item doesn't exist.
 *         - LE_UNAVAILABLE if the secure storage is currently unavailable.
 *         - LE_FAULT if there was some other error.
 */
//--------------------------------------------------------------------------------------------------
static le_result_t SaveKeys
(
    const std::string& keyName,         ///< [IN] Name of the key set to save.
    const http::KeyPair_t& credentials  ///< [IN] The keys to save.
)
//--------------------------------------------------------------------------------------------------
{
    le_cfg_IteratorRef_t iterRef = le_cfg_CreateWriteTxn(keyName.c_str());

    le_cfg_SetString(iterRef, "./public", credentials.publicKey.c_str());
    le_cfg_SetString(iterRef, "./secret", credentials.secretKey.c_str());

    le_cfg_CommitTxn(iterRef);

    return LE_OK;
}




//--------------------------------------------------------------------------------------------------
/**
 * Load a consumer/OAuth key set from secure storage.
 *
 * @return - LE_OK if successful.
 *         - LE_UNAVAILABLE if the load fails.
 */
//--------------------------------------------------------------------------------------------------
static le_result_t LoadKeys
(
    const std::string& consumerKeyName,  ///< [IN] Key set name.
    const std::string& oAuthKeyName,     ///< [IN] Key set name.
    http::Credentials_t& credentials     ///< [OUT] Buffer to hold the loaded keys.
)
//--------------------------------------------------------------------------------------------------
{
    le_result_t result = LoadKeys(consumerKeyName, credentials.consumer);

    if (result != LE_OK)
    {
        LE_ERROR("Could not load key set %s: %s", consumerKeyName.c_str(), LE_RESULT_TXT(result));
        return LE_UNAVAILABLE;
    }

    result = LoadKeys(oAuthKeyName, credentials.oAuthAccess);

    if (result != LE_OK)
    {
        LE_ERROR("Could not load key set %s: %s", oAuthKeyName.c_str(), LE_RESULT_TXT(result));
        return LE_UNAVAILABLE;
    }

    return LE_OK;
}



//--------------------------------------------------------------------------------------------------
/**
 * Delete an OAuth key set from the secure storage.
 */
//--------------------------------------------------------------------------------------------------
static void DeleteKeys
(
    const std::string& keyName  ///< [IN] Name of the key set to delete.
)
//--------------------------------------------------------------------------------------------------
{
    le_cfg_QuickDeleteNode(keyName.c_str());
}




//--------------------------------------------------------------------------------------------------
/**
 * Call to check to see if the service is fully authenticated against the Twitter server.
 *
 * @return - True if the authentication has been completed, false if not.
 */
//--------------------------------------------------------------------------------------------------
bool serviceConfig_IsAuthenticated
(
    void
)
//--------------------------------------------------------------------------------------------------
{
    le_cfg_IteratorRef_t iterRef = le_cfg_CreateReadTxn("");

    std::string name = ConsumerKeyName + "/public";
    bool exists = le_cfg_NodeExists(iterRef, name.c_str());

    if (exists == false)
    {
        le_cfg_CancelTxn(iterRef);
        return false;
    }

    name = OAuthKeyName + "/public";
    exists = le_cfg_NodeExists(iterRef, name.c_str());

    le_cfg_CancelTxn(iterRef);

    return exists;
}




//--------------------------------------------------------------------------------------------------
/**
 * Configure the identity of the client application.  This should only need to be done once, however
 * it must be done before GetPinUrl and TransmitUserPin are called.
 *
 * This is because the server needs to know the identity of the client before it will communicate
 * with it.
 *
 * @return - LE_OK if successful.
 *         - LE_UNAVAILABLE if the write fails, or if the secure storage service is unavailable.
 */
//--------------------------------------------------------------------------------------------------
le_result_t serviceConfig_SetConsumerKeys
(
    const char* publicKey,  ///< [IN] Application public key.
    const char* secretKey   ///< [IN] The secret key that shall not be shared.
)
//--------------------------------------------------------------------------------------------------
{
    le_result_t result = SaveKeys(ConsumerKeyName, { publicKey, secretKey });

    if (result != LE_OK)
    {
        LE_ERROR("Could not set consumer key set, %s", LE_RESULT_TXT(result));
        return LE_UNAVAILABLE;
    }

    return LE_OK;
}




//--------------------------------------------------------------------------------------------------
/**
 * Request an authentication PIN URL be generated.  This URL is then given to the user to visit.  On
 * connecting the user will be given an unique PIN code.  This code is then passed to the
 * TransmitUserPin to fully authorize the client.
 *
 * @return - LE_OK if successful.
 *         - LE_COMM_ERROR if the server can not be contacted.
 *         - LE_UNAVAILABLE if the secure storage service is unavailable.
 */
//--------------------------------------------------------------------------------------------------
le_result_t serviceConfig_GetPinUrl
(
    char* url,            ///< [OUT] A URL string for the user to enter into their web-browser.
    size_t urlBufferSize  ///< [IN]  Size of the URL buffer.
)
//--------------------------------------------------------------------------------------------------
{
    http::KeyPair_t consumerKeys;
    le_result_t result = LoadKeys(ConsumerKeyName, consumerKeys);

    if (result != LE_OK)
    {
        LE_ERROR("Could not load consumer key set: %s", LE_RESULT_TXT(result));
        return LE_UNAVAILABLE;
    }

    try
    {
        http::KeyPair_t oAuthKeys;
        oAuthKeys = http::RequestTempAccessTokens(consumerKeys,
                                                  "https://api.twitter.com/oauth/request_token");

        int n = snprintf(url,
                         urlBufferSize,
                         "http://twitter.com/oauth/authorize?oauth_token=%s",
                         oAuthKeys.publicKey.c_str());

        if (   (n >= (int)urlBufferSize)
            || (n == -1))
        {
            result = LE_OVERFLOW;
        }
        else
        {
            if ((result = SaveKeys(OAuthTempKeyName, oAuthKeys)) != LE_OK)
            {
                LE_ERROR("Could not save OAuth key set: %s", LE_RESULT_TXT(result));
                result = LE_UNAVAILABLE;
            }
        }
    }
    catch (json::Value_t& error)
    {
        result = LE_COMM_ERROR;
        LE_ERROR("JSON error.");

        //{ "errors" : [ { "code" : 135, "message" : "Timestamp out of bounds." } ] }
    }
    catch (std::runtime_error& error)
    {
        result = LE_COMM_ERROR;
        LE_ERROR("Communication error with server: %s", error.what());
    }

    return result;
}




//--------------------------------------------------------------------------------------------------
/**
 * The PIN code is passed to this function to send back to the server.  This allows the server to
 * confirm that the user does want to authorize this client.  The server then sends back the final
 * authorization keys that the service will keep in secure storage.
 *
 * @return - LE_OK if successful and the service has been successfully authenticated.
 *         = LE_COMM_ERROR if the server can not be contacted.
 *         - LE_UNAVAILABLE if the secure storage service is unavailable.
 */
//--------------------------------------------------------------------------------------------------
le_result_t serviceConfig_TransmitUserPin
(
    const char* newPin  ///< [IN] The unique PIN given to the user by the server.
)
//--------------------------------------------------------------------------------------------------
{
    http::Credentials_t credentials;
    le_result_t result = LoadKeys(ConsumerKeyName, OAuthTempKeyName, credentials);

    if (result != LE_OK)
    {
        LE_ERROR("Could not load credentials");
        return LE_UNAVAILABLE;
    }

    try
    {
        http::KeyPair_t oAuthKeys;
        oAuthKeys = http::AuthenticatePin(credentials,
                                          "https://api.twitter.com/oauth/access_token",
                                          newPin);

        if ((result = SaveKeys(OAuthKeyName, oAuthKeys)) != LE_OK)
        {
            LE_ERROR("Could not save OAuth key set: %s", LE_RESULT_TXT(result));
            result = LE_UNAVAILABLE;
        }

        DeleteKeys(OAuthTempKeyName);
    }
    catch (json::Value_t& error)
    {
        result = LE_COMM_ERROR;
    }
    catch (std::runtime_error& error)
    {
        result = LE_COMM_ERROR;
        LE_ERROR("Communication error with server: %s", error.what());
    }

    return result;
}



//--------------------------------------------------------------------------------------------------
/**
 * Tweet a status update to the world at large.
 *
 * @return - LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t twitter_Tweet
(
    const char* message  ///< [IN] The message to send to the internet.
)
//--------------------------------------------------------------------------------------------------
{
    if (serviceConfig_IsAuthenticated() == false)
    {
        return LE_NOT_PERMITTED;
    }

    if (le_utf8_IsFormatCorrect(message) != true)
    {
        return LE_FORMAT_ERROR;
    }

    if (le_utf8_NumChars(message) > 140)
    {
        return LE_OVERFLOW;
    }

    http::Credentials_t credentials;
    le_result_t result = LoadKeys(ConsumerKeyName, OAuthKeyName, credentials);

    if (result != LE_OK)
    {
        LE_ERROR("Could not load credentials");
        return LE_UNAVAILABLE;
    }

    const std::string url = "https://api.twitter.com/1.1/statuses/update.json";

    try
    {
        std::map<std::string, std::string> params =
                {
                    { "status", message }
                };

        OAuthSignRequest(credentials, http::Request_t::Post, url, params);

        Request(http::Request_t::Post, {}, url, params, "");
    }
    catch (json::Value_t& e)
    {
        result = LE_COMM_ERROR;

        auto& errorList = e["errors"].AsArray();

        for (auto& error : errorList)
        {
            LE_ERROR("Transmit Error<%f>: %s",
                     error["code"].AsNumber(),
                     error["message"].AsString().c_str());
        }
    }
    catch (std::runtime_error& error)
    {
        result = LE_COMM_ERROR;
        LE_ERROR("Communication error on %s: %s", url.c_str(), error.what());
    }

    return LE_OK;
}
