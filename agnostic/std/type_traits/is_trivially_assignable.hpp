// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "integral_constant.hpp"
#endif

namespace std {

template <class T, class U>
struct is_trivially_assignable : bool_constant<__is_trivially_assignable(T, U)> {};

template <class T, class U>
constexpr bool is_trivially_assignable_v = __is_trivially_assignable(T, U);

}
