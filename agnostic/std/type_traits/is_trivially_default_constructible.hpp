// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "integral_constant.hpp"
#endif

namespace std {

template <class T>
struct is_trivially_default_constructible : bool_constant<__is_trivially_constructible(T)> {};

template <class T, class... Args>
constexpr bool is_trivially_default_constructible_v = __is_trivially_constructible(T);

}
