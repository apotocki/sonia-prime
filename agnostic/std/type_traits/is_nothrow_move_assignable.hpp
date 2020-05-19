// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_NOTHROW_MOVE_ASSIGNABLE_HPP
#define AGNOSTIC_STD_IS_NOTHROW_MOVE_ASSIGNABLE_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "integral_constant.hpp"
#endif

#ifdef _MSC_VER

#ifndef DO_NOT_USE_AGNOSTIC_ADD_LVALUE_REFERENCE
#   include "add_lvalue_reference.hpp"
#endif

namespace std {

template <typename T>
struct is_nothrow_move_assignable : integral_constant<bool, __is_nothrow_assignable(add_lvalue_reference_t<T>, T)> {};
template <typename T> constexpr bool is_nothrow_move_assignable_v = __is_nothrow_assignable(add_lvalue_reference_t<T>, T);

}

#else

#include "is_referenceable.hpp"

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_ASSIGNABLE
#   include "is_nothrow_assignable.hpp"
#endif

namespace std {

namespace detail {

template <typename T, bool = is_referenceable<T>::value>
struct is_nothrow_move_assignable_impl : false_type {};

template <typename T>
struct is_nothrow_move_assignable_impl<T, true> : is_nothrow_assignable<T&, T&&>

}

template <typename T>
struct is_nothrow_move_assignable : detail::is_nothrow_move_assignable_impl<T> {};
template <typename T> constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;

}

#endif

#endif // AGNOSTIC_STD_IS_NOTHROW_MOVE_ASSIGNABLE_HPP
