//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>

#include "sonia/string.hpp"
#include "sonia/cstdint.hpp"

namespace sonia {

class type_registry
{
public:
    virtual ~type_registry() = default;

    virtual uint32_t get_type_id(std::string_view, std::string_view) = 0;
    virtual std::pair<std::string_view, std::string_view> get_type_description(uint32_t) const = 0; // throws an exception if a description is undefined for the given id.
};

}
