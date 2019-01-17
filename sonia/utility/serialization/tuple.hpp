//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_TUPLE_HPP
#define SONIA_SERIALIZATION_TUPLE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <tuple>
#include <type_traits>
#include <utility>

#include "serialization.hpp"

namespace sonia { namespace serialization {

template <class TupleT, size_t N> struct tuple_item
{
    TupleT * tuple;
};

template <typename TagT, typename ... Ts, size_t N>
class coder<TagT, tuple_item<std::tuple<Ts ...>, N>>
{
    using tuple_t = std::tuple<Ts ...> ;
    using item_t = std::tuple_element_t<N, tuple_t>;

public:
    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, tuple_item<tuple_t, N> ts) const
    {
        try {
            return sonia::decode<TagT, item_t>(std::move(ii), &std::get<N>(*ts.tuple));
        } catch (...) {
            do_destroy(std::make_index_sequence<N>(), ts.tuple);
            throw;
        }
    }

private:
    template <typename T>
    static void destroyer(T * obj) noexcept { std::destroy_at(obj); }

    template <size_t ... I>
    static void do_destroy(std::index_sequence<I ...>, std::tuple<Ts ...> * val) noexcept
    {
        (destroyer(&std::get<I>(*val)), ...);
    }
};


template <typename TagT, typename ... Ts>
class coder<TagT, std::tuple<Ts ...>>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(std::tuple<Ts ...> const& val, OutputIteratorT oi) const
    {
        return do_encode(std::make_index_sequence<sizeof ...(Ts)>(), val, std::move(oi));
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, std::tuple<Ts ...> & val) const
    {
        return do_decode(std::make_index_sequence<sizeof ...(Ts)>(), std::move(ii), val);
    }

    // I rely on: a construction of all items of a tuple is equivalent of the tuple construction
    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, std::tuple<Ts ...> * val) const
    {
        return do_decode(std::make_index_sequence<sizeof ...(Ts)>(), std::move(ii), val);
    }

private:
    template <size_t ... I, typename OutputIteratorT>
    static OutputIteratorT do_encode(std::index_sequence<I ...>, std::tuple<Ts ...> const& val, OutputIteratorT oi)
    {
        return (encoder<TagT, OutputIteratorT>(std::move(oi)) & ... & std::get<I>(val));
    }

    template <size_t ... I, typename InputIteratorT>
    static InputIteratorT do_decode(std::index_sequence<I ...>, InputIteratorT ii, std::tuple<Ts ...> & val)
    {
        return (decoder<TagT, InputIteratorT>(std::move(ii)) & ... & std::get<I>(val));
    }

    template <size_t ... I, typename InputIteratorT>
    static InputIteratorT do_decode(std::index_sequence<I ...>, InputIteratorT ii, std::tuple<Ts ...> * val)
    {
        return (decoder<TagT, InputIteratorT>(std::move(ii)) & ... & tuple_item<std::tuple<Ts ...>, I>{val});
    }
};

}}

#endif // SONIA_SERIALIZATION_TUPLE_HPP
