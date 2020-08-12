// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ADD_CONST_HPP
#define AGNOSTIC_STD_ADD_CONST_HPP

#pragma once

namespace std {

template <typename T> struct add_const { using type = const T; };
template <typename T> using add_const_t = typename add_const<T>::type;

}

#endif // AGNOSTIC_STD_ADD_CONST_HPP
