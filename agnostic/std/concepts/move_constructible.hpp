// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_MOVE_CONSTRUCTIBLE_CONCEPT

#include "agnostic/std/concepts/constructible_from.hpp"
#include "agnostic/std/concepts/convertible_to.hpp"

namespace std {

template< class T >
concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
