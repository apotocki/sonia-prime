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

#include "integral.hpp"

namespace sonia { namespace serialization {

template <typename TagT, typename T>
class coder<TagT, std::vector<T>> {
    typedef std::vector<T> vector_t;
    typedef typename vector_t::size_type size_type;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(vector_t const& value, OutputIteratorT oi) const {
        oi = sonia::encode<TagT, size_type>(value.size(), std::move(oi));
        if constexpr (sizeof(T) == 1) {
            return std::copy(value.begin(), value.end(), std::move(oi));
        } else {
            coder<TagT, T> enc;
            for (auto it = value.begin(), eit = value.end(); it != eit; ++it) {
                oi = enc.encode(*it, std::move(oi));
            }
            return std::move(oi);
        }
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, vector_t & value) const {
        value.clear();
        size_type sz;
        ii = sonia::decode<TagT, size_type>(std::move(ii), sz);
        if constexpr (sizeof(T) == 1) {
            value.reserve(sz);
            return std::copy_n(std::move(ii), sz, std::back_inserter(value));
        } else {
            coder<TagT, T> dec;
            value.resize(sz);
            for (auto sit = value.begin(), eit = value.end(); sit != eit; ++sit) {
                ii = dec.decode(std::move(ii), *sit);
            }
            return std::move(ii);
        }
    }
};

}}

#endif // SONIA_SERIALIZATION_VECTOR_HPP
