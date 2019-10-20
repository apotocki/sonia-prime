//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_XML_LEXEMS_HPP
#define SONIA_UTILITY_XML_LEXEMS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

namespace sonia::parsers::xml {

enum lexem {
    ID_SPACE = 1,
    ID_XML_DECL_BEGIN = 2,
    ID_PI = 3,
    ID_XML_DECL_END = 4,
    ID_VERSION_NUM = 5,
    ID_ENCODING = 6,
    ID_ENC_NAME = 7,
    ID_SDECL = 8,

    ID_COMMENT = 11,
    ID_DOCTYPE_BEGIN = 12,

    ID_SYSTEM = 13,
    ID_PUBLIC = 14,
    ID_NAME = 15,
    ID_CHAR_DATA = 16,

    ID_STRING = 17,

    ID_CDATA,
    ID_ETAG,
    ID_OPEN_SQUARE_BRACKET,
    ID_CLOSE_SQUARE_BRACKET,
    ID_OPEN_BROKET,
    ID_CLOSE_BROKET,
    ID_CLOSE_EMPTY_TAG,
    ID_EQ,
    //ID_LINE_COMMENT,    // // up to eol
    //ID_COMMENT,         // /* ... */
    //ID_NULL,            // null
    //ID_TRUE,            // true
    //ID_FALSE,           // false
    //ID_START_ARRAY,     // [
    //ID_STOP_ARRAY,      // ]
    //ID_START_OBJECT,    // {
    //ID_STOP_OBJECT,     // }
    //ID_COMMA,           // ,
    //ID_COLON,           // :
    //ID_STRING,          // string
    //ID_NAME,            // prop name in objects
    //ID_DOUBLE,
    //ID_INTEGER,
    ID_ANY              // any character
};

}

#endif // SONIA_UTILITY_XML_LEXEMS_HPP
