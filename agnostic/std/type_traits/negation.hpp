// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_NEGATION

#include "agnostic/std/type_traits/integral_constant.hpp"

namespace std {
    
template <typename B> struct negation : bool_constant<!bool(B::value)> {};

template <typename... B>
inline constexpr bool negation_v = negation<B...>::value;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
