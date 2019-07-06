//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_REFERENCE_WRAPPER_ITERATOR_HPP
#define SONIA_UTILITY_REFERENCE_WRAPPER_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/iterator_traits.hpp"

namespace sonia {

template <typename IteratorT>
class reference_wrapper_iterator
{
public:
    using base_iterator = IteratorT;
    using value_type = iterator_value_t<IteratorT>;
    using pointer = iterator_pointer_t<IteratorT>;
    using reference = iterator_reference_t<IteratorT>;
    using difference_type = iterator_difference_t<IteratorT>;

    using base_iterator_category = iterator_category_t<IteratorT>;
    using iterator_category = conditional_t<
        is_base_of_v<std::output_iterator_tag, base_iterator_category>,
        base_iterator_category,
        std::input_iterator_tag
    >;

    reference_wrapper_iterator() : base_(nullptr) {}
    explicit reference_wrapper_iterator(IteratorT & it) : base_(std::addressof(it)) {}

    IteratorT & base() { return *base_; }
    IteratorT const& base() const { return *base_; }

    bool operator== (reference_wrapper_iterator const& rhs) const { return base() == rhs.base(); }
    bool operator!= (reference_wrapper_iterator const& rhs) const { return base() != rhs.base(); }
    decltype(auto) operator*() const { return **base_; }

    reference_wrapper_iterator & operator++() { ++base(); return *this; }
    reference_wrapper_iterator operator++(int) = delete;

    reference_wrapper_iterator & operator--() { return --base(); }
    reference_wrapper_iterator operator--(int) = delete;

    bool empty() const
    {
        if (!base_) return true;
        return sonia::empty(*base_);
    }

    template <typename T = IteratorT>
    enable_if_t<iterators::has_method_flush_v<T, void()>> flush() { base_->flush(); }

    template <typename T = IteratorT>
    enable_if_t<iterators::has_method_close_v<T, void()>> close() { base_->close(); }

private:
    IteratorT * base_;
};

}

#endif // SONIA_UTILITY_REFERENCE_WRAPPER_ITERATOR_HPP
