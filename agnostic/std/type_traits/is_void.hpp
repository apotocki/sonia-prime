// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "agnostic/std/type_traits/integral_constant.hpp"
#endif

namespace std {

template <typename T> struct is_void : false_type {};

template <> struct is_void<void> : true_type {};

template <typename T> constexpr bool is_void_v = is_void<T>::value;

}
