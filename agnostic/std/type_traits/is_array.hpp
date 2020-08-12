// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_ARRAY

#include "agnostic/std/type_traits/integral_constant.hpp"

namespace std {

template <typename T> struct is_array : false_type {};

template <typename T> struct is_array<T[]> : true_type {};

template <class T, size_t N> struct is_array<T[N]> : true_type {};

template <typename T> constexpr bool is_array_v = is_array<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
