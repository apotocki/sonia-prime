// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_TRIVIALLY_DESTRUCTIBLE_HPP
#define AGNOSTIC_STD_IS_TRIVIALLY_DESTRUCTIBLE_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "integral_constant.hpp"
#endif

namespace std {

#ifdef _MSC_VER
template <class T>
struct is_trivially_destructible : bool_constant<__is_trivially_destructible(T)> {};

template <class T>
constexpr bool is_trivially_destructible_v = __is_trivially_destructible(T);

#else

template <class T>
struct is_trivially_destructible : bool_constant<__has_trivial_destructor(T)> {};

template <class T>
constexpr bool is_trivially_destructible_v = __has_trivial_destructor(T);

#endif

}

#endif // AGNOSTIC_STD_IS_TRIVIALLY_DESTRUCTIBLE_HPP
