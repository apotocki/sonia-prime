//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include "sonia/array_view.hpp"

namespace sonia {

template <typename T, class AllocatorT>
struct hash<std::vector<T, AllocatorT>>
{
    size_t operator()(std::vector<T, AllocatorT> const& vec) const noexcept
    {
        return hasher()(array_view{vec});
    }
};

}
