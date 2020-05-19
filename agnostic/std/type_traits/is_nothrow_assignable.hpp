// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_NOTHROW_ASSIGNABLE_HPP
#define AGNOSTIC_STD_IS_NOTHROW_ASSIGNABLE_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_DECLVAL
#   include "../utility/declval.hpp"
#endif

namespace std {

#ifdef _MSC_VER
template <typename T, class U>
struct is_nothrow_assignable : integral_constant<bool, __is_nothrow_assignable(T, U)> {};
#else

namespace detail {

template <typename T, typename U>
struct is_nothrow_assignable_impl : integral_constant<bool, noexcept(declval<T>() = declval<U>())> {};

}

template <typename T, typename U>
struct is_nothrow_assignable : conjunction<
    is_assignable<T, U>,
    detail::is_nothrow_assignable_impl<T, U>
>{}; 

#endif

template <typename T, typename U> constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<T, U>::value;

}

#endif // AGNOSTIC_STD_IS_NOTHROW_ASSIGNABLE_HPP
