//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_FUNCTIONAL_HASH_VECTOR_HPP
#define SONIA_FUNCTIONAL_HASH_VECTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/array_view.hpp"

namespace sonia {

template <typename T, class AllocatorT>
struct hash<std::vector<T, AllocatorT>>
{
    size_t operator()(std::vector<T, AllocatorT> const& vec) const noexcept
    {
        return hasher()(to_array_view(vec));
    }
};

}

#endif // SONIA_FUNCTIONAL_HASH_VECTOR_HPP
