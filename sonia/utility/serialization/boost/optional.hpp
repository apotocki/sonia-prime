//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_BOOST_SERIALIZATION_OPTIONAL_HPP
#define SONIA_BOOST_SERIALIZATION_OPTIONAL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <optional>

#include <boost/serialization/optional.hpp>

namespace boost::serialization {

template <class ArchiveT, class T>
void save(ArchiveT & ar, const std::optional<T> & t, const unsigned int /*version*/)
{
    const bool tflag = !!t;
    ar << boost::serialization::make_nvp("initialized", tflag);
    if (tflag) {
        ar << boost::serialization::make_nvp("value", *t);
    }
}

template <class ArchiveT, class T>
void load(ArchiveT & ar, std::optional<T> & t, const unsigned int version)
{
    bool tflag;
    ar >> boost::serialization::make_nvp("initialized", tflag);
    if (!tflag) {
        t.reset();
        return;
    }

    if (0 == version) {
        boost::serialization::item_version_type item_version(0);
        boost::archive::library_version_type library_version(
            ar.get_library_version()
        );
        if (boost::archive::library_version_type(3) < library_version){
            ar >> BOOST_SERIALIZATION_NVP(item_version);
        }
    }
    if (!t) {
        t.emplace();
    }
    ar >> boost::serialization::make_nvp("value", *t);
}

template<class ArchiveT, class T>
void serialize(ArchiveT & ar, std::optional<T> & t, const unsigned int version)
{
    boost::serialization::split_free(ar, t, version);
}

}

#endif // SONIA_BOOST_SERIALIZATION_OPTIONAL_HPP
