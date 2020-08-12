// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_TRIVIALLY_COPYABLE_HPP
#define AGNOSTIC_STD_IS_TRIVIALLY_COPYABLE_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "integral_constant.hpp"
#endif

namespace std {

template <class T>
struct is_trivially_copyable : bool_constant<__is_trivially_copyable(T)> {};

template <class T>
constexpr bool is_trivially_copyable_v = __is_trivially_copyable(T);

}

#endif // AGNOSTIC_STD_IS_TRIVIALLY_COPYABLE_HPP
