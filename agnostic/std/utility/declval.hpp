// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ADD_RVALUE_REFERENCE
#   include "agnostic/std/type_traits/add_rvalue_reference.hpp"
#endif

namespace std {

template <typename T>
add_rvalue_reference_t<T> declval() noexcept;

}
