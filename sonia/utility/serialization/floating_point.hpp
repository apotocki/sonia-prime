//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "serialization.hpp"
#include "sonia/utility/algorithm/pull.hpp"

namespace sonia::serialization {

template <typename TagT, std::floating_point T>
class coder<TagT, T>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(const T value, OutputIteratorT oi) const
    {
        const uint8_t* pbytes = reinterpret_cast<const uint8_t*>(&value);
        return std::copy(pbytes, pbytes + sizeof(T), std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T * value) const
    {
        alignas(T) uint8_t pbytes[sizeof(T)];
        ii = pull(std::move(ii), pbytes, pbytes + sizeof(T));
        *value = *reinterpret_cast<T*>(pbytes);
        return std::move(ii);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, T& value) const
    {
        return decode(std::move(ii), &value);
    }
};

}
