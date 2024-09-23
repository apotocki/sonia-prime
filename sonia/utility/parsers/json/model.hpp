//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>
#include <cstdlib>

#include "sonia/exceptions.hpp"
#include "sonia/utility/json/value.hpp"
#include "sonia/utility/parsers/utility.hpp"

namespace sonia::parsers::json {

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
    void put_number(IteratorT b, IteratorT e)
    {
        tempstr_.clear();
        std::copy(b, e, std::back_inserter(tempstr_));
        stack_.push_back(json_value(mp::decimal{string_view(tempstr_.data(), tempstr_.size())}));
    }

    template <typename IteratorT>
    void put_string(IteratorT b, IteratorT e)
    {
        ++b;
        tempstr_.clear();
        normilize_json_string(b, e, std::back_inserter(tempstr_));
        tempstr_.pop_back();
        stack_.push_back(json_value(string_view(tempstr_.data(), tempstr_.size())));
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
        return res;
    }

private:
    std::vector<json_value> stack_;
    std::vector<std::pair<state, size_t>> state_stack_; // state->stack_position
    std::vector<std::string> strings_;
    std::vector<char> tempstr_;
};

}
