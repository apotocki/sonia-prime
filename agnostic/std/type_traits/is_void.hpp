// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_VOID_HPP
#define AGNOSTIC_STD_IS_VOID_HPP

#pragma once

namespace std {

template <typename T> struct is_void : false_type {};

template <> struct is_void<void> : true_type {};

template <typename T> constexpr bool is_void_v = is_void<T>::value;

}

#endif // AGNOSTIC_STD_IS_VOID_HPP
