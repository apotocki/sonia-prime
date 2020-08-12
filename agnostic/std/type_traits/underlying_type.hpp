// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_UNDERLYING_TYPE

#include "agnostic/std/type_traits/is_enum.hpp"

namespace std {

template <typename T, bool = is_enum_v<T>>
struct underlying_type
{
    using type = __underlying_type(T);
};

template <typename T>
struct underlying_type<T, false> {};

template <typename T>
using underlying_type_t = typename underlying_type<T>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
