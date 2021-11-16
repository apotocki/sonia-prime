// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_UTILITY_STATIC_LOG2_HPP
#define AGNOSTIC_UTILITY_STATIC_LOG2_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "agnostic/std/type_traits/integral_constant.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ENABLE_IF
#   include "agnostic/std/type_traits/enable_if.hpp"
#endif

namespace agnostic {

template <unsigned long long v>
struct static_log2 : std::integral_constant<unsigned long long, 1 + static_log2<v/2>::value> {};

template <> struct static_log2<0> {};
template <> struct static_log2<1> : std::integral_constant<unsigned long long, 0> {};

template <unsigned long long v> constexpr unsigned long long static_log2_v = static_log2<v>::value;

}

#endif // AGNOSTIC_STD_ALIGNMENT_OF_HPP
