// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_ARITHMETIC_HPP
#define AGNOSTIC_STD_IS_ARITHMETIC_HPP

#pragma once

namespace std {

template <typename T> struct is_arithmetic : bool_constant<is_integral_v<T> || is_floating_point_v<T>> {};

template <typename T> constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

}

#endif // AGNOSTIC_STD_IS_ARITHMETIC_HPP
