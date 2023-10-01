//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <functional>
#include "sonia/utility/parsers/utility.hpp"
#include "lexems.hpp"

namespace sonia::parsers::tmpl {

template <class LexerT, class ModelT>
class parser
{
    using iterator = typename LexerT::iterator;
    using token_type = typename LexerT::token_type;

public:
    explicit parser(ModelT & mdl) : mdl_{mdl} {}

    void parse(iterator& b, iterator const& e) const;

private:
    //static bool ws(iterator & b, iterator const& e);

    //void skip_ws(iterator & b, iterator const& e) const;

    ModelT & mdl_;
};

}

// Implementation
namespace sonia::parsers::tmpl {

template <class LexerT, class ModelT>
void parser<LexerT, ModelT>::parse(iterator& b, iterator const& e) const
{
    /*
    document = (text | escaping | expr | code)*
    text = [~%]*
    escaping = '%%'
    expr = '%' (text | escaping) '%'
    code = '<%' (text | escaping) '%>'
    */

    mdl_.state = model::state_type::TEXT;
    for (; b != e; ++b) {
        if (b->id == ID_COMMENT || b->id == ID_LINE_COMMENT) {
            BOOST_ASSERT(mdl_.state == model::state_type::TEXT);
            continue;
        }
        if (b->id == ID_ANY) {
            mdl_.push_text(b->first, b->second);
            continue;
        }
        if (b->id == ID_ESCAPED_PROCENT) {
            mdl_.push_text("%"sv);
            continue;
        }
        if (b->id == ID_EXPR_SWITCH) {
            if (mdl_.state == model::state_type::TEXT) {
                mdl_.commit_text();
                mdl_.state = model::state_type::EXPR;
                continue;
            }
            if (mdl_.state == model::state_type::EXPR) {
                mdl_.commit_expr();
                mdl_.state = model::state_type::TEXT;
                continue;
            }
            THROW_INTERNAL_ERROR("unexpected tocken ID_EXPR_SWITCH for the state %1%"_fmt % (int)mdl_.state);
        }
        if (b->id == ID_CODE_SWITCH_START) {
            BOOST_ASSERT(mdl_.state == model::state_type::TEXT);
            mdl_.commit_text();
            mdl_.state = model::state_type::CODE;
            continue;
        }
        if (b->id == ID_CODE_SWITCH_END) {
            BOOST_ASSERT(mdl_.state == model::state_type::CODE);
            mdl_.commit_code();
            mdl_.state = model::state_type::TEXT;
            continue;
        }
        THROW_INTERNAL_ERROR("unexpected tocken %1%"_fmt % b->id);
    }
    
    if (mdl_.state == model::state_type::TEXT) {
        mdl_.commit_text();
    } else if (mdl_.state == model::state_type::CODE) {
        mdl_.commit_code();
    } else {
        mdl_.commit_expr();
    }
}

//template <class LexerT, class ModelT>
//void parser<LexerT, ModelT>::skip_ws(iterator& b, iterator const& e) const
//{
//    star(b, e, &this->ws);
//}
//
//template <class LexerT, class ModelT>
//bool parser<LexerT, ModelT>::ws(iterator& b, iterator const& e)
//{
//    if (b->id != ID_SPACE) return false;
//    ++b;
//    return true;
//}

}
