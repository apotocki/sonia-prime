//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_INTEGRAL_HPP
#define SONIA_SERIALIZATION_INTEGRAL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <algorithm>

#include "sonia/type_traits.hpp"
#include "sonia/algorithm/pull.hpp"

#include "serialization.hpp"

namespace sonia { namespace serialization {

template <typename T>
class coder<default_t, T, enable_if_t<is_integral_v<T>>>
{
public:
    template <typename ArgT, typename OutputIteratorT>
    OutputIteratorT encode(ArgT val, OutputIteratorT oi) const {
        auto it = reinterpret_cast<char const*>(&val), eit = it + sizeof(ArgT);
        return std::copy(it, eit, oi);
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, ArgT * val) const {
        auto it = reinterpret_cast<char*>(val), eit = it + sizeof(ArgT);
        return pull(std::move(ii), it, eit);
    }

    template <typename ArgT, typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, ArgT & val) const {
        return decode(std::move(ii), &val);
    }
};

}}

#endif // SONIA_SERIALIZATION_INTEGRAL_HPP
