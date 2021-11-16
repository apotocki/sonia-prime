// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT

namespace std {

template <typename T, T v>
struct integral_constant
{
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

template <bool B> using bool_constant = integral_constant<bool, B>;
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
