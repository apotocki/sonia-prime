// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_IS_ANY_OF_HPP
#define AGNOSTIC_STD_IS_ANY_OF_HPP

#pragma once

namespace std {
    
template <typename T, typename... Types>
constexpr bool is_any_of_v = disjunction_v<is_same<T, Types>...>;

template <typename T, typename... Types>
using is_any_of = bool_constant<is_any_of_v<T, Types>>;

}

#endif // AGNOSTIC_STD_IS_ANY_OF_HPP
