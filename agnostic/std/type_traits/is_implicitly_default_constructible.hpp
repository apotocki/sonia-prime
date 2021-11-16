// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/type_traits/void.hpp"

namespace std {

namespace detail {

template <typename T>
void implicitly_default_construct(T const&);

}

template <typename T, typename = void>
struct is_implicitly_default_constructible : false_type {};

template <typename T>
struct is_implicitly_default_constructible<T, void_t<decltype(detail::implicitly_default_construct<T>({}))>> : true_type {};

template <typename T> constexpr bool is_implicitly_default_constructible_v = is_implicitly_default_constructible<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
