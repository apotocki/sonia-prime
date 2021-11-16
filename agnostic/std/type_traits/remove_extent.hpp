// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_REMOVE_EXTENT_HPP
#define AGNOSTIC_STD_REMOVE_EXTENT_HPP

#pragma once

namespace std {

template <typename T> struct remove_extent { using type = T; };

template <typename T> struct remove_extent<T[]> { using type = T; };

template <typename T, size_t N> struct remove_extent<T[N]> { using type = T; };

template <typename T, unsigned N = 0> using remove_extent_t = typename remove_extent<T>::type;

}

#endif // AGNOSTIC_STD_REMOVE_EXTENT_HPP
