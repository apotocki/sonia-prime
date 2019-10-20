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
#include "sonia/utility/parsers/utility.hpp"

namespace sonia::parsers::xml {

template <typename DerivedT>
class model_base
{
    model_base(model_base const&) = delete;
    model_base& operator= (model_base const&) = delete;

    DerivedT const& derived() const { return static_cast<DerivedT const&>(*this); }
    DerivedT & derived() { return static_cast<DerivedT&>(*this); }

public:
    model_base() = default;
    //~model_base(){}

    // XML Decl
    void on_encoding() {}
    void on_version() {}
    void on_standalone()
    {
        string_view sv = to_array_view(tempstr_);
        if (sv == "yes") derived().on_standalone(true);
        else if (sv == "no") derived().on_standalone(false);
        else throw exception("wrong standalone value %1%"_fmt % sv);
    }
    void on_standalone(bool){}

    template <typename IteratorT> void on_pi(IteratorT b, IteratorT e)
    {
        put_string(b, e);
    }
    void on_start_doctype() {}
    void on_system_literal() {}
    void on_pubid_literal() {}

    void on_begin_element() { element_name_ = std::move(tempstr_); }
    void on_end_element() { } // on '>'
    void on_close_element() { } // on '/>'
    void on_close_tag() { } // on '</' name
    void on_attribute_name() { attr_name_ = std::move(tempstr_); }
    void on_attribute_value() {}
    void on_char_data() {}

//    void put_null();
//    void put_boolean(bool);
//    void put_object();
//    void put_array();
//    
//    template <typename IteratorT>
//    void put_number(IteratorT b, IteratorT e) {
//        tempstr_.clear();
//        std::copy(b, e, std::back_inserter(tempstr_));
//        stack_.push_back(json_value(decimal::parse(string_view(&tempstr_.front(), tempstr_.size()))));
//    }
//
    template <typename IteratorT>
    void put_string(IteratorT b, IteratorT e, uint8_t qsz = 1)
    {
        for (uint8_t v = qsz; v > 0; ++b, --v);
        tempstr_.clear();
        std::copy(b, e, std::back_inserter(tempstr_));
        //normilize_xml_string(b, e, std::back_inserter(tempstr_));
        BOOST_ASSERT(qsz <= tempstr_.size());
        tempstr_.resize(tempstr_.size() - qsz);
    }

    template <typename IteratorT>
    void put_cdata_string(IteratorT b, IteratorT e)
    {
        for (uint8_t v = 9; v > 0; ++b, --v);
        tempstr_.clear();
        std::copy(b, e, std::back_inserter(tempstr_));
        tempstr_.resize(tempstr_.size() - 3);
    }
//
//    bool has_state() const { return !state_stack_.empty(); }
//    state get_state() const { return state_stack_.back().first; }
//    void push_state(state s);
//    void pop_state() { state_stack_.pop_back(); }
//
//    template <typename IteratorT>
//    void push_string(IteratorT b, IteratorT e)
//    {
//        ++b;
//        std::string str;
//        normilize_json_string(b, e, std::back_inserter(str));
//        str.pop_back();
//        strings_.push_back(std::move(str));
//    }
//
//    template <typename IteratorT>
//    void push_name(IteratorT b, IteratorT e)
//    {
//        std::string str;
//        normilize_json_string(b, e, std::back_inserter(str));
//        strings_.push_back(std::move(str));
//    }
//
//    bool value_stack_empty() const
//    {
//        return state_stack_.back().second == stack_.size();
//    }
//
//    json_value detach_result()
//    {
//        json_value res = std::move(stack_.back());
//        stack_.pop_back();
//        return std::move(res);
//    }
//
protected:
//    std::vector<json_value> stack_;
//    std::vector<std::pair<state, size_t>> state_stack_; // state->stack_position
//    std::vector<std::string> strings_;
    std::vector<char> tempstr_, element_name_, attr_name_;
};

class basic_model : public model_base<basic_model>
{

};

}

#endif // SONIA_UTILITY_JSON_MODEL_HPP
