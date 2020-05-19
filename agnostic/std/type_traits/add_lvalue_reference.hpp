// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ADD_LVALUE_REFERENCE_HPP
#define AGNOSTIC_STD_ADD_LVALUE_REFERENCE_HPP

#pragma once

namespace std {

template <typename T> struct add_lvalue_reference { using type = T&; };
template <typename T> struct add_lvalue_reference<T&> { using type = T&; };
template <typename T> struct add_lvalue_reference<T&&> { using type = T&; };
template <typename T> using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

}

#endif // AGNOSTIC_STD_ADD_LVALUE_REFERENCE_HPP
