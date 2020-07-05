// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_CONVERTIBLE_TO_CONCEPT

#include "agnostic/std/type_traits/add_rvalue_reference.hpp"

namespace std {

template <class From, class To>
concept convertible_to = __is_convertible_to(From, To)
    && requires(add_rvalue_reference_t<From>(&Fn)()) {
        static_cast<To>(Fn());
};

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <concepts>
#endif
