// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IN_PLACE_HPP
#define AGNOSTIC_STD_IN_PLACE_HPP

#pragma once

namespace std {

struct in_place_t { explicit in_place_t() = default; };
inline constexpr in_place_t in_place{};

template <class T> struct in_place_type_t { explicit in_place_type_t() = default; };
template <typename T> inline constexpr in_place_type_t<T> in_place_type{};

template <size_t I> struct in_place_index_t
{ explicit in_place_index_t() = default; };

template <size_t I>
inline constexpr in_place_index_t<I> in_place_index{};

}

#endif // AGNOSTIC_STD_IN_PLACE_HPP
