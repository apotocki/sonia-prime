// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_NEGATION_HPP
#define AGNOSTIC_STD_NEGATION_HPP

#pragma once

namespace std {
    
template <typename B> struct negation : bool_constant<!bool(B::value)> {};

template <typename... B>
inline constexpr bool negation_v = negation<B...>::value;

}

#endif // AGNOSTIC_STD_NEGATION_HPP
