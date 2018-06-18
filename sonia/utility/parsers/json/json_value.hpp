//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_JSON_VALUE_HPP
#define SONIA_UTILITY_JSON_VALUE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/optional.hpp"

namespace sonia {

class json_value_visitor {
public:
    virtual ~json_value_visitor() {}
};

class json_value {
public:
    virtual ~json_value() {}

    virtual bool is_null() const = 0;
    virtual bool is_boolean() const = 0;
    virtual bool is_integral() const = 0;
    virtual bool is_floating() const = 0;
    virtual bool is_string() const = 0;
    virtual bool is_object() const = 0;
    virtual bool is_array() const = 0;

    virtual optional<bool> get_boolean() const = 0;
    virtual intptr_t const* get_integral() const = 0;
    virtual float const* get_floating() const = 0;
    virtual std::string const* get_string() const = 0;
    
    virtual size_t size() const = 0;
    virtual shared_ptr<json_value> operator[](size_t) const = 0;
    virtual shared_ptr<json_value> operator[](string_view) const = 0;
};

}
#endif // SONIA_UTILITY_JSON_VALUE_HPP
