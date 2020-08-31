//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_BOOST_SERIALIZATION_OPTIMIZED_HPP
#define SONIA_BOOST_SERIALIZATION_OPTIMIZED_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <sstream>

#include "sonia/exceptions.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/utility/optimized/array.hpp"

namespace boost::serialization {

template <class ArchiveT, typename ElementT, size_t ByteSzV, typename RefCountT>
void save(ArchiveT & ar, sonia::shared_optimized_array<ElementT, ByteSzV, RefCountT> const& jv, const unsigned int /*version*/)
{
    if constexpr (std::is_same_v<char, remove_cv_t<ElementT>>) {
        auto str = sonia::to_string(sonia::string_view(jv.to_array_view()));
        ar << boost::serialization::make_nvp(nullptr, str);
    } else {
        size_t sz = jv.size();
        ar << boost::serialization::make_nvp("size", sz);
        for (auto & v : jv) {
            ar << boost::serialization::make_nvp("e", v);
        }
    }
}

template <class ArchiveT, typename ElementT, size_t ByteSzV, typename RefCountT>
void load(ArchiveT & ar, sonia::shared_optimized_array<ElementT, ByteSzV, RefCountT>& t, const unsigned int version)
{
    if constexpr (std::is_same_v<char, remove_cv_t<ElementT>>) {
        std::string value;
        ar >> boost::serialization::make_nvp(nullptr, value);
        t = sonia::shared_optimized_array<ElementT, ByteSzV, RefCountT>(sonia::to_string_view(value));
    } else {
        size_t sz;
        ar >> boost::serialization::make_nvp("size", sz);
        t = sonia::shared_optimized_array<ElementT, ByteSzV, RefCountT>(sz);
        for (size_t i = 0; i < sz; ++i) {
            ar >> boost::serialization::make_nvp("e", const_cast<remove_cv_t<ElementT>&>(t[i]));
        }
    }
}

template <class ArchiveT, typename ElementT, size_t ByteSzV, typename RefCountT>
void serialize(ArchiveT & ar, sonia::shared_optimized_array<ElementT, ByteSzV, RefCountT>& t, const unsigned int version)
{
    boost::serialization::split_free(ar, t, version);
}

}

#endif // SONIA_BOOST_SERIALIZATION_OPTIMIZED_HPP
