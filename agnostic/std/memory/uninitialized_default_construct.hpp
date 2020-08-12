// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ITERATOR_TRAITS
#   include "../iterator/iterator_traits.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE
#   include "../type_traits/is_trivially_default_constructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_PLACEMENT_NEW
#   include "../new/placement_new.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ADDRESSOF
#   include "addressof.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_DESTROY
#   include "destroy.hpp"
#endif

namespace std {

template <class ForwardIt>
void uninitialized_default_construct(ForwardIt first, ForwardIt last)
{
    using v_t = iterator_traits<ForwardIt>::value_type;

    if constexpr (!is_trivially_default_constructible_v<v_t>) {
        ForwardIt current = first;
        try {
            for (; current != last; ++current) {
                ::new (static_cast<void*>(addressof(*current))) v_t;
            }
        } catch (...) {
            destroy(first, current);
            throw;
        }
    }
}

}
