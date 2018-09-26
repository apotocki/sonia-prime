//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_TYPE_REGISTRY_HPP
#define SONIA_TYPE_REGISTRY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/string.hpp"
#include "sonia/cstdint.hpp"

namespace sonia {

class type_registry {
public:
    virtual ~type_registry() {}

    virtual uint32_t get_type_id(string_view) = 0;
    virtual string_view get_type_name(uint32_t) const = 0; // throws an exception if the name is undefined for the given id.
};

}

#endif // SONIA_TYPE_REGISTRY_HPP
