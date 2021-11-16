// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_ARITHMETIC

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/type_traits/is_integral.hpp"
#include "agnostic/std/type_traits/is_floating_point.hpp"

namespace std {

template <typename T> struct is_arithmetic : bool_constant<is_integral_v<T> || is_floating_point_v<T>> {};

template <typename T> constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
