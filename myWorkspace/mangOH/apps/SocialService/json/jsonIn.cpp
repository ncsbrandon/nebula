
//--------------------------------------------------------------------------------------------------
/**
 * @file jsonIn.cpp  Implementation of json stream input operators.
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#include <istream>
#include "json.h"



// { "items" : [], "count" : 670, "size" : 0, "offset" : 670 }



//--------------------------------------------------------------------------------------------------
/**
 * Skip any whitespace starting at the streams current location.
 *
 * @return A false if the file hits EOF or encounters errors while reading.  A true is returned
 *         otherwise.
 */
//--------------------------------------------------------------------------------------------------
static bool SkipWhitespace
(
    std::istream& in  /// [IN] The stream to read.
)
//--------------------------------------------------------------------------------------------------
{
    char nextChar;

    while (   (nextChar = in.peek())
           && (in.good()))
    {
        if (   (nextChar == ' ')
            || (nextChar == '\t')
            || (nextChar == '\r')
            || (nextChar == '\n'))
        {
            in.get();
        }
        else
        {
            break;
        }
    }

    if (in.bad())
    {
        throw std::runtime_error("Unexpected error in file.");
    }

    return in.good();
}



//--------------------------------------------------------------------------------------------------
/**
 * Attempt to match a literal from the current location in the input stream.
 *
 * @return A true if the input at the current location matches, a false if not.
 */
//--------------------------------------------------------------------------------------------------
static bool LookForLiteral
(
    std::istream& in,                      ///< [IN] The stream to read.
    const json::String_t& expectedLiteral  ///< [IN] The literal text we're looking for.
)
//--------------------------------------------------------------------------------------------------
{
    auto position = in.tellg();

    for (auto nextChar : expectedLiteral)
    {
        char newChar = in.get();

        if (nextChar != newChar)
        {
            in.seekg(position, in.beg);
            return false;
        }
    }

    return true;
}




//--------------------------------------------------------------------------------------------------
/**
 * Try to match a literal string at the current location in the source stream.  If the literal is
 * not matched, an exception is thrown.
 */
//--------------------------------------------------------------------------------------------------
static void ExpectLiteral
(
    std::istream& in,                      ///< [IN] The stream we're reading.
    const json::String_t& expectedLiteral  ///< [IN] The literal we're looking for.
)
//--------------------------------------------------------------------------------------------------
{
    if (LookForLiteral(in, expectedLiteral) == false)
    {
        throw std::runtime_error("Malformed json, expected: '" + expectedLiteral + "'.");
    }
}



//--------------------------------------------------------------------------------------------------
/**
 * Check for a literal in the source stream, and if the literal is present, copy it into the base
 * string.
 *
 * @return A true if the literal was found and copied, false if not.
 */
//--------------------------------------------------------------------------------------------------
static bool ExtractIf
(
    std::istream& in,              ///< [IN] The stream we are reading.
    json::String_t& baseStr,       ///< [IN/OUT] The text to update with the found literal.
    const json::String_t& literal  ///< [IN] The text we're looking for.
)
//--------------------------------------------------------------------------------------------------
{
    bool result = false;

    if (LookForLiteral(in, literal))
    {
        baseStr += literal;
        result = true;
    }

    return result;
}



//--------------------------------------------------------------------------------------------------
/**
 * Check for two literals in the source text, if one of them matches, copy it into the basing
 * string.  If neither matches, no copy if performed.
 *
 * @return A true if one of the literal strings was matched, false otherwise.
 */
//--------------------------------------------------------------------------------------------------
static bool ExtractEither
(
    std::istream& in,                ///< [IN] The stream to read.
    json::String_t& baseStr,         ///< [IN/OUT] The string we're appending to.
    const json::String_t& literalA,  ///< [IN] Attempt to match this literal.
    const json::String_t& literalB   ///< [IN] Or attempt to match this literal.
)
//--------------------------------------------------------------------------------------------------
{
    if (ExtractIf(in, baseStr, literalA))
    {
        return true;
    }

    return ExtractIf(in, baseStr, literalB);
}



//--------------------------------------------------------------------------------------------------
/**
 * Attempt to extract a set of digits from the input stream, stop extraction once a non-digit
 * character is encountered.  All extracted digits are copied into baseStr.
 */
//--------------------------------------------------------------------------------------------------
static void ExtractDigits
(
    std::istream& in,        ///< [IN] The stream we are reading.
    json::String_t& baseStr  ///< [IN/OUT] The string digits are to be written to.
)
//--------------------------------------------------------------------------------------------------
{
    char nextChar;

    while (   (nextChar = in.peek())
           && (in.good())
           && (   (nextChar >= '0')
               && (nextChar <= '9')))
    {
        in.get();
        baseStr += nextChar;
    }
}



//--------------------------------------------------------------------------------------------------
/**
 * Pull a string literal from the source stream.  That is a string delimited by " characters.
 *
 * @return The string extracted from the stream.
 */
//--------------------------------------------------------------------------------------------------
static json::String_t ExtractString
(
    std::istream& in  ///< [IN] The input stream to read from.
)
//--------------------------------------------------------------------------------------------------
{
    json::String_t newString;

    ExpectLiteral(in, "\"");

    char nextChar;

    while (   (nextChar = in.peek())
           && (in.good())
           && (nextChar != '\"'))
    {
        in.get();

        if (nextChar == '\\')
        {
            nextChar = in.get();

            switch (nextChar)
            {
                case 't': nextChar = '\t'; break;
                case 'r': nextChar = '\r'; break;
                case 'n': nextChar = '\n'; break;
            }
        }

        newString += nextChar;
    }

    ExpectLiteral(in, "\"");

    return newString;
}



//--------------------------------------------------------------------------------------------------
/**
 * Read a floating point value from the input stream.
 *
 * @return A json value holding a floating point value.
 */
//--------------------------------------------------------------------------------------------------
static json::Value_t ExtractNumber
(
    std::istream& in  ///< [IN] The stream to read from.
)
//--------------------------------------------------------------------------------------------------
{
    json::String_t number;

    ExtractEither(in, number, "+", "-");
    ExtractDigits(in, number);

    if (ExtractIf(in, number, "."))
    {
        ExtractDigits(in, number);
    }

    if (ExtractEither(in, number, "e", "E"))
    {
        ExtractEither(in, number, "+", "-");
        ExtractDigits(in, number);
    }

    return json::Value_t(std::stod(number));
}



//--------------------------------------------------------------------------------------------------
/**
 * Is the given character the start of a numeric literal?
 *
 * @return A true if the character is the start of a numeric literal, false if not.
 */
//--------------------------------------------------------------------------------------------------
static bool IsNumeric
(
    char theChar  ///< [IN] The character to check.
)
//--------------------------------------------------------------------------------------------------
{
    if (   (theChar == '-')
        || (theChar == '+')
        || ((theChar >= '0') && (theChar <= '9')))
    {
        return true;
    }

    return false;
}



//--------------------------------------------------------------------------------------------------
/**
 * Attempt to stream a json object from the input stream.
 *
 * @return A reference to the input stream for further pipelining.
 */
//--------------------------------------------------------------------------------------------------
std::istream& operator >>
(
    std::istream& in,       ///< [IN] The input stream to read.
    json::Object_t& object  ///< [OUT] The object to stream data into.
)
//--------------------------------------------------------------------------------------------------
{
    object.clear();

    SkipWhitespace(in);

    if (in.eof())
    {
        return in;
    }

    ExpectLiteral(in, "{");

    bool done = false;

    while (   (done == false)
           && (in.good()))
    {
        SkipWhitespace(in);

        json::String_t name = ExtractString(in);

        auto iter = object.find(name);

        if (iter != object.end())
        {
            throw std::runtime_error("Duplicate name, '" + name + "' found in object.");
        }

        SkipWhitespace(in);

        ExpectLiteral(in, ":");

        if (in.good())
        {
            char peekChar = in.peek();

            if (in.good() == false)
            {
                throw std::runtime_error("Unexpected end of file in object.");
            }
            else if (peekChar == '}')
            {
                throw std::runtime_error("Expected value in object.");
            }

            json::Value_t value;
            in >> value;

            object.insert(json::Object_t::value_type(name, value));
        }

        SkipWhitespace(in);
        done = !LookForLiteral(in, ",");
    }

    SkipWhitespace(in);
    ExpectLiteral(in, "}");

    return in;
}



//--------------------------------------------------------------------------------------------------
/**
 * Attempt to stream a json array from the input stream.
 *
 * @return A reference to the input stream for further pipelining.
 */
//--------------------------------------------------------------------------------------------------
std::istream& operator >>
(
    std::istream& in,     ///< [IN] The input stream to read.
    json::Array_t& array  ///< [OUT] Stream data into this array of json values.
)
//--------------------------------------------------------------------------------------------------
{
    array.clear();

    SkipWhitespace(in);

    if (in.eof())
    {
        return in;
    }

    ExpectLiteral(in, "[");

    bool done = false;

    while (   (done == false)
           && (in.good()))
    {
        SkipWhitespace(in);

        if (in.good())
        {
            char peekChar = in.peek();

            if (in.good() == false)
            {
                throw std::runtime_error("Unexpected end of file in array.");
            }
            else if (peekChar == ']')
            {
                throw std::runtime_error("Expected value in array.");
            }

            json::Value_t value;
            in >> value;

            array.push_back(value);
        }

        SkipWhitespace(in);
        done = !LookForLiteral(in, ",");
    }

    SkipWhitespace(in);
    ExpectLiteral(in, "]");

    return in;
}



//--------------------------------------------------------------------------------------------------
/**
 * Attempt to stream a json array from the input stream.
 *
 * @return A reference to the input stream for further pipelining.
 */
//--------------------------------------------------------------------------------------------------
std::istream& operator >>
(
    std::istream& in,     ///< [IN] The input stream to read.
    json::Value_t& value  ///< [OUT] The json value to populate from the input stream.
)
//--------------------------------------------------------------------------------------------------
{
    if (SkipWhitespace(in) == false)
    {
        value = json::Value_t();
        return in;
    }

    char nextChar = in.peek();

    switch (nextChar)
    {
        case '{':
            {
                json::Object_t object;
                in >> object;

                value = std::move(object);
            }
            break;

        case '[':
            {
                json::Array_t array;
                in >> array;

                value = std::move(array);
            }
            break;

        case '\"':
            value = ExtractString(in);
            break;

        case 'f':
            ExpectLiteral(in, "false");
            value = false;
            break;

        case 't':
            ExpectLiteral(in, "true");
            value = true;
            break;

        case 'n':
            ExpectLiteral(in, "null");
            value = json::Value_t();
            break;

        default:
            if (IsNumeric(nextChar))
            {
                value = ExtractNumber(in);
            }
            else
            {
                char charStr[2] = { nextChar };
                throw std::runtime_error(json::String_t("Unexpected character in json, '") +
                                         charStr + "'.");
            }
            break;
    }

    return in;
}
