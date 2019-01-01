//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_ABS_HPP
#define SONIA_FUNCTIONAL_ABS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <cmath>

#include "sonia/cstdint.hpp"
#include "sonia/type_traits.hpp"

namespace sonia {

template <typename T, typename Enable = void>
struct abs_f {
    bool operator()(T val) const { return std::abs(val); }
};

template <typename T>
struct abs_f<T, enable_if_t<is_integral_v<T>>> {
    T operator()(T val) const {
        if constexpr (is_signed_v<T>) {
            return val < 0 ? -val : val;
        } else {
            return val;
        }
    }
};

template <typename T>
T abs(T val) { return abs_f<T>()(val); }

}

#endif // SONIA_FUNCTIONAL_ABS_HPP
