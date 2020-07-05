// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_DERIVED_FROM_CONCEPT

#include "agnostic/std/type_traits/is_base_of.hpp"
#include "agnostic/std/type_traits/is_convertible.hpp"

namespace std {

template <class Derived, class Base>
concept derived_from =
    is_base_of_v<Base, Derived> &&
    is_convertible_v<const volatile Derived*, const volatile Base*>;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
