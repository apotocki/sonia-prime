// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_DISABLE_IF_HPP
#define AGNOSTIC_STD_DISABLE_IF_HPP

#pragma once

namespace std {

template <bool B, typename T = void> struct disable_if {};

template <typename T> struct disable_if<false, T> { using type = T; };

template <bool B, typename T = void> using disable_if_t = typename disable_if<B, T>::type;

}

#endif // AGNOSTIC_STD_DISABLE_IF_HPP
