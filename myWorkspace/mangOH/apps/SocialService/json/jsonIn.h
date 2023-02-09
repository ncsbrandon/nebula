
//--------------------------------------------------------------------------------------------------
/**
 * @file jsonIn.h
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#ifndef LEGATO_AVTOOLS_JSON_IN_H_INCLUDE_GUARD
#define LEGATO_AVTOOLS_JSON_IN_H_INCLUDE_GUARD



//--------------------------------------------------------------------------------------------------
/**
 * Attempt to stream a json object from the input stream.
 *
 * @return A reference to the input stream for further pipelining.
 */
//--------------------------------------------------------------------------------------------------
JSON_EXPORT std::istream& operator >>
(
    std::istream& in,       ///< [IN] The input stream to read.
    json::Object_t& object  ///< [OUT] The object to stream data into.
);



//--------------------------------------------------------------------------------------------------
/**
 * Attempt to stream a json array from the input stream.
 *
 * @return A reference to the input stream for further pipelining.
 */
//--------------------------------------------------------------------------------------------------
JSON_EXPORT std::istream& operator >>
(
    std::istream& in,     ///< [IN] The input stream to read.
    json::Array_t& array  ///< [OUT] Stream data into this array of json values.
);



//--------------------------------------------------------------------------------------------------
/**
 * Attempt to stream a json array from the input stream.
 *
 * @return A reference to the input stream for further pipelining.
 */
//--------------------------------------------------------------------------------------------------
JSON_EXPORT std::istream& operator >>
(
    std::istream& in,     ///< [IN] The input stream to read.
    json::Value_t& value  ///< [OUT] The json value to populate from the input stream.
);



#endif // LEGATO_AVTOOLS_JSON_IN_H_INCLUDE_GUARD
