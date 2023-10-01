//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/parsers/lexer.hpp"

#include "lexems.hpp"

namespace sonia::parsers::json {

template <typename DerivedT, typename IteratorT>
class lexertl_lexer_base : public lexer_base<DerivedT, IteratorT>
{
protected:
    using lexertl_lexer_base_t = lexertl_lexer_base;

public:
    template <typename RulesT>
    static void build_string_rules(RulesT & rules)
    {
        rules.push("\\x22([\\x20\\x21\\x23-\\x5b\\x5d-\\xff]|\\x5c[bfnrt\\x22\\x27\\x5c\\x2f]|\\x5cu(\\d|[a-f]|[A-F]){4})*\\x22", ID_STRING);
        rules.push("\\x27([\\x20-\\x26\\x28-\\x5b\\x5d-\\xff]|\\x5c[bfnrt\\x22\\x27\\x5c\\x2f]|\\x5cu(\\d|[a-f]|[A-F]){4})*\\x27", ID_STRING);
    }

    template <typename RulesT>
    static void build_numeric_rules(RulesT & rules)
    {
        rules.push("-?(0|[1-9]\\d*)(([.]\\d+([eE][-+]?\\d+)?)|([eE][-+]?\\d+))", ID_DOUBLE);
        rules.push("-?(0|[1-9]\\d*)", ID_INTEGER);
    }

    template <typename RulesT>
    static void build_rules(RulesT & rules)
    {
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
        DerivedT::build_string_rules(rules);
        DerivedT::build_numeric_rules(rules);
        rules.push(".", ID_ANY);
    }
};

template <typename IteratorT>
class lexertl_lexer : public lexertl_lexer_base<lexertl_lexer<IteratorT>, IteratorT>
{};

template <typename IteratorT>
class light_lexertl_lexer : public lexertl_lexer_base<light_lexertl_lexer<IteratorT>, IteratorT>
{
public:
    template <typename RulesT>
    static void build_string_rules(RulesT & rules)
    {
        light_lexertl_lexer::lexertl_lexer_base_t::build_string_rules(rules);
        rules.push("[A-Z_a-z\\x23-\\x25\\x2e\\x7f-\\xff]([0-9A-Z_a-z\\x23-\\x25\\x2d\\x2e\\x7f-\\xff]|\\x5c[bfnrt\\x22\\x27\\x3b\\x5c\\x2f]|\\x5cu(\\d|[a-f]|[A-F]){4})*", ID_NAME);
    }
};

}
