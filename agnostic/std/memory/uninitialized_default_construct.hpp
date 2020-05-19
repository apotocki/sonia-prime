// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_UNINITIALIZED_DEFAULT_CONSTRUCT_HPP
#define AGNOSTIC_STD_UNINITIALIZED_DEFAULT_CONSTRUCT_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ITERATOR_TRAITS
#   include "../iterator/iterator_traits.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_DEFAULT_CONSTRUCTIBLE
#   include "../type_traits/is_default_constructible.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIAL
#   include "../type_traits/is_trivial.hpp"
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

    if constexpr (!is_trivial_v<v_t> && !is_default_constructible_v<v_t>) {
        ForwardIt current = first;
        try {
            for (; current != last; ++current) {
                ::new (static_cast<void*>(std::addressof(*current))) v_t;
            }
        } catch (...) {
            destroy(first, current);
            throw;
        }
    }
}

}

#endif // AGNOSTIC_STD_UNINITIALIZED_DEFAULT_CONSTRUCT_HPP
