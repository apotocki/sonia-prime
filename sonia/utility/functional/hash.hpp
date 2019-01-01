//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_HASH_HPP
#define SONIA_FUNCTIONAL_HASH_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <functional>
#include "sonia/type_traits.hpp"

namespace sonia {

template <typename T>
struct hash : public std::hash<T> {};

template <class T>
inline void hash_combine(std::size_t& seed, const T& v) noexcept
{
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template <typename T>
inline size_t hash_value(T && arg) {
    return hash<remove_cvref_t<T>>()(std::forward<T>(arg));
}

struct hasher {
    template <typename T>
    size_t operator()(T && arg) const noexcept {
        return hash<remove_cvref_t<T>>()(std::forward<T>(arg));
    }
};

}

#endif // SONIA_FUNCTIONAL_HASH_HPP
