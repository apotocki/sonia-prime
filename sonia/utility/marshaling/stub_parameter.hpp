//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_MARSHALING_STUB_PARAMETER_HPP
#define SONIA_UTILITY_MARSHALING_STUB_PARAMETER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include "sonia/utility/serialization/serialization.hpp"

namespace sonia {

template <typename T, typename Enabler = void>
struct stub_bound_parameter { using type = remove_cvref_t<T>; };

template <typename T> using stub_bound_parameter_t = typename stub_bound_parameter<T>::type;


template <class T, class PointerT>
struct stub_parameter_holder
{
    PointerT ptr;
    operator T&() const { return *ptr; }
    operator T*() const { return ptr.get(); }
};

namespace serialization {

template <typename TagT, class T, class PointerT>
class coder<TagT, stub_parameter_holder<T, PointerT>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(stub_parameter_holder<T, PointerT> const& val, OutputIteratorT oi) const
    {
        return sonia::encode<TagT>(*val.ptr, std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, stub_parameter_holder<T, PointerT> & val) const
    {
        return sonia::decode<TagT>(std::move(ii), val.ptr);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, stub_parameter_holder<T, PointerT> * val) const
    {
        PointerT ptr;
        ii = sonia::decode<TagT>(std::move(ii), ptr);
        new (val) stub_parameter_holder<T, PointerT>{std::move(ptr)};
        return std:move(ii);
    }
};

}}

#endif // SONIA_UTILITY_MARSHALING_STUB_PARAMETER_HPP
