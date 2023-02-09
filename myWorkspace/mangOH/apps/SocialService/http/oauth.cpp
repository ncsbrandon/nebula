
//--------------------------------------------------------------------------------------------------
/**
 * @file oauth.cpp
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#include <sstream>
#include <ctime>
#include <random>
#include "legato.h"
#include "interfaces.h"
#include "json.h"
#include "http.h"
#include "base64.h"
#include "SHA1.h"
#include "HMAC_SHA1.h"



namespace http
{




//--------------------------------------------------------------------------------------------------
/**
 * .
 */
//--------------------------------------------------------------------------------------------------
static std::string GenerateTimestamp
(
)
//--------------------------------------------------------------------------------------------------
{
    std::time_t result = std::time(nullptr);
    std::stringstream buffer;

    buffer << result;

    return buffer.str();
}




//--------------------------------------------------------------------------------------------------
/**
 * .
 */
//--------------------------------------------------------------------------------------------------
static std::string GenerateNonce
(
    const std::string& timestamp  ///< [IN]
)
//--------------------------------------------------------------------------------------------------
{
    static const char alphNum[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    std::string result = timestamp;

    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_int_distribution<> dis(0, sizeof(alphNum) - 1);

    for (int i = 0; i < 8; i++)
    {
        result.append(1, alphNum[dis(gen)]);
    }

    return result;
}




//--------------------------------------------------------------------------------------------------
/**
 * Sign the request parameters so that the server can properly validate the request.  Additional
 * OAuth parameters are also added to this list.
 */
//--------------------------------------------------------------------------------------------------
void OAuthSignRequest
(
    const Credentials_t& keys,  ///< [IN] The application's keys.
    Request_t type,             ///< [IN] The type of request to be performed.
    const std::string& url,     ///< [IN] The address and path of the request.
    ParamMap_t& params,         ///< [IN/OUT] The parameter list will be updated with the required
                                ///<          OAuth additions.
    bool excludeConsumer        ///< [IN] Should the consumer key be excluded from the params?
)
//--------------------------------------------------------------------------------------------------
{
    std::string timeStamp = GenerateTimestamp();

    params.insert({ "oauth_nonce", GenerateNonce(timeStamp) });
    params.insert({ "oauth_signature_method", "HMAC-SHA1" });
    params.insert({ "oauth_timestamp", timeStamp });
    params.insert({ "oauth_version", "1.0" });

    if (excludeConsumer == false)
    {
        params.insert({ "oauth_consumer_key", keys.consumer.publicKey });
    }

    if (keys.oAuthAccess.publicKey.size() > 0)
    {
        params.insert({ "oauth_token", keys.oAuthAccess.publicKey });
    }

    std::stringstream requestString;
    std::stringstream paramString;

    switch (type)
    {
        case Request_t::Get:
            requestString << "GET&";
            break;

        case Request_t::Post:
            requestString << "POST&";
            break;

        default:
            throw std::runtime_error("Unexpected HTTP request type.");
    }

    requestString << UrlEncode(url) << "&";

    bool first = true;
    for (auto iter : params)
    {
        if (first) { first = false; } else { paramString << "&"; }
        paramString << UrlEncode(iter.first) << "=" << UrlEncode(iter.second);
    }

    requestString << UrlEncode(paramString.str());

    for (size_t i = 0; i < requestString.str().size(); i += 20)
    {
        std::string section = requestString.str().substr(i, 20);
    }

    CHMAC_SHA1 sha1;

    std::string signingKey = UrlEncode(keys.consumer.secretKey) + "&";

    if (keys.oAuthAccess.secretKey.size() > 0)
    {
        signingKey += UrlEncode(keys.oAuthAccess.secretKey);
    }

    BYTE digest[CHMAC_SHA1::SHA1_DIGEST_LENGTH] = "";

    sha1.HMAC_SHA1((unsigned char*)requestString.str().c_str(),
                   requestString.str().size(),
                   (unsigned char*)signingKey.c_str(),
                   signingKey.size(),
                   digest);

    params.insert({ "oauth_signature", base64_encode(digest, sizeof(digest)) });
}




//--------------------------------------------------------------------------------------------------
/**
 * Using the application keys, we request temporary OAuth keys for further requests.
 *
 * @return A public/secret key set for future use in communicating with the API server.
 */
//--------------------------------------------------------------------------------------------------
KeyPair_t RequestTempAccessTokens
(
    const KeyPair_t& consumerKey,  ///< [IN] The application's keys.
    const std::string& url         ///< [IN] The URL to contact.
)
//--------------------------------------------------------------------------------------------------
{
    ParamMap_t params =
        {
            { "oauth_callback", "oob" }
        };

    OAuthSignRequest({ consumerKey }, Request_t::Post, url, params);

    std::string response = Request(Request_t::Post, {}, url, params, "");
    json::Value_t value;

    if (response[0] == '{')
    {
        std::stringstream responsStream(response);
        responsStream >> value;

        throw value;
    }

    value = ParseKeyValueString(response);
    return { value["oauth_token"].AsString(), value["oauth_token_secret"].AsString() };
}




//--------------------------------------------------------------------------------------------------
/**
 * Transmit an authentication PIN to the server.
 *
 * @return If successful a new permanent key set is returned.
 */
//--------------------------------------------------------------------------------------------------
KeyPair_t AuthenticatePin
(
    const Credentials_t& keys,  ///< [IN] The current keys to authenticate with.
    const std::string& url,     ///< [IN] The URL to transmit these keys to.
    const std::string& pin      ///< [IN] The PIN code to send.
)
//--------------------------------------------------------------------------------------------------
{
    std::map<std::string, std::string> params =
        {
            { "oauth_verifier", pin }
        };

    OAuthSignRequest(keys, Request_t::Post, url, params, true);

    std::string response = Request(Request_t::Post, {}, url, params, "");
    json::Value_t value;

    if (response[0] == '{')
    {
        std::stringstream responsStream(response);
        responsStream >> value;

        throw value;
    }

    value = ParseKeyValueString(response);
    return { value["oauth_token"].AsString(), value["oauth_token_secret"].AsString() };
}




}
