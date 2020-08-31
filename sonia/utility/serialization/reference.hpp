//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_REFERENCE_HPP
#define SONIA_SERIALIZATION_REFERENCE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include "sonia/utility/rvalue.hpp"
#include "serialization.hpp"

namespace sonia::serialization {

template <typename TagT, class T>
class coder<TagT, reference_wrapper<T>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(reference_wrapper<T> v, OutputIteratorT oi) const
    {
        return sonia::encode<TagT>(v.get(), std::move(oi));
    }

    template <typename InputIteratorT>
    disable_if_t<is_const_v<T>, InputIteratorT> decode(InputIteratorT ii, reference_wrapper<T> vr) const
    {
        return sonia::decode<TagT>(std::move(ii), vr.get());
    }
};

template <typename TagT, class T>
class coder<TagT, rvalue_reference_wrapper<T>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(rvalue_reference_wrapper<T> v, OutputIteratorT oi) const
    {
        return sonia::encode<TagT>(v.get(), std::move(oi));
    }
};

template <typename TagT, class T>
class coder<TagT, rvalue_wrapper<T>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(rvalue_wrapper<T> v, OutputIteratorT oi) const
    {
        return sonia::encode<TagT>(v.get(), std::move(oi));
    }
};

}

#endif // SONIA_SERIALIZATION_REFERENCE_HPP
