// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_CONDITIONAL_EXT_HPP
#define AGNOSTIC_STD_CONDITIONAL_EXT_HPP

#pragma once

namespace std {

template <bool CV, typename T, typename F>
using conditional_t = typename conditional<CV, T, F>::type;

template <typename CT, typename T, typename F>
struct if_ : conditional<CT::value, T, F> {};

template <typename CT, typename T, typename F>
using if_t = typename if_<CT, T, F>::type;

}

#endif // AGNOSTIC_STD_CONDITIONAL_EXT_HPP
