// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_CONJUNCTION_HPP
#define AGNOSTIC_STD_CONJUNCTION_HPP

#pragma once

namespace std {
    
template <typename...> struct conjunction : true_type { };
template <typename B1> struct conjunction<B1> : B1 { };
template <typename B1, typename... Bn> struct conjunction<B1, Bn...>
    : conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

template <typename... B>
inline constexpr bool conjunction_v = conjunction<B...>::value;

}

#endif // AGNOSTIC_STD_CONJUNCTION_HPP
