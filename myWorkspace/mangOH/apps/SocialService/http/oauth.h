
//--------------------------------------------------------------------------------------------------
/**
 * @file oauth.h
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#ifndef HTTP_OAUTH_H_INCLUDE_GUARD
#define HTTP_OAUTH_H_INCLUDE_GUARD




namespace http
{




//--------------------------------------------------------------------------------------------------
/**
 * A public and private key set.  Used for authenticating communication using the OAuth standard.
 */
//--------------------------------------------------------------------------------------------------
struct KeyPair_t
{
    std::string publicKey;  ///< The public key to send to the server.
    std::string secretKey;  ///< The private key we hash our message data against.
};




//--------------------------------------------------------------------------------------------------
/**
 * A full credential set.  When completed this represents an authentication of a client application
 * to act on behalf of a user.
 */
//--------------------------------------------------------------------------------------------------
struct Credentials_t
{
    KeyPair_t consumer;     ///< The application's credentials.
    KeyPair_t oAuthAccess;  ///< The application/user authentication.
};




//--------------------------------------------------------------------------------------------------
/**
 * Sign the request parameters so that the server can properly validate the request.  Additional
 * OAuth parameters are also added to this list.
 */
//--------------------------------------------------------------------------------------------------
HTTP_EXPORT void OAuthSignRequest
(
    const Credentials_t& keys,    ///< [IN] The application's keys.
    Request_t type,               ///< [IN] The type of request to be performed.
    const std::string& url,       ///< [IN] The address and path of the request.
    ParamMap_t& params,           ///< [IN/OUT] The parameter list will be updated with the required
                                  ///<          OAuth additions.
    bool excludeConsumer = false  ///< [IN] Should the consumer key be excluded from the params?
);




//--------------------------------------------------------------------------------------------------
/**
 * Using the application keys, we request temporary OAuth keys for further requests.
 *
 * @return A public/secret key set for future use in communicating with the API server.
 */
//--------------------------------------------------------------------------------------------------
HTTP_EXPORT KeyPair_t RequestTempAccessTokens
(
    const KeyPair_t& consumerKey,  ///< [IN] The application's keys.
    const std::string& url         ///< [IN] The URL to contact.
);




//--------------------------------------------------------------------------------------------------
/**
 * Transmit an authentication PIN to the server.
 *
 * @return If successful a new permanent key set is returned.
 */
//--------------------------------------------------------------------------------------------------
HTTP_EXPORT KeyPair_t AuthenticatePin
(
    const Credentials_t& keys,  ///< [IN] The current keys to authenticate with.
    const std::string& url,     ///< [IN] The URL to transmit these keys to.
    const std::string& pin      ///< [IN] The PIN code to send.
);




}




#endif // HTTP_OAUTH_H_INCLUDE_GUARD
