//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <cmath>

#include "sonia/cstdint.hpp"
#include "sonia/type_traits.hpp"

namespace sonia {

template <typename T>
struct abs_f
{
    inline T operator()(T val) const noexcept { return std::abs(val); }
};

template <std::signed_integral T>
struct abs_f<T>
{
    inline std::make_unsigned_t<T> operator()(T val) const noexcept
    {
        std::make_unsigned_t<T> result = val;
        if (val < 0) {
            result = ~result + 1;
        }
        return result;
    }
};

template <std::unsigned_integral T>
struct abs_f<T>
{
    inline T operator()(T val) const noexcept { return val; }
};

template <typename T>
inline auto abs(T val) noexcept { return abs_f<T>{}(val); }

}
