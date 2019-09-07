//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_REGISTRY_HPP
#define SONIA_SERVICES_REGISTRY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>

#include "sonia/string.hpp"
#include "sonia/cstdint.hpp"

namespace sonia {

class registry
{
public:
    virtual ~registry() = default;

    virtual uint32_t lookup(string_view) = 0;
    virtual std::string const* lookup(uint32_t) const = 0;
};

}

#endif // SONIA_SERVICES_REGISTRY_HPP
