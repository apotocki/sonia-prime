// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_POINTER_HPP
#define AGNOSTIC_STD_IS_POINTER_HPP

#pragma once

namespace std {

template <typename T> struct is_pointer : false_type {};
template <typename T> struct is_pointer<T*> : true_type {};
//template <typename T> struct is_pointer<T const*> : true_type {};
//template <typename T> struct is_pointer<T volatile*> : true_type {};
//template <typename T> struct is_pointer<T const volatile*> : true_type {};

template <typename T> constexpr bool is_pointer_v = is_pointer<T>::value;

}

#endif // AGNOSTIC_STD_IS_POINTER_HPP
