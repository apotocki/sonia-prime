//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_OPTIONAL_HPP
#define SONIA_SERIALIZATION_OPTIONAL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/optional.hpp"
#include "sonia/utility/automatic.hpp"
#include "serialization.hpp"

namespace sonia { namespace serialization {

template <typename TagT, typename T>
class coder<TagT, optional<T>>
{
    using type = optional<T>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(type const& value, OutputIteratorT oi) const
    {
        if (value) {
            *oi = 0xff; ++oi;
            return sonia::encode<TagT>(*value, std::move(oi));
        } else {
            *oi = 0; ++oi;
            return std::move(oi);
        }
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type & value) const
    {
        uint8_t v = *ii; ++ii;
        if (v) {
            automatic<T> tmp(in_place_decode<TagT>(ii));
            value.emplace(std::move(*tmp));
        } else {
            value.reset();
        }
        return std::move(ii);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type * value) const
    {
        new(value) type;
        try {
            sonia::decode<TagT, T>(std::move(ii), *value);
        } else {
            std::destroy_at(value);
            throw;
        }
    }
};

}}

#endif // SONIA_SERIALIZATION_OPTIONAL_HPP
