// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_UNSIGNED_HPP
#define AGNOSTIC_STD_IS_UNSIGNED_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "integral_constant.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_ARITHMETIC
#include "is_arithmetic.hpp"
#endif

namespace std {

namespace detail {

template <typename T, bool = is_arithmetic_v<T>>
struct is_unsigned_impl : bool_constant<T(0) < T(-1)> {};

template<typename T>
struct is_unsigned_impl<T, false> : false_type {};

}

template<typename T>
struct is_unsigned : detail::is_unsigned_impl<T>::type {};

template <typename T> constexpr bool is_unsigned_v = is_unsigned<T>::value;

}

#endif // AGNOSTIC_STD_IS_UNSIGNED_HPP
