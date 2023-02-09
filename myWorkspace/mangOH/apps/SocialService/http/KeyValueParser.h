
//--------------------------------------------------------------------------------------------------
/**
 * @file KeyValueParser.h
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#ifndef HTTP_KEY_VALUE_PARSER_H_INCLUDE_GUARD
#define HTTP_KEY_VALUE_PARSER_H_INCLUDE_GUARD




namespace http
{




using KeyValueMap_t = std::map<std::string, std::string>;




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
);




}




#endif // HTTP_KEY_VALUE_PARSER_H_INCLUDE_GUARD
