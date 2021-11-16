// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_UNINITIALIZED_FILL

#include "agnostic/std/iterator/iterator_traits.hpp"
#include "agnostic/std/type_traits/is_assignable.hpp"
#include "agnostic/std/type_traits/is_same.hpp"
#include "agnostic/std/new/placement_new.hpp"
#include "agnostic/std/memory/addressof.hpp"

#include "agnostic/stdext/unwrap_enum.hpp"
#include "agnostic/stdext/is_character_or_byte.hpp"

namespace std {

template <typename ForwardIt, typename T>
void uninitialized_fill(ForwardIt first, ForwardIt last, T const& value)
{
    using val_t = typename iterator_traits<ForwardIt>::value_type;
    using ref_t = typename iterator_traits<ForwardIt>::reference;

    if constexpr (
            is_assignable_v<ref_t, const T&>
                &&
            (
                (
                    stdext::is_character_or_byte_v<stdext::unwrap_enum_t<T>> &&
                    stdext::is_character_or_byte_v<stdext::unwrap_enum_t<val_t>>
                )
                    ||
                (
                    is_same_v<bool, stdext::unwrap_enum_t<T>> &&
                    is_same_v<bool, stdext::unwrap_enum_t<val_t>>
                )
            )
            
        )
    {
        memset(first, static_cast<unsigned char>(value), static_cast<size_t>(last - first));
    } else {
        
        ForwardIt current = first;
        try {
            for (; current != last; ++current) {
                ::new (static_cast<void*>(addressof(*current))) val_t(value);
            }
        }
        catch (...) {
            destroy(first, current);
            throw;
        }
    }
}

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <memory>
#endif
