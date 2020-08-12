// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_SAME_AS_CONCEPT

#include "agnostic/std/type_traits/is_same.hpp"

namespace std {

template <class T1, class T2>
concept same_as = is_same_v<T1, T2> && is_same_v<T2, T1>;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
