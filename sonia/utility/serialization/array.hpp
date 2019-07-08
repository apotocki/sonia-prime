//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_ARRAY_HPP
#define SONIA_SERIALIZATION_ARRAY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <algorithm>

#include "sonia/array_view.hpp"
#include "sonia/utility/algorithm/pull.hpp"
#include "sonia/utility/algorithm/copy.hpp"
#include "serialization.hpp"

namespace sonia { namespace serialization {

template <typename TagT, typename T>
class coder<TagT, array_view<T>>
{
    using type = array_view<T>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(type const& value, OutputIteratorT oi) const
    {
        if constexpr (is_trivial_v<T> && sizeof(T) == 1) {
            return sonia::copy(value.begin(), value.end(), std::move(oi));
        } else {
            coder<TagT, remove_cv_t<T>> enc;
            for (auto const& e : value) {
                oi = enc.encode(e, std::move(oi));
            }
            return std::move(oi);
        }
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type value) const
    {
        if constexpr (is_trivial_v<T> && sizeof(T) == 1) {
            return sonia::pull(std::move(ii), value.begin(), value.end());
        } else {
            coder<TagT, T> dec;
            for (auto & e : value) {
                ii = dec.decode(std::move(ii), e);
            }
            return std::move(ii);
        }
    }
};

template <typename TagT, typename T, size_t SzV>
class coder<TagT, std::array<T, SzV>>
{
    using type = std::array<T, SzV>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(type const& value, OutputIteratorT oi) const
    {
        return sonia::encode<TagT>(to_array_view(value), std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, type value) const
    {
        return sonia::decode<TagT>(std::move(ii), to_array_view(value));
    }
};

template <typename TagT, class VectorT, typename SizeT = typename VectorT::size_type>
struct vector_coder
{
    using vector_coder_t = vector_coder;

    template <typename OutputIteratorT>
    OutputIteratorT encode(VectorT const& value, OutputIteratorT oi) const
    {
        return sonia::encode<TagT>(to_array_view(value),
            sonia::encode<TagT, SizeT>(value.size(), std::move(oi))
        );
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, VectorT & value) const
    {
        SizeT sz;
        ii = sonia::decode<TagT>(std::move(ii), sz);
        value.resize(sz);
        return sonia::decode<TagT>(std::move(ii), to_array_view(value));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, VectorT * value) const
    {
        SizeT sz;
        ii = sonia::decode<TagT>(std::move(ii), sz);

        try {
            new(value) VectorT(sz);
            return sonia::decode<TagT>(std::move(ii), to_array_view(*value));
        } catch (...) {
            std::destroy_at(value);
            throw;
        }
    }
};

}}

#endif // SONIA_SERIALIZATION_ARRAY_HPP
