// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_NOTHROW_MOVE_CONSTRUCTIBLE_HPP
#define AGNOSTIC_STD_IS_NOTHROW_MOVE_CONSTRUCTIBLE_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "integral_constant.hpp"
#endif

#ifdef _MSC_VER

namespace std {

template <typename T>
struct is_nothrow_move_constructible : integral_constant<bool, __is_nothrow_constructible(T, T)> {};
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

#endif // AGNOSTIC_STD_IS_NOTHROW_MOVE_CONSTRUCTIBLE_HPP
