// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_CONVERTIBLE

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/type_traits/is_void.hpp"
#include "agnostic/std/utility/declval.hpp"

namespace std {

namespace detail {

template <typename> using true_type_for = true_type;

template <typename T>
auto test_returnable(int)->true_type_for<T()>;

template <typename>
auto test_returnable(...)->false_type;

template <typename From, typename To>
auto test_nonvoid_convertible(int)->true_type_for<
    decltype(declval<void(&)(To)>()(declval<From>()))
>;

template <typename, typename>
auto test_nonvoid_convertible(...)->false_type;

}

template <typename From, typename To>
using is_convertible = bool_constant<
    (decltype(detail::test_returnable<To>(0))::value &&
        decltype(detail::test_nonvoid_convertible<From, To>(0))::value) ||
    (is_void_v<From> && is_void_v<To>)
>;

template <typename From, typename To> constexpr bool is_convertible_v = is_convertible<From, To>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
