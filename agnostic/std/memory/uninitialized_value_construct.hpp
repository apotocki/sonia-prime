// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_UNINITIALIZED_VALUE_CONSTRUCT

#include "agnostic/std/iterator/iterator_traits.hpp"
#include "agnostic/std/type_traits/is_scalar.hpp"
#include "agnostic/std/type_traits/is_volatile.hpp"
#include "agnostic/std/type_traits/is_member_pointer.hpp"
#include "agnostic/std/type_traits/remove_reference.hpp"
#include "agnostic/std/new/placement_new.hpp"
#include "agnostic/std/memory/addressof.hpp"
#include "agnostic/std/memory/destroy.hpp"

namespace std {

template <class ForwardIt>
void uninitialized_value_construct(ForwardIt first, ForwardIt last)
{
    using v_t = iterator_traits<ForwardIt>::value_type;

    if constexpr (is_pointer_v<ForwardIt> && is_scalar_v<v_t> 
        && !is_volatile_v<remove_reference_t<iterator_traits<ForwardIt>::reference>>
        && !is_member_pointer_v<v_t>)
    {
        memset(first, 0, static_cast<size_t>(last - first));
    } else {
        ForwardIt current = first;
        try {
            for (; current != last; ++current) {
                ::new (static_cast<void*>(addressof(*current))) v_t{};
            }
        } catch (...) {
            destroy(first, current);
            throw;
        }
    }
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <memory>
#endif
