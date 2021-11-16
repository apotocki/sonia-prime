// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "is_character.hpp"

namespace stdext {

using namespace std;

template <typename T> struct is_character_or_byte : is_character<T> {};

#if _HAS_STD_BYTE
template <> struct is_character_or_byte<byte> : true_type {};
#endif

template <typename T> constexpr bool is_character_or_byte_v = is_character_or_byte<T>::value;

}
