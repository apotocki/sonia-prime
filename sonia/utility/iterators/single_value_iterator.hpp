//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_SIGNLE_VALUE_ITERATOR_HPP
#define SONIA_UTILITY_SIGNLE_VALUE_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <iterator>
#include <utility>

#include "sonia/type_traits.hpp"

namespace sonia {

template <class ValueT, class ReferenceT = add_lvalue_reference_t<remove_reference_t<ValueT>>>
class single_value_iterator
{
public:
    using value_type = remove_reference_t<ValueT>;
    using stored_t = conditional_t<is_reference_v<ValueT>, reference_wrapper<value_type>, value_type>;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using pointer = add_pointer_t<value_type>;
    using reference = ReferenceT;

    single_value_iterator(single_value_iterator const&) = default;
    single_value_iterator(single_value_iterator &&) = default;
    single_value_iterator& operator= (single_value_iterator const&) = default;
    single_value_iterator& operator= (single_value_iterator &&) = default;    

    bool operator== (single_value_iterator const& rhs) const
    {
        if (empty()) return rhs.empty();
        if (rhs.empty()) return false;
        return value_ == rhs.value_;
    }

    bool operator!= (single_value_iterator const& rhs) const
    {
        return !this->operator==(rhs);
    }

    explicit single_value_iterator(ValueT arg)
        : value_{std::move(arg)}, empty_{false}
    {}

    reference operator*() const
    {
        return static_cast<reference>(value_);
    }

    single_value_iterator& operator++()
    {
        empty_ = true;
        return *this;
    }

    single_value_iterator operator++(int)
    {
        single_value_iterator result(*this);
        empty_ = true;
        return std::move(result);
    }

    bool empty() const { return empty_; }

private:
    mutable stored_t value_;
    bool empty_;
};

}

#endif // SONIA_UTILITY_SIGNLE_VALUE_ITERATOR_HPP
