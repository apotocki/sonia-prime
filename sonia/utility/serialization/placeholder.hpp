//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_PLACEHOLDER_HPP
#define SONIA_SERIALIZATION_PLACEHOLDER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include "serialization.hpp"

namespace sonia::serialization {

template <typename TagT, class T>
class coder<TagT, T, enable_if_t<0 != is_placeholder_v<T>>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T const&, OutputIteratorT oi) const noexcept { return std::move(oi); }

    template <typename InputIteratorT, typename AnyT>
    InputIteratorT decode(InputIteratorT ii, AnyT &&) const noexcept { return std::move(ii); }
};

}

#endif // SONIA_SERIALIZATION_PLACEHOLDER_HPP
