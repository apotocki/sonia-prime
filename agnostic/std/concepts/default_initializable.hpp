// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_DEFAULT_INITIALIZABLE_CONCEPT

#include "agnostic/std/concepts/constructible_from.hpp"
#include "agnostic/std/new/placement_new.hpp"

namespace std {

template<class T>
concept default_initializable =
    constructible_from<T> &&
    requires { T{}; } &&
    requires { ::new (static_cast<void*>(nullptr)) T; };

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
