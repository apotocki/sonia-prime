//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "basic_decimal.hpp"
#include "sonia/mp/basic_integer.hpp"

namespace sonia {

using decimal = basic_decimal<
    mp::basic_integer<uint64_t, 1>,
    int32_t
>;

}
