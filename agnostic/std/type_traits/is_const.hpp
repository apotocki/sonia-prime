// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_CONST_HPP
#define AGNOSTIC_STD_IS_CONST_HPP

#pragma once

namespace std {

template <typename T> struct is_const : false_type {};

template <typename T> struct is_const<const T> : true_type {};

template <typename T> constexpr bool is_const_v = is_const<T>::value;

}

#endif // AGNOSTIC_STD_IS_CONST_HPP
