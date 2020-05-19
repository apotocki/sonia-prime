// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_SAME_HPP
#define AGNOSTIC_STD_IS_SAME_HPP

#pragma once

namespace std {

template <typename T, typename U> struct is_same : false_type {};
template <typename T> struct is_same<T, T> : true_type {};
template <typename T, typename U> constexpr bool is_same_v = is_same<T, U>::value;

}

#endif // AGNOSTIC_STD_IS_SAME_HPP
