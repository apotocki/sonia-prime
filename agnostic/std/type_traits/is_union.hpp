// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_UNION_HPP
#define AGNOSTIC_STD_IS_UNION_HPP

#pragma once

namespace std {

template <typename T> struct is_union : bool_constant<__is_union(T)> {};

template <typename T> constexpr bool is_union_v = __is_union(T);

}

#endif // AGNOSTIC_STD_IS_UNION_HPP
