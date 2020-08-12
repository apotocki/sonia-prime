// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_COPYABLE

#include "agnostic/std/type_traits/integral_constant.hpp"

namespace std {

template <class T>
struct is_trivially_copyable : bool_constant<__is_trivially_copyable(T)> {};

template <class T>
constexpr bool is_trivially_copyable_v = __is_trivially_copyable(T);

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
