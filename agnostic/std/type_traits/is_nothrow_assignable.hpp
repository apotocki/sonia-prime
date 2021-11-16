// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_ASSIGNABLE

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/utility/declval.hpp"

namespace std {

#ifdef _MSC_VER
template <typename T, class U>
struct is_nothrow_assignable : bool_constant<__is_nothrow_assignable(T, U)> {};
#else

namespace detail {

template <typename T, typename U>
struct is_nothrow_assignable_impl : bool_constant<noexcept(declval<T>() = declval<U>())> {};

}

template <typename T, typename U>
struct is_nothrow_assignable : conjunction<
    is_assignable<T, U>,
    detail::is_nothrow_assignable_impl<T, U>
>{}; 

#endif

template <typename T, typename U> constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<T, U>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
