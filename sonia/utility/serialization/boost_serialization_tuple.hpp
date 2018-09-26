//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_BOOST_SERIALIZATION_TUPLE_HPP
#define SONIA_BOOST_SERIALIZATION_TUPLE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <tuple>
#include <boost/serialization/nvp.hpp>

namespace boost { namespace serialization {

template <class ArchiveT>
struct tuple_serializer
{
    explicit tuple_serializer(ArchiveT & ar)
        : ar_(ar)
    {}

    template <typename... ArgsT>
    void operator()(ArgsT&& ... args) {
        ((ar_ & make_nvp("item", std::forward<ArgsT>(args))), ...);
    }

    ArchiveT & ar_;
};

template<class ArchiveT, typename... Args>
void serialize(ArchiveT & ar, std::tuple<Args...> & t, const unsigned int)
{
    std::apply(tuple_serializer<ArchiveT>(ar), t);
}

}}

#endif // SONIA_BOOST_SERIALIZATION_TUPLE_HPP
