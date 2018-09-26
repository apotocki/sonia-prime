//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_RANGE_HASHER_HPP
#define SONIA_UTILITY_RANGE_HASHER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

namespace sonia {

struct range_hasher {
    template <class RangeT>
    size_t operator()(RangeT const& r) const noexcept {
        return boost::hash_range(boost::begin(r), boost::end(r));
    }
};

}

#endif // SONIA_UTILITY_RANGE_HASHER_HPP
