// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_RANK_HPP
#define AGNOSTIC_STD_RANK_HPP

#pragma once

namespace std {


template <typename T>
struct rank : public integral_constant<size_t, 0> {};

template <typename T>
struct rank<T[]> : public integral_constant<size_t, rank<T>::value + 1> {};

template <typename T, size_t N>
struct rank<T[N]> : public integral_constant<size_t, rank<T>::value + 1> {};


template <typename T, unsigned N = 0> constexpr size_t rank_v = rank<T>::value;

}

#endif // AGNOSTIC_STD_RANK_HPP
