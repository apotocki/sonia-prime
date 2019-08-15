//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_VALUE_AS_ITERATOR_HPP
#define SONIA_UTILITY_VALUE_AS_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include <boost/iterator/iterator_facade.hpp>

namespace sonia {

template <typename T, typename DifferenceT = std::ptrdiff_t>
class value_as_iterator
    : public boost::iterator_facade<
          value_as_iterator<T>
        , T
        , std::random_access_iterator_tag
    >
{
    friend class boost::iterator_core_access;
    
    bool equal(value_as_iterator const& rhs) const { return value_ == rhs.value_; }
    
    T & dereference() const { return value_; }

    void increment() { ++value_; }
    void decrement() { --value_; }
    void advance(DifferenceT dif) { value_ += dif; }
    DifferenceT distance_to(value_as_iterator const& rhs) const { return (DifferenceT)(rhs.value_ - value_); }

public:
    value_as_iterator() = default;

    template <typename ArgT>
    explicit value_as_iterator(ArgT && val) : value_(std::forward<ArgT>(val)) {}

private:
    mutable remove_cv_t<T> value_;
};

}

#endif // SONIA_UTILITY_VALUE_AS_ITERATOR_HPP
