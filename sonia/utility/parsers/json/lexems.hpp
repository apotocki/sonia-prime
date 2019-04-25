//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_JSON_LEXEMS_HPP
#define SONIA_UTILITY_JSON_LEXEMS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

namespace sonia::parsers::json {

enum lexem {
    ID_SPACE = 1,
    ID_LINE_COMMENT,    // // up to eol
    ID_COMMENT,         // /* ... */
    ID_NULL,            // null
    ID_TRUE,            // true
    ID_FALSE,           // false
    ID_START_ARRAY,     // [
    ID_STOP_ARRAY,      // ]
    ID_START_OBJECT,    // {
    ID_STOP_OBJECT,     // }
    ID_COMMA,           // ,
    ID_COLON,           // :
    ID_STRING,          // string
    ID_NAME,            // prop name in objects
    ID_DOUBLE,
    ID_INTEGER,
    ID_ANY              // any character
};

}

#endif // SONIA_UTILITY_JSON_LEXEMS_HPP
