
//--------------------------------------------------------------------------------------------------
/**
 * @file KeyValueParser.cpp
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#include "legato.h"
#include "interfaces.h"
#include "json.h"
#include "http.h"




namespace http
{




//--------------------------------------------------------------------------------------------------
/**
 * Parse a key/value string of the format:
 *
 *   <key>=<value>&<key>=<value>&<key>=<value>...
 *
 * Into a JSON object.
 *
 * @return A JSON version of the key/value string.
 */
//--------------------------------------------------------------------------------------------------
json::Value_t ParseKeyValueString
(
    const std::string& keyString  ///< [IN] The key value string to parse.
)
//--------------------------------------------------------------------------------------------------
{
    json::Object_t object;
    size_t pos = 0;

    do
    {
        size_t endPos = keyString.find_first_of('=', pos);

        if (pos != std::string::npos)
        {
            std::string key = keyString.substr(pos, endPos - pos);

            pos = endPos + 1;

            endPos = keyString.find_first_of('&', pos);

            std::string value = keyString.substr(pos, endPos - pos);

            pos = endPos;

            if (pos != std::string::npos)
            {
                pos += 1;
            }

            object.insert({ key, value });
        }
    }
    while (pos != std::string::npos);

    return object;
}




}
