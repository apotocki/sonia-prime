// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_IS_CONSTRUCTIBLE
#   include "is_constructible.hpp"
#endif

namespace std {

template <typename T> struct is_default_constructible : is_constructible<T> {};

template <typename T> constexpr bool is_default_constructible_v = is_default_constructible<T>::value;

}
