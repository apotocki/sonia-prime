//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>

#include "sonia/utility/functional/hash.hpp"

namespace sonia {

template <typename FT, typename ST>
struct hash<std::pair<FT, ST>>
{
    size_t operator()(std::pair<FT, ST> const& v) const
    {
        size_t seed = hash_init_value();
        hash_combine(seed, hasher{}(v.first));
        hash_combine(seed, hasher{}(v.second));
        return seed;
    }

};

}
