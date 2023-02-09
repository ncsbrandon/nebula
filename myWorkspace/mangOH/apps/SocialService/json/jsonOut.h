
//--------------------------------------------------------------------------------------------------
/**
 * @file jsonOut.h
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#ifndef LEGATO_AVTOOLS_JSON_OUT_H_INCLUDE_GUARD
#define LEGATO_AVTOOLS_JSON_OUT_H_INCLUDE_GUARD



//--------------------------------------------------------------------------------------------------
/**
 * Write a json object as properly formated json text to the given output stream.
 *
 * @return A reference to the input stream for further pipelining.
 */
//--------------------------------------------------------------------------------------------------
JSON_EXPORT std::ostream& operator <<
(
    std::ostream& out,            ///< [IN] The output stream to write to.
    const json::Object_t& object  ///< [IN] The object to write as json.
);



//--------------------------------------------------------------------------------------------------
/**
 * Write a json array as properly formated json text to the given output stream.
 *
 * @return A reference to the input stream for further pipelining.
 */
//--------------------------------------------------------------------------------------------------
JSON_EXPORT std::ostream& operator <<
(
    std::ostream& out,          ///< [IN] The output stream to write to.
    const json::Array_t& array  ///< [IN] The array to write as json.
);



//--------------------------------------------------------------------------------------------------
/**
 * Write a json value as properly formated json text to the given output stream.
 *
 * @return A reference to the input stream for further pipelining.
 */
//--------------------------------------------------------------------------------------------------
JSON_EXPORT std::ostream& operator <<
(
    std::ostream& out,          ///< [IN] The output stream to write to.
    const json::Value_t& value  ///< [IN] The value to write as json.
);



#endif // LEGATO_AVTOOLS_JSON_OUT_H_INCLUDE_GUARD
