// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ADD_RVALUE_REFERENCE_HPP
#define AGNOSTIC_STD_ADD_RVALUE_REFERENCE_HPP

#pragma once

namespace std {

template <typename T> struct add_rvalue_reference { using type = T&; };
template <typename T> struct add_rvalue_reference<T&> { using type = T&; };
template <typename T> struct add_rvalue_reference<T&&> { using type = T&&; };
template <typename T> using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

}

#endif // AGNOSTIC_STD_ADD_RVALUE_REFERENCE_HPP
