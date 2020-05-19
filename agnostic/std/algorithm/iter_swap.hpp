// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ALGORITHM_ITER_SWAP_HPP
#define AGNOSTIC_STD_ALGORITHM_ITER_SWAP_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_MOVE
#   include "../utility/move.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_MOVE_CONSTRUCTIBLE
#   include "../type_traits/is_nothrow_move_constructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_NOTHROW_MOVE_ASSIGNABLE
#   include "../type_traits/is_nothrow_move_assignable.hpp"
#endif

namespace std {

template< class T >
constexpr void swap(T& a, T& b) noexcept(is_nothrow_move_constructible_v<T> && is_nothrow_move_assignable_v<T>)
{
    T tmp = move(a);
    a = move(b);
    b = move(tmp);
}

}

#endif // AGNOSTIC_STD_ALGORITHM_ITER_SWAP_HPP
