// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_REMOVE_CONST_HPP
#define AGNOSTIC_STD_REMOVE_CONST_HPP

#pragma once

namespace std {

template <typename T> struct remove_const { using type = T; };
template <typename T> struct remove_const<const T> { using type = T; };
template <typename T> using remove_const_t = typename remove_const<T>::type;

}

#endif // AGNOSTIC_STD_REMOVE_CONST_HPP
