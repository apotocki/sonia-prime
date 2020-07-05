// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_IS_FLOATING_POINT

#include "agnostic/std/type_traits/integral_constant.hpp"
#include "agnostic/std/type_traits/remove_cv.hpp"
#include "agnostic/stdext/is_any_of.hpp"

namespace std {

template <typename T> struct is_floating_point : bool_constant<
    stdext::is_any_of_v<
        remove_cv_t<T>, 
        float, double, long double
    >>{};

template <typename T> constexpr bool is_floating_point_v = is_floating_point<T>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
