//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_HASH_HPP
#define SONIA_FUNCTIONAL_HASH_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <functional>
#include <typeindex>
#include "sonia/type_traits.hpp"
#include "sonia/utility/functional/has.hpp"

namespace sonia {

template <typename T, typename Enabler = void> struct hash
{
    size_t operator()(T const& v) const
    {
        return hash_value(v);
    }
};

template <typename T> struct hash<T, enable_if_t<is_integral_v<T>>> : std::hash<T> {};

template <> struct hash<std::type_index> : std::hash<std::type_index> {};

template <class T>
inline void hash_combine(std::size_t& seed, const T& v) noexcept
{
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct hasher
{
    template <typename T>
    size_t operator()(T && arg) const noexcept
    {
        return hash<remove_cvref_t<T>>()(std::forward<T>(arg));
    }
};

inline constexpr size_t hash_init_value()
{
    if constexpr (sizeof(size_t) == 4)
    {
        return 2166136261U;
    } else {
        return 14695981039346656037ULL;
    }
}

inline constexpr size_t hash_prime_value()
{
    if constexpr (sizeof(size_t) == 4)
    {
        return 16777619U;
    } else {
        return 1099511628211ULL;
    }
}

}

#endif // SONIA_FUNCTIONAL_HASH_HPP
