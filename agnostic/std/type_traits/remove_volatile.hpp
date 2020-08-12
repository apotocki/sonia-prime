// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_REMOVE_VOLATILE_HPP
#define AGNOSTIC_STD_REMOVE_VOLATILE_HPP

#pragma once

namespace std {

template <typename T> struct remove_volatile { using type = T; };
template <typename T> struct remove_volatile<volatile T> { using type = T; };
template <typename T> using remove_volatile_t = typename remove_volatile<T>::type;

}

#endif // AGNOSTIC_STD_REMOVE_VOLATILE_HPP
