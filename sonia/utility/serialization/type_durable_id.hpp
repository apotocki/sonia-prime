//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_TYPE_DURABLE_ID_HPP
#define SONIA_SERIALIZATION_TYPE_DURABLE_ID_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/utility/type_durable_id.hpp"

#include "serialization.hpp"
#include "integral.hpp"

namespace sonia { namespace serialization {

template <typename SerializationTagT>
class coder<SerializationTagT, sonia::type::durable_id>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(sonia::type::durable_id const& did, OutputIteratorT oi) const
    {
        return sonia::encode<SerializationTagT>(did.val_, std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, sonia::type::durable_id & did) const
    {
        uint32_t didval;
        ii = sonia::decode<SerializationTagT, uint32_t>(std::move(ii), didval);
        did = sonia::type::durable_id(didval);
        return std::move(ii);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, sonia::type::durable_id * did) const
    {
        uint32_t didval;
        ii = sonia::decode<SerializationTagT, uint32_t>(std::move(ii), didval);
        new(did) sonia::type::durable_id(didval);
        return std::move(ii);
    }
};

}}

#endif // SONIA_SERIALIZATION_TYPE_DURABLE_ID_HPP
