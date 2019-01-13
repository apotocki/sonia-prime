//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_BOOST_SERIALIZATION_TUPLE_HPP
#define SONIA_BOOST_SERIALIZATION_TUPLE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <tuple>
#include <string>

namespace boost { namespace serialization {

template <size_t ... I, class ArchiveT, class TupleT>
void do_save(std::index_sequence<I ...>, ArchiveT & ar, TupleT const& t)
{
    (ar << ... << boost::serialization::make_nvp(std::to_string(I).c_str(), std::get<I>(t)));
}

template <class ArchiveT, typename ... Ts>
void save(ArchiveT & ar, const std::tuple<Ts ...> & t, const unsigned int /*version*/)
{
    do_save(std::make_index_sequence<sizeof ...(Ts)>(), ar, t);
}

template <size_t ... I, class ArchiveT, class TupleT>
void do_load(std::index_sequence<I ...>, ArchiveT & ar, TupleT & t)
{
    (ar >> ... >> boost::serialization::make_nvp(std::to_string(I).c_str(), std::get<I>(t)));
}

template <class ArchiveT, typename ... Ts>
void load(ArchiveT & ar, std::tuple<Ts ...> & t, const unsigned int /*version*/)
{
    do_load(std::make_index_sequence<sizeof ...(Ts)>(), ar, t);
}

template<class ArchiveT, typename ... Ts>
void serialize(ArchiveT & ar, std::tuple<Ts...> & t, const unsigned int version)
{
    boost::serialization::split_free(ar, t, version);
}

}}

#endif // SONIA_BOOST_SERIALIZATION_TUPLE_HPP
