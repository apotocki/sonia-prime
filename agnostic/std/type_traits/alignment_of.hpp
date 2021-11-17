// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALIGNMENT_OF

#include "agnostic/std/type_traits/integral_constant.hpp"

namespace std {

template <typename T> struct alignment_of : integral_constant<size_t, alignof(T)> {};
template <typename T> constexpr size_t alignment_of_v = alignof(T);

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
