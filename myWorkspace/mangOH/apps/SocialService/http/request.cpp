
//--------------------------------------------------------------------------------------------------
/**
 * @file request.cpp
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#include <sstream>
#include <memory>
#include <set>
#include "legato.h"
#include "interfaces.h"
#include "curl/curl.h"
#include "json.h"
#include "http.h"




namespace http
{




/// Smart pointer that allows for a user deletion function.
using CurlPtr_t = std::unique_ptr<CURL, void (*)(CURL*)>;



//--------------------------------------------------------------------------------------------------
/**
 * Construct a new CURL object smart pointer.
 *
 * @return A properly initialized smart pointer if successful, or one that holds a value of nullptr
 &         if not.
 */
//--------------------------------------------------------------------------------------------------
static CurlPtr_t MakeCurlPtr
(
)
//--------------------------------------------------------------------------------------------------
{
    return CurlPtr_t(curl_easy_init(), [](CURL* curlPtr)
        {
            if (curlPtr != nullptr)
            {
                curl_easy_cleanup(curlPtr);
            }
        });
}




//--------------------------------------------------------------------------------------------------
/**
 * Encode a string so that it is usable in a HTTP request.
 *
 * @return A string, safe for transport.
 */
//--------------------------------------------------------------------------------------------------
std::string UrlEncode
(
    const std::string& original  ///< [IN] The original string to filter.
)
//--------------------------------------------------------------------------------------------------
{
    static const std::set<unsigned char> encodeMap =
        {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
            'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',

            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
            'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',

            '-', '.', '_', '~'
        };

    std::string result;

    for (auto next : original)
    {
        auto iter = encodeMap.find(next);

        if (iter != encodeMap.end())
        {
            result.append(1, next);
        }
        else
        {
            char buffer[10];
            snprintf(buffer, sizeof(buffer), "%%%02X", (int)next);
            result.append(buffer);
        }
    }

    return result;
}




//--------------------------------------------------------------------------------------------------
/**
 * Send a HTTP request to a remote server.
 *
 * @return The servers response is returned as a string.
 */
//--------------------------------------------------------------------------------------------------
static std::string GenerateFullUrl
(
    const std::string& baseUrl,  /// < [IN] The URL we're appending parameters to.
    const ParamMap_t& params     /// < [IN] The parameters to append.
)
//--------------------------------------------------------------------------------------------------
{
    if (params.empty())
    {
        return baseUrl;
    }

    std::stringstream newUrl;

    newUrl << baseUrl << "?";

    bool first = true;
    for (auto param : params)
    {
        if (first == false)
        {
            newUrl << "&";
        }
        else
        {
            first = false;
        }


        newUrl << UrlEncode(param.first) << "=" << UrlEncode(param.second);
    }

    return newUrl.str();
}




//--------------------------------------------------------------------------------------------------
/**
 * Send a HTTP request to a remote server.
 *
 * @return The servers response is returned as a string.
 */
//--------------------------------------------------------------------------------------------------
std::string Request
(
    Request_t type,                             ///< [IN] The type of request we're sending.
    const std::list<std::string> headerParams,  ///< [IN] Additional parameters for the header.
    const std::string& url,                     ///< [IN] The URL to post the request to.
    const ParamMap_t& params,                   ///< [IN] Parameters in the URL.
    const std::string& messageBody              ///< [IN] The body text of the message.
)
//--------------------------------------------------------------------------------------------------
{
    std::string fullUrl = GenerateFullUrl(url, params);

    auto bufferPtr = std::unique_ptr<std::string>(new std::string());
    auto curlPtr = MakeCurlPtr();

    if (curlPtr == nullptr)
    {
        throw std::runtime_error("Could not init curl library.");
    }

    struct curl_slist* chunkPtr = nullptr;
    for (auto header : headerParams)
    {
        std::string encodedParam = UrlEncode(header);
        chunkPtr = curl_slist_append(chunkPtr, encodedParam.c_str());
    }

    curl_easy_setopt(curlPtr.get(), CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curlPtr.get(), CURLOPT_USERAGENT, "legato-agent/1.0");
    curl_easy_setopt(curlPtr.get(), CURLOPT_URL, fullUrl.c_str());
    curl_easy_setopt(curlPtr.get(), CURLOPT_WRITEDATA, bufferPtr.get());

    if (type == Request_t::Post)
    {
        curl_easy_setopt(curlPtr.get(), CURLOPT_POST, 1);

        if (messageBody.size() > 0)
        {
            curl_easy_setopt(curlPtr.get(), CURLOPT_POSTFIELDSIZE, messageBody.size());
            curl_easy_setopt(curlPtr.get(), CURLOPT_POSTFIELDS, messageBody.c_str());
        }
    }

    curl_write_callback callbackPtr =
        [](char* contentPtr, size_t size, size_t numElements, void* userPtr) -> size_t
        {
            size_t realSize = size * numElements;

            try
            {
                auto bufferPtr = reinterpret_cast<std::string*>(userPtr);
                bufferPtr->append(static_cast<const char*>(contentPtr), realSize);
            }
            catch (...)
            {
                realSize = 0;
            }

            return realSize;
        };

    curl_easy_setopt(curlPtr.get(), CURLOPT_WRITEFUNCTION, callbackPtr);

    if (chunkPtr != nullptr)
    {
        curl_easy_setopt(curlPtr.get(), CURLOPT_HTTPHEADER, chunkPtr);
    }

    CURLcode res = curl_easy_perform(curlPtr.get());

    if (headerParams.size() > 0)
    {
        curl_slist_free_all(chunkPtr);
    }

    if (res != CURLE_OK)
    {
        throw std::runtime_error(std::string("Curl request failed: ") + curl_easy_strerror(res));
    }

    return std::move(*bufferPtr.get());
}



}



COMPONENT_INIT
{
    curl_global_init(CURL_GLOBAL_ALL);
}
