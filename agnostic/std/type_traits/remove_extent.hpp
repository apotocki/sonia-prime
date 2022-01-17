// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_EXTENT_HPP

namespace std {

template <typename T> struct remove_extent { using type = T; };

template <typename T> struct remove_extent<T[]> { using type = T; };

template <typename T, size_t N> struct remove_extent<T[N]> { using type = T; };

template <typename T, unsigned N = 0> using remove_extent_t = typename remove_extent<T>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
