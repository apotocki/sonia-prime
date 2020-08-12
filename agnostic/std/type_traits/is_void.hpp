// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_VOID

#include "agnostic/std/type_traits/integral_constant.hpp"

namespace std {

template <typename T> struct is_void : false_type {};

template <> struct is_void<void> : true_type {};

template <typename T> constexpr bool is_void_v = is_void<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
