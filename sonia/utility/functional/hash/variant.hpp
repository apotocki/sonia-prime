//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_HASH_VARIANT_HPP
#define SONIA_FUNCTIONAL_HASH_VARIANT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/variant.hpp>

#include "sonia/utility/functional/hash.hpp"

namespace sonia {

struct variant_hash_visitor : boost::static_visitor<size_t>
{
    template <typename T>
    size_t operator()(T const& val) const
    {
        return  hash<T>()(val);
    }
};

template <typename ... Ts>
struct hash<boost::variant<Ts...>>
{
    size_t operator()(boost::variant<Ts...> const& v) const noexcept
    {
        return boost::apply_visitor(variant_hash_visitor(), v);
    }
};

}

#endif // SONIA_FUNCTIONAL_HASH_VARIANT_HPP
