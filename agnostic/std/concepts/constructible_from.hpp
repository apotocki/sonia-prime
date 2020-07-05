// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_CONSTRUCTIBLE_FROM_CONCEPT

#include "agnostic/std/concepts/destructible.hpp"
#include "agnostic/std/type_traits/is_constructible.hpp"

namespace std {

template <class T, class... Args>
concept constructible_from = destructible<T> && is_constructible_v<T, Args...>;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
