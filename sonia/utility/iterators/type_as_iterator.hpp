//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_TYPE_AS_ITERATOR_HPP
#define SONIA_UTILITY_TYPE_AS_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>

namespace sonia {

template <typename T, typename DifferenceT = std::ptrdiff_t>
class type_as_iterator
    : public boost::iterator_facade<
          type_as_iterator<T>
        , T
        , std::random_access_iterator_tag
        , T const&
    >
{
    friend class boost::iterator_core_access;
    
    bool equal(type_as_iterator const& rhs) const { return value == rhs.value; }
    
    T const& dereference() const { return value; }

    void increment() { ++value; }
    void decrement() { --value; }
    void advance(DifferenceT dif) { value += dif; }
    DifferenceT distance_to(type_as_iterator const& rhs) const { return (DifferenceT)(rhs.value - value); }

public:
    type_as_iterator() = default;

    template <typename ArgT>
    explicit type_as_iterator(ArgT && val) : value(std::forward<ArgT>(val)) {}

    T value;
};

}

#endif // SONIA_UTILITY_TYPE_AS_ITERATOR_HPP
