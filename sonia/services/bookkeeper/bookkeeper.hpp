//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_BOOKKEEPER_HPP
#define SONIA_BOOKKEEPER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/optional.hpp"
#include "sonia/utility/json/value.hpp"

namespace sonia::services {

class bookkeeper
{
public:
    virtual ~bookkeeper() = default;
    
    virtual void set(string_view key, json_value const&) = 0;
    virtual optional<json_value> get(string_view key) const = 0;
    virtual void erase(string_view key) = 0;

    virtual bool compare_and_swap(string_view key, json_value const* expected, json_value const* newval) = 0;

    bool compare_and_swap(string_view key, json_value const* expected, json_value const& newval)
    {
        return compare_and_swap(key, expected, &newval);
    }

    bool compare_and_swap(string_view key, json_value const& expected, json_value const* newval)
    {
        return compare_and_swap(key, &expected, newval);
    }

    bool compare_and_swap(string_view key, json_value const& expected, json_value const& newval)
    {
        return compare_and_swap(key, &expected, &newval);
    }
};

}

#endif // SONIA_BOOKKEEPER_HPP
