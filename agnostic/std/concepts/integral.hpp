// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONCEPT

#include "agnostic/std/type_traits/is_integral.hpp"

namespace std {

template <typename T>
concept integral = is_integral_v<T>;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
