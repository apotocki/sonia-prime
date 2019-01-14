//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_VECTOR_HPP
#define SONIA_SERIALIZATION_VECTOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <algorithm>
#include <vector>

#include "array.hpp"
#include "integral.hpp"

namespace sonia { namespace serialization {

template <typename TagT, typename T, class AllocT>
class coder<TagT, std::vector<T, AllocT>>
{
    using type = std::vector<T, AllocT>;
    using size_type = typename vector_t::size_type;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(type const& value, OutputIteratorT oi) const
    {
        return sonia::encode<TagT>(to_array_view(value), 
            sonia::encode<TagT, size_type>(value.size(), std::move(oi));
        );
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type & value) const
    {
        size_type sz;
        ii = sonia::decode<TagT>(std::move(ii), sz);
        value.resize(sz);
        return sonia::decode<TagT>(std::move(ii), to_array_view(value));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type * value) const
    {
        return default_decode_ptr(std::move(ii), value);
    }
};

}}

#endif // SONIA_SERIALIZATION_VECTOR_HPP
