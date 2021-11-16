// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_DESTRUCTIBLE

#include "agnostic/std/type_traits/integral_constant.hpp"

namespace std {

template <typename T> struct is_nothrow_destructible : bool_constant<__is_nothrow_destructible(T)> {};

template <typename T> constexpr bool is_nothrow_destructible_v = __is_nothrow_destructible(T);

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
