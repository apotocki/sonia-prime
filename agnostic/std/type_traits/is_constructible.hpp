// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_CONSTRUCTIBLE

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/type_traits/void.hpp"
#include "agnostic/std/utility/declval.hpp"

namespace std {

namespace detail {

template <class, typename T, typename... Args> struct _is_constructible : false_type {};

template <typename T, typename... Args>
struct _is_constructible<void_t<decltype(T(declval<Args>()...))>, T, Args...> : true_type {};

}

template <typename T, typename... Args>
using is_constructible = detail::_is_constructible<void, T, Args...>;

template <typename T, typename... Args> constexpr bool is_constructible_v = is_constructible<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
