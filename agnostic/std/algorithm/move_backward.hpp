// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_MOVE_BACKWARD

#include "agnostic/std/iterator/iterator_traits.hpp"
#include "agnostic/std/type_traits/is_assignable.hpp"
#include "agnostic/std/type_traits/is_trivial.hpp"
#include "agnostic/std/type_traits/is_pointer.hpp"
#include "agnostic/std/utility/move.hpp"


namespace std {

template <class BidirIt1, class BidirIt2>
OutputIt move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
{
    using v0_t = iterator_traits<InputIt>::value_type;
    using r0_t = iterator_traits<InputIt>::reference;
    using v1_t = iterator_traits<OutputIt>::value_type;
    using r1_t = iterator_traits<OutputIt>::reference;

    if constexpr (is_trivial_v<v0_t> && is_trivial_v<v1_t> && is_assignable_v<r1_t, r0_t>
        && is_pointer_v<InputIt> && is_pointer_v<OutputIt>)
    {
        return static_cast<OutputIt>(memmove(d_first, first, last - first));
    }
    else {
        while (first != last) {
            *(--d_last) = move(*(--last));
        }
        return d_last;
    }
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <algorithm>
#endif
