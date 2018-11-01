//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_IS_NEGATIVE_HPP
#define SONIA_FUNCTIONAL_IS_NEGATIVE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <cmath>

#include "sonia/cstdint.hpp"
#include "sonia/type_traits.hpp"

namespace sonia {

template <typename T, typename Enable = void>
struct is_negative {
    bool operator()(T val) const { return std::signbit(val); }
};

template <typename T>
struct is_negative<T, enable_if_t<is_integral_v<T>>> {
    bool operator()(T val) const {
        if constexpr (is_signed_v<T>) {
            return val < 0;
        } else {
            return false;
        }
    }
};

template <typename T>
bool signbit(T val) { return is_negative<T>()(val); }

}

#endif // SONIA_FUNCTIONAL_IS_NEGATIVE_HPP
