//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_HASH_TUPLE_HPP
#define SONIA_FUNCTIONAL_HASH_TUPLE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <tuple>

#include "sonia/utility/functional/hash.hpp"

namespace sonia {

template <typename ... Ts>
struct hash<std::tuple<Ts...>>
{
    size_t operator()(std::tuple<Ts...> const& v) const
    {
        size_t seed = hash_init_value();
        return do_work(seed, v, std::make_index_sequence<sizeof ...(Ts)>());
    }

    template <typename T, size_t ... Idxs>
    size_t do_work(size_t seed, T const& v, std::index_sequence<Idxs...>) const
    {
        (hash_combine(seed, hasher{}(std::get<Idxs>(v))), ...);
        return seed;
    }
};

}

#endif // SONIA_FUNCTIONAL_HASH_TUPLE_HPP
