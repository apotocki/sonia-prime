// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_MOVE

#include "agnostic/std/iterator/iterator_traits.hpp"
#include "agnostic/std/type_traits/is_assignable.hpp"
#include "agnostic/std/type_traits/is_trivial.hpp"
#include "agnostic/std/type_traits/is_pointer.hpp"
#include "agnostic/std/utility/move.hpp"


namespace std {

template <class InputIt, class OutputIt>
OutputIt move(InputIt first, InputIt last, OutputIt d_first)
{
    using v0_t = iterator_traits<InputIt>::value_type;
    using r0_t = iterator_traits<InputIt>::reference;
    using v1_t = iterator_traits<OutputIt>::value_type;
    using r1_t = iterator_traits<OutputIt>::reference;

    if constexpr (is_trivial_v<v0_t> && is_trivial_v<v1_t> && is_assignable_v<r1_t, r0_t> 
        && is_pointer_v<InputIt> && is_pointer_v<OutputIt>)
    {
        return static_cast<OutputIt>(memcpy(d_first, first, last - first));
    } else {
        for (; first != last; ++d_first, ++first) {
            *d_first = move(*first);
        }
        return d_first;
    }
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <algorithm>
#endif
