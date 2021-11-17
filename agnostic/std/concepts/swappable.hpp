// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_SWAPPABLE_CONCEPT

#include "agnostic/std/concepts/ranges/swap.hpp"

namespace std {

template <class T>
concept swappable = requires(T& a, T& b) { ranges::swap(a, b); };

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
