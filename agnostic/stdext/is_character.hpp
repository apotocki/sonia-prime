// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#include "agnostic/std/type_traits/integral_constant.hpp"

namespace stdext {

using namespace std;

template <typename T> struct is_character : false_type {};

template <> struct is_character<char> : true_type {};

template <> struct is_character<signed char> : true_type {};

template <> struct is_character<unsigned char> : true_type {};

#ifdef __cpp_char8_t
template <> struct is_character<char8_t> : true_type {};
#endif

template <typename T> constexpr bool is_character_v = is_character<T>::value;

}
