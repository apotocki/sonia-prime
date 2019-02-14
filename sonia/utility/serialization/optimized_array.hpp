//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_OPTIMIZED_ARRAY_HPP
#define SONIA_SERIALIZATION_OPTIMIZED_ARRAY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/utility/optimized/array.hpp"

#include "array.hpp"
#include "integral.hpp"

namespace sonia { namespace serialization {

template <typename TagT, typename ElementT, size_t btsz, typename RefCountT>
class coder<TagT, shared_optimized_array<ElementT, btsz, RefCountT>>
    : public vector_coder<TagT, shared_optimized_array<ElementT, btsz, RefCountT>>
{
    using type = shared_optimized_array<ElementT, btsz, RefCountT>;
    using vector_coder_t = typename coder::vector_coder_t;

public:
    using vector_coder_t::decode;

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type & value) const
    {
        typename type::size_type sz;
        ii = sonia::decode<TagT>(std::move(ii), sz);
        value = type{sz};
        return sonia::decode<TagT>(std::move(ii), to_array_view(value));
    }
};

}}

#endif // SONIA_SERIALIZATION_OPTIMIZED_ARRAY_HPP
