// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_SAME

#include "agnostic/std/type_traits/integral_constant.hpp"

namespace std {

template <typename T, typename U> struct is_same : false_type {};
template <typename T> struct is_same<T, T> : true_type {};
template <typename T, typename U> constexpr bool is_same_v = is_same<T, U>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
