// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_DISJUNCTION_HPP
#define AGNOSTIC_STD_DISJUNCTION_HPP

#pragma once

namespace std {
    
template <typename...> struct disjunction : false_type { };
template <typename B1> struct disjunction<B1> : B1 { };
template <typename B1, typename... Bn> struct disjunction<B1, Bn...>
    : conditional_t<bool(B1::value), B1, disjunction<Bn...>> {};

template <typename... B>
inline constexpr bool disjunction_v = disjunction<B...>::value;

}

#endif // AGNOSTIC_STD_DISJUNCTION_HPP
