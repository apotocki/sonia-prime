// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_MOVABLE_CONCEPT

#include "agnostic/std/type_traits/is_object.hpp"

#include "agnostic/std/concepts/move_constructible.hpp"
#include "agnostic/std/concepts/assignable_from.hpp"
#include "agnostic/std/concepts/swappable.hpp"

namespace std {

template<class T>
concept movable =
    is_object_v<T> &&
    move_constructible<T> &&
    assignable_from<T&, T> &&
    swappable<T>;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
