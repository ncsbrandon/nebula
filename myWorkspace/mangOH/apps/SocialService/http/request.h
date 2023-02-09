
//--------------------------------------------------------------------------------------------------
/**
 * @file request.h
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#ifndef HTTP_REQUEST_H_INCLUDE_GUARD
#define HTTP_REQUEST_H_INCLUDE_GUARD



namespace http
{




//--------------------------------------------------------------------------------------------------
/**
 * Send a HTTP request to a remote server.
 *
 * @return The servers response is returned as a string.
 */
//--------------------------------------------------------------------------------------------------
enum class Request_t
{
    Get,
    Post
};




using ParamMap_t = std::map<std::string, std::string>;




//--------------------------------------------------------------------------------------------------
/**
 * Encode a string so that it is usable in a HTTP request.
 *
 * @return A string, safe for transport.
 */
//--------------------------------------------------------------------------------------------------
HTTP_EXPORT std::string UrlEncode
(
    const std::string& original  ///< [IN] The original string to filter.
);




//--------------------------------------------------------------------------------------------------
/**
 * Send a HTTP request to a remote server.
 *
 * @return The servers response is returned as a string.
 */
//--------------------------------------------------------------------------------------------------
HTTP_EXPORT std::string Request
(
    Request_t type,                             ///< [IN] The type of request we're sending.
    const std::list<std::string> headerParams,  ///< [IN] Additional parameters for the header.
    const std::string& url,                     ///< [IN] The URL to post the request to.
    const ParamMap_t& params,                   ///< [IN] Parameters in the URL.
    const std::string& messageBody = ""         ///< [IN] The body text of the message.
);



}




#endif // HTTP_REQUEST_H_INCLUDE_GUARD
