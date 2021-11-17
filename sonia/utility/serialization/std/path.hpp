//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_STD_SERIALIZATION_PATH_HPP
#define SONIA_STD_SERIALIZATION_PATH_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <filesystem>

namespace boost::serialization {

template <class ArchiveT>
void save(ArchiveT & ar, const std::filesystem::path& p, const unsigned int /*version*/)
{
    std::string val = p.string();
    ar << boost::serialization::make_nvp(nullptr, val);
}

template <class ArchiveT>
void load(ArchiveT & ar, std::filesystem::path & p, const unsigned int version)
{
    std::string tmpval;
    ar >> boost::serialization::make_nvp(nullptr, tmpval);
    p = std::filesystem::path(tmpval);
}

template <class ArchiveT>
void serialize(ArchiveT & ar, std::filesystem::path & p, const unsigned int version)
{
    boost::serialization::split_free(ar, p, version);
}

}

#endif // SONIA_STD_SERIALIZATION_PATH_HPP
