// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_ASSIGNABLE

#include "agnostic/std/type_traits/integral_constant.hpp"
//#include "agnostic/std/utility/declval.hpp"

namespace std {

#if 1

template <typename T, class U>
struct is_assignable : bool_constant<__is_assignable(T, U)> {};

#else

namespace detail {

struct is_assignable_imp
{
    struct no_type { char padding[8]; };

    template<typename T, typename U, typename = decltype(declval<T>() = declval<U>())>
    static char test(int);

    template<typename, typename>
    static no_type test(...);
};

template <class T, class U> struct is_assignable 
    : public integral_constant<bool, sizeof(detail::is_assignable_imp::test<T, U>(0)) == 1>
{}

template <class T, size_t N, class U> struct is_assignable<T[N], U> : public is_assignable<T, U> {};
template <class T, size_t N, class U> struct is_assignable<T(&)[N], U> : public is_assignable<T&, U> {};
template <class T, class U> struct is_assignable<T[], U> : public is_assignable<T, U> {};
template <class T, class U> struct is_assignable<T(&)[], U> : public is_assignable<T&, U> {};
template <class U> struct is_assignable<void, U> : public integral_constant<bool, false> {};
template <class U> struct is_assignable<void const, U> : public integral_constant<bool, false> {};
template <class U> struct is_assignable<void volatile, U> : public integral_constant<bool, false> {};
template <class U> struct is_assignable<void const volatile, U> : public integral_constant<bool, false> {};

}

#endif

template <typename T, typename U> constexpr bool is_assignable_v = is_assignable<T, U>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
