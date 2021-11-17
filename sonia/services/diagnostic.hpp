//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>

#include "sonia/string.hpp"
#include "sonia/cstdint.hpp"

namespace sonia {

class diagnostic
{
public:
    virtual ~diagnostic() = default;

    virtual std::string get_diagnostic_info() = 0;
};

}
