// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_FLOATING_POINT_HPP
#define AGNOSTIC_STD_IS_FLOATING_POINT_HPP

#pragma once

namespace std {

template <typename T> struct is_floating_point : bool_constant<
    is_any_of_v<
        remove_cv_t<T>, 
        float, double, long double
    >>{};

template <typename T> constexpr bool is_floating_point_v = is_floating_point<T>::value;

}

#endif // AGNOSTIC_STD_IS_FLOATING_POINT_HPP
