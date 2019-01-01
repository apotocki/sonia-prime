//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_HASH_COMBINE_ITERATOR_HPP
#define SONIA_HASH_COMBINE_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <cmath>

#include "sonia/cstdint.hpp"
#include "sonia/type_traits.hpp"

namespace sonia {

class hash_combine_iterator {
public:
    hash_combine_iterator(size_t & val) : value_(&val) {}

    hash_combine_iterator& operator*() { return *this; }
    hash_combine_iterator& operator++() { return *this; }

    template <typename T>
    void operator= (T const& val)
    {
        boost::hash_combine(*value_, val);
    };

    size_t get() const { return *value_; }

private:
    size_t * value_;
};

}

#endif // SONIA_HASH_COMBINE_ITERATOR_HPP
