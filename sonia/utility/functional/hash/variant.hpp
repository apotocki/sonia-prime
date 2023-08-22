//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#ifndef SONIA_VARIANT_DECLARED
#   error do not include directly, use "sonia/variant.hpp"
#endif

#include "sonia/utility/functional/hash.hpp"

namespace sonia {

struct variant_hash_visitor : static_visitor<size_t>
{
    template <typename T>
    size_t operator()(T const& val) const
    {
        return hash<T>()(val);
    }
};

template <typename ... Ts>
struct hash<variant<Ts...>>
{
    size_t operator()(variant<Ts...> const& v) const
    {
        return apply_visitor(variant_hash_visitor(), v);
    }
};

}
