//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/type_traits.hpp"
#include "serialization.hpp"

namespace sonia::serialization {

template <typename TagT, class T>
requires(0 != is_placeholder_v<T>)
class coder<TagT, T>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T const&, OutputIteratorT oi) const noexcept { return std::move(oi); }

    template <typename InputIteratorT, typename AnyT>
    InputIteratorT decode(InputIteratorT ii, AnyT &&) const noexcept { return std::move(ii); }
};

}
