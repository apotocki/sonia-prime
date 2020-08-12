// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_TRIVIAL_HPP
#define AGNOSTIC_STD_IS_TRIVIAL_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "integral_constant.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_CONSTRUCTIBLE
#   include "is_trivially_constructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_COPYABLE
#   include "is_trivially_copyable.hpp"
#endif

namespace std {

template <class T>
struct is_trivial : bool_constant<is_trivially_constructible_v<T> && is_trivially_copyable_v<T>> {};

template <class T>
constexpr bool is_trivial_v = is_trivial<T>::value;

}

#endif // AGNOSTIC_STD_IS_TRIVIAL_HPP
