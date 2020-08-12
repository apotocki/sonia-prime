// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ALGORITHM_ITER_SWAP_HPP
#define AGNOSTIC_STD_ALGORITHM_ITER_SWAP_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_SWAP
#   include "swap.hpp"
#endif

namespace std {

template <class ForwardIt1, class ForwardIt2>
constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b)
{
    using std::swap;
    swap(*a, *b);
}

}

#endif // AGNOSTIC_STD_ALGORITHM_ITER_SWAP_HPP
