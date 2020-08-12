// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_DECVAL

#include "agnostic/std/type_traits/add_rvalue_reference.hpp"

namespace std {

template <typename T>
add_rvalue_reference_t<T> declval() noexcept;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <utility>
#endif
