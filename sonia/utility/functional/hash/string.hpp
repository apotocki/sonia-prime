//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_HASH_STRING_HPP
#define SONIA_FUNCTIONAL_HASH_STRING_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/string.hpp"
#include "array_view.hpp"

namespace sonia {

template <class CharT, class TraitsT>
struct hash<basic_string_view<CharT, TraitsT>> {
    size_t operator()(basic_string_view<CharT, TraitsT> str) const noexcept {
        return hash<array_view<const CharT>>()(str);
    }
};

template <class CharT, class TraitsT>
struct hash<basic_cstring_view<CharT, TraitsT>> {
    size_t operator()(basic_cstring_view<CharT, TraitsT> str) const noexcept {
        return hash<array_view<const CharT>>()(str);
    }
};

template <class CharT, class TraitsT, class AllocatorT>
struct hash<std::basic_string<CharT, TraitsT, AllocatorT>> {
    size_t operator()(std::basic_string<CharT, TraitsT, AllocatorT> const& str) const noexcept {
        return hash<array_view<const CharT>>()(to_string_view(str));
    }
};

}

#endif // SONIA_FUNCTIONAL_HASH_STRING_HPP
