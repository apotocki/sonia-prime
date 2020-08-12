// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_TRIVIALLY_CONSTRUCTIBLE_HPP
#define AGNOSTIC_STD_IS_TRIVIALLY_CONSTRUCTIBLE_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "integral_constant.hpp"
#endif

namespace std {

template <class T, class... Args>
struct is_trivially_constructible : bool_constant<__is_trivially_constructible(T, _Args...)> {};

template <class T, class... Args>
constexpr bool is_trivially_constructible_v = __is_trivially_constructible(T, _Args...);

}

#endif // AGNOSTIC_STD_IS_TRIVIALLY_CONSTRUCTIBLE_HPP
