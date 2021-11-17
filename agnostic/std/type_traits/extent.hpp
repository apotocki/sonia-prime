// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_EXTENT_HPP
#define AGNOSTIC_STD_EXTENT_HPP

#pragma once

namespace std {

template <typename T, unsigned N = 0>
struct extent : integral_constant<size_t, 0> {};

template <typename T>
struct extent<T[], 0> : integral_constant<size_t, 0> {};

template <typename T, unsigned N>
struct extent<T[], N> : integral_constant<size_t, extent<T, N - 1>::value> {};

template <typename T, size_t N>
struct extent<T[N], 0> : integral_constant<size_t, N> {};

template <typename T, size_t I, unsigned N>
struct extent<T[I], N> : integral_constant<size_t, extent<T, N - 1>::value> {};


template <typename T, unsigned N = 0> constexpr size_t extent_v = extent<T>::value;

}

#endif // AGNOSTIC_STD_EXTENT_HPP
