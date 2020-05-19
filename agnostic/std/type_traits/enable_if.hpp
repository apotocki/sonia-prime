// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ENABLE_IF_HPP
#define AGNOSTIC_STD_ENABLE_IF_HPP

#pragma once

namespace std {

template <bool B, typename T = void> struct enable_if {};

template <typename T> struct enable_if<true, T> { using type = T; };

template <bool B, typename T = void> using enable_if_t = typename enable_if<B, T>::type;

}

#endif // AGNOSTIC_STD_ENABLE_IF_HPP
