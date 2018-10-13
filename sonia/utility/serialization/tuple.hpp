//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_TUPLE_HPP
#define SONIA_SERIALIZATION_TUPLE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"

#include "serialization.hpp"

namespace sonia { namespace serialization {

template <typename TagT, typename ... Ts>
class coder<TagT, std::tuple<Ts ...>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(std::tuple<Ts ...> const& val, OutputIteratorT oi) const {
        return do_encode(std::make_index_sequence<sizeof ...(Ts)>(), val, std::move(oi));
    }

private:
    template <size_t ... I, typename OutputIteratorT>
    static OutputIteratorT do_encode(std::index_sequence<I ...>, std::tuple<Ts ...> const& val, OutputIteratorT oi) {
        return (encoder<TagT, OutputIteratorT>(std::move(oi)) & ... & std::get<I>(val));
    }
    //template <typename ArgT, typename InputIteratorT>
    //InputIteratorT decode(InputIteratorT ii, ArgT * val) const {
    //    auto it = reinterpret_cast<char*>(val), eit = it + sizeof(ArgT);
    //    return pull(std::move(ii), it, eit);
    //}

    //template <typename ArgT, typename InputIteratorT>
    //InputIteratorT decode(InputIteratorT ii, ArgT & val) const {
    //    return decode(std::move(ii), &val);
    //}
};

}}

#endif // SONIA_SERIALIZATION_TUPLE_HPP
