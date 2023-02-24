//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>

#include "serialization.hpp"

namespace sonia::serialization {

template <typename TagT, typename FT, typename ST >
class coder<TagT, std::pair<FT, ST>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(std::pair<FT, ST> const& val, OutputIteratorT oi) const
    {
        return sonia::encode<TagT>(val.second, sonia::encode<TagT>(val.first, std::move(oi)));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, std::pair<FT, ST> & val) const
    {
        return sonia::decode<TagT>(sonia::decode<TagT>(std::move(ii), val.first), val.second);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, std::pair<FT, ST> * val) const
    {
        return default_decode_ptr<TagT>(std::move(ii), val);
    }
};

}
