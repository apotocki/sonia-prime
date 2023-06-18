//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>

#include "sonia/string.hpp"
#include "sonia/cstdint.hpp"

namespace sonia {

class registry
{
public:
    virtual ~registry() = default;

    virtual uint32_t lookup(std::string_view) = 0;
    virtual std::string const* lookup(uint32_t) const = 0;
};

}
