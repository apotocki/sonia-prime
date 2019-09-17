//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_MAYBE_ARRAY_HPP
#define SONIA_SERIALIZATION_MAYBE_ARRAY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <vector>

#include "array.hpp"
#include "sonia/utility/optimized/maybe_array.hpp"

namespace sonia::serialization {

template <typename TagT, typename ElementT, size_t ByteSzV, typename RefCountT>
class coder<TagT, maybe_array<ElementT, ByteSzV, RefCountT>>
    : public vector_coder<TagT, maybe_array<ElementT, ByteSzV, RefCountT>>
{
    using type = maybe_array<ElementT, ByteSzV, RefCountT>;
    using vector_coder_t = typename coder::vector_coder_t;

public:
    using vector_coder_t::decode;

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type & value) const
    {
        typename type::size_type sz;
        ii = sonia::decode<TagT>(std::move(ii), sz);
        value = type{sz};
        return sonia::decode<TagT>(std::move(ii), value.to_array_view());
    }
};

}

#endif // SONIA_SERIALIZATION_MAYBE_ARRAY_HPP
