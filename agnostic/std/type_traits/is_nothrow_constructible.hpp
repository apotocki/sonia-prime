// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_CONSTRUCTIBLE

#include "agnostic/std/type_traits/integral_constant.hpp"

namespace std {

#ifdef _MSC_VER
template <typename T, typename... Args>
struct is_nothrow_constructible : bool_constant<__is_nothrow_constructible(T, Args...)> {};
#else

namespace detail {

template <typename T> struct is_nothrow_default_constructible_atom
    : public integral_constant<bool, noexcept(T())> {};

template <typename T, bool = is_array_v<T>> struct is_nothrow_default_constructible_impl;

template <typename T> struct is_nothrow_default_constructible_impl<T, true>
    : public conjunction<
        integral_constant<bool, (extent_v<T> > 0)>,
        is_nothrow_default_constructible_atom<remove_all_extents_t<T>>
    >
{};

template <typename T> struct is_nothrow_default_constructible_impl<T, false>
    : public is_nothrow_default_constructible_atom<T>
{};

template <typename T, typename... Args>
struct is_nothrow_constructible_impl : integral_constant<bool, noexcept(T(declval<Args>()...))> {};

template <typename T, typename Arg>
struct is_nothrow_constructible_impl<T, Arg> : integral_constant<bool, noexcept(static_cast<T>(declval<Arg>()))> {};

template <typename T>
struct is_nothrow_constructible_impl<T> : is_nothrow_default_constructible_impl<T> {};
    
}

template <typename T, typename... Args>
struct is_nothrow_constructible : conjunction<
    is_constructible<T, Args...>,
    detail::is_nothrow_constructible_impl<T, Args...>
>{};

#endif

template <typename T, typename... Args> constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
