// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_INTEGRAL_CONSTANT_HPP
#define AGNOSTIC_STD_INTEGRAL_CONSTANT_HPP

#pragma once

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

#endif // AGNOSTIC_STD_INTEGRAL_CONSTANT_HPP
