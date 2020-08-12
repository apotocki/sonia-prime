// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_MOVE_ASSIGNABLE

#include "agnostic/std/type_traits/integral_constant.hpp"


#ifdef _MSC_VER

#include "agnostic/std/type_traits/add_lvalue_reference.hpp"

namespace std {

template <typename T>
struct is_nothrow_move_assignable : integral_constant<bool, __is_nothrow_assignable(add_lvalue_reference_t<T>, T)> {};
template <typename T> constexpr bool is_nothrow_move_assignable_v = __is_nothrow_assignable(add_lvalue_reference_t<T>, T);

}

#else

#include "agnostic/stdext/is_referenceable.hpp"
#include "agnostic/std/type_traits/is_nothrow_assignable.hpp"

namespace std {

namespace detail {

template <typename T, bool = stdext::is_referenceable<T>::value>
struct is_nothrow_move_assignable_impl : false_type {};

template <typename T>
struct is_nothrow_move_assignable_impl<T, true> : is_nothrow_assignable<T&, T&&>

}

template <typename T>
struct is_nothrow_move_assignable : detail::is_nothrow_move_assignable_impl<T> {};
template <typename T> constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;

}

#endif

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
