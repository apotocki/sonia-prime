//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_SERVICE_HPP
#define SONIA_SERIALIZATION_SERVICE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include "sonia/services.hpp"

#include "integral.hpp"

namespace sonia::serialization {

template <typename TagT, class T>
class coder<TagT, T, enable_if_t<is_base_of_v<service, T>>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T const& serv, OutputIteratorT oi) const
    {
        return coder<TagT, service::id>().encode(serv.get_id(), std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, shared_ptr<T> & val) const
    {
        service::id sid;
        ii = sonia::decode<TagT>(std::move(ii), sid);
        if (!val || val->get_id() != sid) {
            val = sonia::services::locate<T>(sid);
        }
        return std::move(ii);
    }
};

template <typename TagT, class T>
class coder<TagT, T*, enable_if_t<is_base_of_v<service, T>>>
    : public coder<TagT, T>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T * pserv, OutputIteratorT oi) const
    {
        return coder<TagT, T>::encode(*pserv, std::move(oi));
    }
};

template <typename TagT, class T>
class coder<TagT, shared_ptr<T>, enable_if_t<is_base_of_v<service, T>>>
    : public coder<TagT, T>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(shared_ptr<T> const& serv, OutputIteratorT oi) const
    {
        return coder<TagT, T>::encode(*serv, std::move(oi));
    }
};

}

#endif // SONIA_SERIALIZATION_SERVICE_HPP
