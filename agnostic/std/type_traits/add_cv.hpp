// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ADD_CV

namespace std {

template <typename T> struct add_cv
{
    using type = add_volatile_t<add_const_t<T>>;
};

template <typename T> using add_cv_t = typename add_cv<T>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
