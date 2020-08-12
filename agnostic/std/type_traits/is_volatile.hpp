// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_VOLATILE_HPP
#define AGNOSTIC_STD_IS_VOLATILE_HPP

#pragma once

namespace std {

template <typename T> struct is_volatile : false_type {};

template <typename T> struct is_volatile<volatile T> : true_type {};

template <typename T> constexpr bool is_volatile_v = is_volatile<T>::value;

}

#endif // AGNOSTIC_STD_IS_VOLATILE_HPP
