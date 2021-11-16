// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_DESTRUCTIBLE_CONCEPT

#include "agnostic/std/type_traits/is_nothrow_destructible.hpp"

namespace std {

template <class T>
concept destructible = is_nothrow_destructible_v<T>;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
