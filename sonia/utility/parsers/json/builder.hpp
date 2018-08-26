//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_JSON_BUILDER_HPP
#define SONIA_UTILITY_JSON_BUILDER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"
#include "sonia/utility/json/value.hpp"

namespace sonia { namespace parsers { namespace json {

class builder {
public:
    typedef std::pair<int, intptr_t> descriptor_type;

    virtual ~builder() {}

    virtual descriptor_type build_null() = 0;
    virtual descriptor_type build_boolean(bool) = 0;
    virtual descriptor_type build_integer(intptr_t) = 0;
    virtual descriptor_type build_double(float) = 0;
    virtual descriptor_type build_string(string_view) = 0;
    virtual descriptor_type build_array(array_view<descriptor_type>) = 0;
    virtual descriptor_type build_object(array_view<std::string>, array_view<descriptor_type>) = 0;

    virtual void free(descriptor_type) = 0;

    virtual json_value build_json_value(descriptor_type) = 0;
};

}}}

#endif // SONIA_UTILITY_JSON_BUILDER_HPP
