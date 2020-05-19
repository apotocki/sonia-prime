// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_REMOVE_ALL_EXTENTS_HPP
#define AGNOSTIC_STD_REMOVE_ALL_EXTENTS_HPP

#pragma once

namespace std {

template <typename T> struct remove_all_extents { using type = T; };

template <typename T> struct remove_all_extents<T[]> { using type = typename remove_all_extents<T>::type; };

template <typename T, size_t N> struct remove_all_extents<T[N]> { using type = typename remove_all_extents<T>::type; };
    
template <typename T, unsigned N = 0> using remove_all_extents_t = typename remove_all_extents<T>::type;

}

#endif // AGNOSTIC_STD_REMOVE_ALL_EXTENTS_HPP
