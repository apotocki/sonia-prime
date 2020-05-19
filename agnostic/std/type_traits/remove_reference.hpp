// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_REMOVE_REFERENCE_HPP
#define AGNOSTIC_STD_REMOVE_REFERENCE_HPP

#pragma once

namespace std {

template <typename T> struct remove_reference { using type = T; };
template <typename T> struct remove_reference<T&> { using type = T; };
template <typename T> struct remove_reference<T&&> { using type = T; };
template <typename T> using remove_reference_t = typename remove_reference<T>::type;

}

#endif // AGNOSTIC_STD_REMOVE_REFERENCE_HPP
