// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_SIGNED_INTEGRAL_CONCEPT

#include "agnostic/std/concepts/integral.hpp"

namespace std {

template <typename T>
concept signed_integral = integral<T> && T(-1) < T(0);

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
