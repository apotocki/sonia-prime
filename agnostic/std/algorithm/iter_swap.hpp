// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_ITER_SWAP

#include "agnostic/std/algorithm/swap.hpp"

namespace std {

template <class ForwardIt1, class ForwardIt2>
constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b)
{
    using std::swap;
    swap(*a, *b);
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <algorithm>
#endif
