//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <algorithm>

#include <map>
#include <unordered_map>

#include "serialization.hpp"

namespace sonia::serialization {

template <typename TagT, class MapT>
struct map_coder
{
    using key_type = typename MapT::key_type;
    using mapped_type = typename MapT::mapped_type;
    using size_type = typename MapT::size_type;

    template <typename OutputIteratorT>
    OutputIteratorT encode(MapT const& value, OutputIteratorT oi) const
    {
        oi = sonia::encode<TagT, size_type>(value.size(), std::move(oi));
        for (auto const& [k, v] : value) {
            oi = sonia::encode<TagT>(k, std::move(oi));
            oi = sonia::encode<TagT>(v, std::move(oi));
        }
        return oi;
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, MapT& value) const
    {
        typename MapT::size_type sz;
        ii = sonia::decode<TagT>(std::move(ii), sz);
        value.reserve(sz);
        for (size_type i = 0; i < sz; ++i) {
            key_type k; ii = sonia::decode<TagT>(std::move(ii), k);
            mapped_type v; ii = sonia::decode<TagT>(std::move(ii), v);
            value.emplace(std::move(k), std::move(v));
        }
        return ii;
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, MapT* value) const
    {
        try {
            new(value) MapT;
            return sonia::decode<TagT>(std::move(ii), *value);
        } catch (...) {
            std::destroy_at(value);
            throw;
        }
    }
};

template <typename TagT, typename KT, typename VT, typename ... Ps>
class coder<TagT, std::unordered_map<KT, VT, Ps...>>
    : public map_coder<TagT, std::unordered_map<KT, VT, Ps...>>
{};

template <typename TagT, typename KT, typename VT, typename ... Ps>
class coder<TagT, std::map<KT, VT, Ps...>>
    : public map_coder<TagT, std::map<KT, VT, Ps...>>
{};

}
