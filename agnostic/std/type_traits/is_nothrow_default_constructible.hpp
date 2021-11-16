// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_DEFAULT_CONSTRUCTIBLE

#include "agnostic/std/type_traits/is_nothrow_constructible.hpp"

namespace std {

template <typename T> struct is_nothrow_default_constructible : is_nothrow_constructible<T> {};

template <typename T> constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
