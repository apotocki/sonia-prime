//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_JSON_PARSER_HPP
#define SONIA_UTILITY_JSON_PARSER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/utility/parsers/utility.hpp"

#include "lexems.hpp"

namespace sonia::parsers::json {

template <class LexerT, class ModelT>
class parser
{
    using iterator = typename LexerT::iterator;
    using token_type = typename LexerT::token_type;

public:
    explicit parser(ModelT & mdl) : mdl_{mdl} {}

    void parse(iterator & b, iterator const& e) const;

private:
    static void throw_unexpected_eof();
    static bool ws(iterator & b, iterator const& e);

    void skip_ws(iterator & b, iterator const& e) const;

    ModelT & mdl_;
};

}

// Implementation
namespace sonia::parsers::json {

template <class LexerT, class ModelT>
void parser<LexerT, ModelT>::parse(iterator & b, iterator const& e) const
{
    mdl_.push_state(model::state::VALUE);

    while (mdl_.has_state())
    {
        skip_ws(b, e);
        bool eof = b == e;

        auto st = mdl_.get_state();

        // handle comma
        bool comma_parsed = false;
        if (st == model::state::OBJECT || st == model::state::ARRAY) {
            if (!eof && b->id == ID_COMMA) {
                if (mdl_.value_stack_empty()) {
                    throw exception("unexpected COMMA token");
                }
                ++b;
                skip_ws(b, e);
                comma_parsed = true;
            }
        }

        if (st == model::state::OBJECT) {
            if (!eof && (b->id == ID_STRING || b->id == ID_NAME)) {
                if (!comma_parsed && !mdl_.value_stack_empty()) {
                    throw exception("expected COMMA token");
                }
                if (b->id == ID_STRING) {
                    mdl_.push_string(b->first, b->second);
                } else {
                    mdl_.push_name(b->first, b->second);
                }
                ++b;
                skip_ws(b, e);
                if (b->id != ID_COLON) {
                    throw exception("expected COLON token");
                }
                mdl_.push_state(model::state::VALUE);
                ++b;
                skip_ws(b, e);
            } else if (!eof && b->id == ID_STOP_OBJECT) {
                if (comma_parsed) throw exception("unexpected '}'");
                ++b;
                mdl_.put_object();
                continue;
            } else {
                throw exception("expected STRING or '}'");
            }
        } else if (st == model::state::ARRAY) {
            if (!eof && b->id == ID_STOP_ARRAY) {
                if (comma_parsed) throw exception("unexpected ']'");
                ++b;
                mdl_.put_array();
                continue;
            }
            if (!comma_parsed && !mdl_.value_stack_empty()) {
                throw exception("expected COMMA token");
            }
            mdl_.push_state(model::state::VALUE);
        }

        // value
        if (eof) throw_unexpected_eof();

        bool start_internal = false;
        switch (b->id)
        {
        case ID_NULL:
            mdl_.put_null();
            mdl_.pop_state();
            break;
        case ID_FALSE:
            mdl_.put_boolean(false);
            mdl_.pop_state();
            break;
        case ID_TRUE:
            mdl_.put_boolean(true);
            mdl_.pop_state();
            break;
        case ID_DOUBLE:
            mdl_.put_number(b->first, b->second);
            mdl_.pop_state();
            break;
        case ID_INTEGER:
            mdl_.put_number(b->first, b->second);
            mdl_.pop_state();
            break;
        case ID_STRING:
            mdl_.put_string(b->first, b->second);
            mdl_.pop_state();
            break;
        case ID_START_ARRAY:
            mdl_.push_state(model::state::ARRAY);
            start_internal = true;
            break;
        case ID_START_OBJECT:
            mdl_.push_state(model::state::OBJECT);
            start_internal = true;
            break;
        default:
            throw exception("unexpected token");
        }
        
        ++b;

        /*
        // handle trailing comma
        if (!start_internal && mdl_.has_state()) {
            skip_ws(b, e);
            auto st = mdl_.get_state();
            if (st == model::state::OBJECT) {
                if (b->id == ID_COMMA) {
                    ++b;
                    comma_parsed = true;
                } else if (b->id != ID_STOP_OBJECT) {
                    throw exception("expected COMMA or }");
                }
            } else if (st == model::state::ARRAY) {
                if (b->id == ID_COMMA) {
                    ++b;
                    comma_parsed = true;
                } else if (b->id != ID_STOP_ARRAY) {
                    throw exception("expected COMMA or ]");
                }
            } else {
                throw internal_error("unexpected state");
            }
        }
        */
    };

    star(b, e, &this->ws);
}

template <class LexerT, class ModelT>
bool parser<LexerT, ModelT>::ws(iterator & b, iterator const& e)
{
    auto tokid = b->id;
    if (tokid == ID_SPACE || tokid == ID_LINE_COMMENT || tokid == ID_COMMENT) {
        ++b;
        return true;
    }
    return false;
}

template <class LexerT, class ModelT>
void parser<LexerT, ModelT>::skip_ws(iterator & b, iterator const& e) const
{
    star(b, e, &this->ws);
}

template <class LexerT, class ModelT>
void parser<LexerT, ModelT>::throw_unexpected_eof()
{
    throw exception("unexpected eof");
}

}

#endif // SONIA_UTILITY_JSON_PARSER_HPP
