// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_SWAP

#include "agnostic/std/utility/move.hpp"
#include "agnostic/std/type_traits/is_nothrow_move_constructible.hpp"
#include "agnostic/std/type_traits/is_nothrow_move_assignable.hpp"

namespace std {

template< class T >
constexpr void swap(T& a, T& b) noexcept(is_nothrow_move_constructible_v<T>&& is_nothrow_move_assignable_v<T>)
{
    T tmp = move(a);
    a = move(b);
    b = move(tmp);
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <algorithm>
#endif
