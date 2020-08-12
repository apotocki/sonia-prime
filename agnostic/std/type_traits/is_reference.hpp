// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_REFERENCE_HPP
#define AGNOSTIC_STD_IS_REFERENCE_HPP

#pragma once

namespace std {

template <typename T> struct is_reference : false_type {};
template <typename T> struct is_reference<T&> : true_type {};
template <typename T> struct is_reference<T&&> : true_type {};

template <typename T> constexpr bool is_reference_v = is_reference<T>::value;

}

#endif // AGNOSTIC_STD_IS_REFERENCE_HPP
