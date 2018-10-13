//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_CACHES_CAR_HPP
#define SONIA_CACHES_CAR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

// based on 
// CAR: Clock with Adaptive Replacement
// https://dl.acm.org/citation.cfm?id=1096699

namespace sonia { namespace cashe {

/*
class handle {
car * cache;
ValueT value;

public:
ValueT & value() { return value; }
~handle() {
cache->release()
}
};

*/
template <typename KeyT, typename ValueT>
class car {
public:

};

}}

#endif // SONIA_CACHES_CAR_HPP