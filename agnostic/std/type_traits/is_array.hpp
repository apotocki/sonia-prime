// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_ARRAY_HPP
#define AGNOSTIC_STD_IS_ARRAY_HPP

#pragma once

namespace std {

template <typename T> struct is_array : false_type {};

template <typename T> struct is_array<T[]> : true_type {};

template <class T, size_t N> struct is_array<T[N]> : true_type {};

template <typename T> constexpr bool is_array_v = is_array<T>::value;

}

#endif // AGNOSTIC_STD_IS_ARRAY_HPP
