//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <functional>
#include <typeindex>
#include "sonia/type_traits.hpp"
#include "sonia/utility/variadic.hpp"

namespace sonia {

template <typename T> struct hash
{
    inline size_t operator()(T const& v) const noexcept
    {
        return hash_value(v);
    }
};

template <std::integral T> struct hash<T> : std::hash<T> {};
template <std::floating_point T> struct hash<T> : std::hash<T> {};
template <> struct hash<std::type_index> : std::hash<std::type_index> {};
template <> struct hash<nullptr_t> { inline size_t operator()(nullptr_t) const noexcept { return 0; } };

template <class T>
inline void hash_combine(std::size_t& seed, const T& v) noexcept
{
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

inline constexpr size_t hash_init_value()
{
    if constexpr (sizeof(size_t) <= 4)
    {
        return 2166136261U;
    } else {
        return 14695981039346656037ULL;
    }
}

inline constexpr size_t hash_prime_value()
{
    if constexpr (sizeof(size_t) <= 4)
    {
        return 16777619U;
    } else {
        return 1099511628211ULL;
    }
}

struct hasher
{
    template <typename T>
    inline size_t operator()(T const& arg) const noexcept
    {
        return hash<T>{}(arg);
    }

    template <typename ... Ts>
    requires(sizeof...(Ts) > 1)
    inline size_t operator()(Ts const& ... vs) const noexcept
    {
        constexpr size_t seed = hash_init_value();
        return do_work(std::make_index_sequence<sizeof ...(Ts)>(), seed, vs ...);
    }

    template <typename ... Ts, size_t ... Idxs>
    inline size_t do_work(std::index_sequence<Idxs...>, size_t seed, Ts const& ... vs) const noexcept
    {
        (sonia::hash_combine(seed, hash<variadic::type_at_t<Idxs, Ts...>>{}(variadic::forward_at<Idxs>(vs ...))), ...);
        return seed;
    }
};

}
