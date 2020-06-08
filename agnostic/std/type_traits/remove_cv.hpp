// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_CONST
#   include "remove_const.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_VOLATILE
#   include "remove_volatile.hpp"
#endif

namespace std {

template <typename T> struct remove_cv { using type = remove_volatile_t<remove_const_t<T>>; };
template <typename T> using remove_cv_t = typename remove_cv<T>::type;

}
