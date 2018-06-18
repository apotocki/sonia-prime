//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_JSON_LEXER_HPP
#define SONIA_UTILITY_JSON_LEXER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/utility/parsers/lexer.hpp"

#include "lexems.hpp"

namespace sonia { namespace parsers { namespace json {

template <typename IteratorT>
class lexertl_lexer : public lexer_base<lexertl_lexer<IteratorT>, IteratorT>
{
public:
    template <typename RulesT>
    static void build_rules(RulesT & rules) {
        rules.push("\\s+", ID_SPACE);
        rules.push("\\/\\/[^\r\n]*", ID_LINE_COMMENT);
        rules.push("\\/\\*([^*]|\\*+[^/*])*\\*+\\/", ID_COMMENT);
        //"\\/\\*.{+}[\r\n]*?\\*\\/"
        rules.push("null", ID_NULL);
        rules.push("true", ID_TRUE);
        rules.push("false", ID_FALSE);
        rules.push("\\[", ID_START_ARRAY);
        rules.push("\\]", ID_STOP_ARRAY);
        rules.push("\\{", ID_START_OBJECT);
        rules.push("\\}", ID_STOP_OBJECT);
        rules.push(",", ID_COMMA);
        rules.push(":", ID_COLON);
        rules.push("\\x22([\\x20\\x21\\x23-\\x5b\\x5d-\\xff]|\\x5c[bfnrt\\x22\\x27\\x5c\\x2f]|\\x5cu(\\d|[a-f]|[A-F]){4})*\\x22", ID_STRING);
        rules.push("\\x27([\\x20-\\x26\\x28-\\x5b\\x5d-\\xff]|\\x5c[bfnrt\\x22\\x27\\x5c\\x2f]|\\x5cu(\\d|[a-f]|[A-F]){4})*\\x27", ID_STRING);
        rules.push("-?(0|[1-9]\\d*)(([.]\\d+([eE][-+]?\\d+)?)|([eE][-+]?\\d+))", ID_DOUBLE);
        rules.push("-?(0|[1-9]\\d*)", ID_INTEGER);
        rules.push(".", ID_ANY);
    }
};

}}}

#endif // SONIA_UTILITY_JSON_LEXER_HPP
