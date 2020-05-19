// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_UNINITIALIZED_COPY_HPP
#define AGNOSTIC_STD_UNINITIALIZED_COPY_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ITERATOR_TRAITS
#   include "../iterator/iterator_traits.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_ASSIGNABLE
#   include "../type_traits/is_assignable.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIAL
#   include "../type_traits/is_trivial.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALGORITHM_COPY
#   include "../algorithm/copy.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_PLACEMENT_NEW
#   include "../new/placement_new.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ADDRESSOF
#   include "addressof.hpp"
#endif

namespace std {

template <class InputIt, class ForwardIt>
ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first)
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
                ::new (static_cast<void*>(std::addressof(*current))) v1_t(*first);
            }
            return current;
        }  catch (...) {
            destroy(d_first, current);
            throw;
        }
    }
}

}

#endif // AGNOSTIC_STD_UNINITIALIZED_COPY_HPP
