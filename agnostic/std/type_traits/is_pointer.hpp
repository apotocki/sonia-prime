// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_POINTER

#include "agnostic/std/type_traits/integral_constant.hpp"

namespace std {

template <typename T> struct is_pointer : false_type {};
template <typename T> struct is_pointer<T*> : true_type {};
//template <typename T> struct is_pointer<T const*> : true_type {};
//template <typename T> struct is_pointer<T volatile*> : true_type {};
//template <typename T> struct is_pointer<T const volatile*> : true_type {};

template <typename T> constexpr bool is_pointer_v = is_pointer<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
