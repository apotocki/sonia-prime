// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_UNINITIALIZED_MOVE

#include "agnostic/std/iterator/iterator_traits.hpp"
#include "agnostic/std/type_traits/is_assignable.hpp"
#include "agnostic/std/type_traits/is_trivial.hpp"

#include "agnostic/std/algorithm/copy.hpp"
#include "agnostic/std/new/placement_new.hpp"
#include "agnostic/std/utility/move.hpp"
#include "agnostic/std/memory/addressof.hpp"

namespace std {

template <class InputIt, class ForwardIt>
ForwardIt uninitialized_move(InputIt first, InputIt last, ForwardIt d_first)
{
    using v0_t = iterator_traits<InputIt>::value_type;
    using r0_t = iterator_traits<InputIt>::reference;
    using v1_t = iterator_traits<ForwardIt>::value_type;
    using r1_t = iterator_traits<ForwardIt>::reference;

    if constexpr (is_trivial_v<v0_t> && is_trivial_v<v1_t> && is_assignable_v<r1_t, r0_t>) {
        return copy(first, last, d_first);
    } else {
        ForwardIt current = d_first;
        try {
            for (; first != last; ++first, ++current) {
                ::new (static_cast<void*>(std::addressof(*current))) v1_t(move(*first));
            }
            return current;
        }  catch (...) {
            destroy(d_first, current);
            throw;
        }
    }
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <memory>
#endif
