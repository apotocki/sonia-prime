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

namespace sonia { namespace serialization {

template <typename TagT, class T>
class coder<TagT, T*, enable_if_t<is_base_of_v<service, T>>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T * pserv, OutputIteratorT oi) {
        return coder<TagT, service::id>().encode(pserv->get_id(), std::move(oi));
    }
};

template <typename TagT, class T>
class coder<TagT, T, enable_if_t<is_base_of_v<service, T>>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(T const& serv, OutputIteratorT oi) {
        return coder<TagT, service::id>().encode(serv.get_id(), std::move(oi));
    }
};

}}

#endif // SONIA_SERIALIZATION_SERVICE_HPP
