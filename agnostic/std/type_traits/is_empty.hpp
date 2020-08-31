// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_EMPTY

#include "agnostic/std/type_traits/integral_constant.hpp"

namespace std {

template <typename T> struct is_empty : bool_constant<__is_empty(T)> {};
template <typename T> constexpr bool is_empty_v = __is_empty(T);

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
