// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIAL

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/type_traits/is_trivially_constructible.hpp"
#include "agnostic/std/type_traits/is_trivially_copyable.hpp"

namespace std {

template <class T>
struct is_trivial : bool_constant<is_trivially_constructible_v<T> && is_trivially_copyable_v<T>> {};

template <class T>
constexpr bool is_trivial_v = is_trivial<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
