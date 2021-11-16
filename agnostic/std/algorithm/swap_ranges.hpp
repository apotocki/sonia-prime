// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_SWAP_RANGES

#include "agnostic/std/algorithm/iter_swap.hpp"

namespace std {

template<class ForwardIt1, class ForwardIt2>
constexpr ForwardIt2 swap_ranges(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2)
{
    while (first1 != last1) {
        std::iter_swap(first1++, first2++);
    }
    return first2;
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <algorithm>
#endif
