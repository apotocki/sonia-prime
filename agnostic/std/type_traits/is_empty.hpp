// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_EMPTY_HPP
#define AGNOSTIC_STD_IS_EMPTY_HPP

#pragma once

namespace std {

template <typename T> struct is_empty : bool_constant<__is_empty(T)> {};
template <typename T> constexpr bool is_empty_v = __is_empty(T);

}

#endif // AGNOSTIC_STD_IS_EMPTY_HPP
