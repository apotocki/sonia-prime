// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_MOVE_CONSTRUCTIBLE

#include "agnostic/std/type_traits/integral_constant.hpp"

#ifdef _MSC_VER

namespace std {

template <typename T>
struct is_nothrow_move_constructible : bool_constant<__is_nothrow_constructible(T, T)> {};
template <typename T> constexpr bool is_nothrow_move_constructible_v = __is_nothrow_constructible(T, T);

}

#else 

#include "is_referenceable.hpp"

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_CONSTRUCTIBLE
#   include "is_nothrow_constructible.hpp"
#endif

namespace std {

namespace detail {

template <typename T, bool = is_referenceable<T>::value>
struct is_nothrow_move_constructible_impl : false_type {};

template <typename T>
struct is_nothrow_move_constructible_impl<T, true> : is_nothrow_constructible<T, T&&>

}

template <typename T>
struct is_nothrow_move_constructible : detail::is_nothrow_move_constructible_impl<T> {};
template <typename T> constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;

}
#endif

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
