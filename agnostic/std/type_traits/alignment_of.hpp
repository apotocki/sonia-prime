// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ALIGNMENT_OF_HPP
#define AGNOSTIC_STD_ALIGNMENT_OF_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "integral_constant.hpp"
#endif

namespace std {

template <typename T> struct alignment_of : integral_constant<size_t, alignof(T)> {};
template <typename T> constexpr size_t alignment_of_v = alignof(T);

}

#endif // AGNOSTIC_STD_ALIGNMENT_OF_HPP
