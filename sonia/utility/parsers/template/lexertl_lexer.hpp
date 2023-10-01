//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/utility/parsers/lexer.hpp"

#include "lexems.hpp"

namespace sonia::parsers::tmpl {

template <typename DerivedT, typename IteratorT>
class lexertl_lexer_base : public lexer_base<DerivedT, IteratorT>
{
protected:
    using lexertl_lexer_base_t = lexertl_lexer_base;

public:
    template <typename RulesT>
    static void build_rules(RulesT & rules)
    {
        rules.push_state("IN_EXPR");
        rules.push_state("IN_CODE");
        // no comments, interfer with context
        //rules.push("INITIAL", "\\/\\/[^\r\n]*((\r\n)|(\n))", ID_LINE_COMMENT, ".");
        //rules.push("INITIAL", "\\/\\*([^*]|\\*+[^/*])*\\*+\\/", ID_COMMENT, ".");
        rules.push("INITIAL,IN_EXPR,IN_CODE", "%%", ID_ESCAPED_PROCENT, ".");
        rules.push("INITIAL", "%", ID_EXPR_SWITCH, "IN_EXPR");
        rules.push("INITIAL", "<%", ID_CODE_SWITCH_START, "IN_CODE");

        rules.push("IN_EXPR", "%", ID_EXPR_SWITCH, "INITIAL");
        rules.push("IN_CODE", "%>", ID_CODE_SWITCH_END, "INITIAL");
        
        rules.push("INITIAL,IN_CODE,IN_EXPR", ".|[\r\n]", ID_ANY, ".");
    }
};

template <typename IteratorT>
class lexertl_lexer : public lexertl_lexer_base<lexertl_lexer<IteratorT>, IteratorT>
{};

}
