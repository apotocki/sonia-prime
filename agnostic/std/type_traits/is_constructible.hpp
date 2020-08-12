// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_CONSTRUCTIBLE_HPP
#define AGNOSTIC_STD_IS_CONSTRUCTIBLE_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_DECLVAL
#   include "../utility/declval.hpp"
#endif

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

#endif // AGNOSTIC_STD_IS_CONSTRUCTIBLE_HPP
