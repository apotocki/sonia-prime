//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <span>
#include "stub_parameter.hpp"

namespace sonia {

template <typename T>
struct stub_bound_parameter<std::span<T>>
{
    using type = std::vector<remove_cv_t<T>>;
    static constexpr bool is_modifiable = false;
};

}
