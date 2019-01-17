//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_JSON_MODEL_HPP
#define SONIA_UTILITY_JSON_MODEL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <vector>
#include <cstdlib>

#include "sonia/exceptions.hpp"
#include "sonia/utility/json/value.hpp"
#include "sonia/utility/parsers/utility.hpp"

namespace sonia { namespace parsers { namespace json {

template <typename InputIteratorT, typename OutputIteratorT>
void normilize_json_string_aux(InputIteratorT & it, InputIteratorT const& eit, OutputIteratorT & oit)
{
    ++it;
    if (it == eit) throw exception("unexpected end of string");
    char c = *it;
    ++it;
    switch (c) {
    case '"':  *oit = '"'; break;
    case '\'':  *oit = '\''; break;
    case '\\': *oit = '\\'; break;
    case '/': *oit = '/'; break;
    case 'b':  *oit = '\b'; break;
    case 'f':  *oit = '\f'; break;
    case 'n':  *oit = '\n'; break;
    case 'r':  *oit = '\r'; break;
    case 't':  *oit = '\t'; break;
    case 'u': {
        uint32_t utf32char;
        if (!hexdigit(it, eit, 4, utf32char)) throw exception("found wrong \\uXXXX character");
        to_utf8(utf32char, oit);
        break;
    }
    default:
        throw exception("unexpected string character");
    }
    ++oit;
}

template <typename InputIteratorT, typename OutputIteratorT>
void normilize_json_string(InputIteratorT it, InputIteratorT const& eit, OutputIteratorT oit)
{
    while (it != eit) {
        char c = *it;
        if (c != '\\') {
            *oit = c;
            ++it; ++oit;
        } else {
            normilize_json_string_aux(it, eit, oit);
        }
    }
}

class model
{
    model(model const&) = delete;
    model& operator= (model const&) = delete;

public:
    enum class state
    {
        VALUE = 1,
        ARRAY,
        OBJECT
    };

    model();
    ~model();

    void put_null();
    void put_boolean(bool);
    void put_object();
    void put_array();
    
    template <typename IteratorT>
    void put_number(IteratorT b, IteratorT e) {
        tempstr_.clear();
        std::copy(b, e, std::back_inserter(tempstr_));
        stack_.push_back(json_value(decimal::parse(string_view(&tempstr_.front(), tempstr_.size()))));
    }

    template <typename IteratorT>
    void put_string(IteratorT b, IteratorT e) {
        ++b;
        tempstr_.clear();
        normilize_json_string(b, e, std::back_inserter(tempstr_));
        tempstr_.pop_back();
        stack_.push_back(json_value(string_view(tempstr_.empty() ? nullptr : &tempstr_[0], tempstr_.size())));
    }

    bool has_state() const { return !state_stack_.empty(); }
    state get_state() const { return state_stack_.back().first; }
    void push_state(state s);
    void pop_state() { state_stack_.pop_back(); }

    template <typename IteratorT>
    void push_string(IteratorT b, IteratorT e)
    {
        ++b;
        std::string str;
        normilize_json_string(b, e, std::back_inserter(str));
        str.pop_back();
        strings_.push_back(std::move(str));
    }

    template <typename IteratorT>
    void push_name(IteratorT b, IteratorT e)
    {
        std::string str;
        normilize_json_string(b, e, std::back_inserter(str));
        strings_.push_back(std::move(str));
    }

    bool value_stack_empty() const
    {
        return state_stack_.back().second == stack_.size();
    }

    json_value detach_result()
    {
        json_value res = std::move(stack_.back());
        stack_.pop_back();
        return std::move(res);
    }

private:
    std::vector<json_value> stack_;
    std::vector<std::pair<state, size_t>> state_stack_; // state->stack_position
    std::vector<std::string> strings_;
    std::vector<char> tempstr_;
};

}}}

#endif // SONIA_UTILITY_JSON_MODEL_HPP
