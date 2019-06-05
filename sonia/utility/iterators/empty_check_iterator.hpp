//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_EMPTY_CHECK_ITERATOR_HPP
#define SONIA_UTILITY_EMPTY_CHECK_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/iterator_traits.hpp"
#include "sonia/exceptions.hpp"

namespace sonia {

template <typename IteratorT>
class empty_check_iterator
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

    empty_check_iterator() = default;
    explicit empty_check_iterator(IteratorT it) : base_{std::move(it)} {}
    
    template <typename ... ArgsT>
    empty_check_iterator(in_place_t, ArgsT&& ... args) : base_{std::forward<ArgsT>(args)...} {}

    IteratorT & base() { return base_; }
    IteratorT const& base() const { return base_; }

    IteratorT & not_empty_base()
    {
        if (BOOST_UNLIKELY(empty())) throw eof_exception();
        return base_;
    }

    IteratorT const& not_empty_base() const
    {
        if (BOOST_UNLIKELY(empty())) throw eof_exception();
        return base_;
    }

    bool operator== (empty_check_iterator const& rhs) const { return base() == rhs.base(); }
    bool operator!= (empty_check_iterator const& rhs) const { return base() != rhs.base(); }
    decltype(auto) operator*() const
    {
        return *not_empty_base();
    }

    empty_check_iterator & operator++() { ++not_empty_base(); return *this; }
    empty_check_iterator operator++(int) { return empty_check_iterator{not_empty_base()++}; }

    empty_check_iterator & operator--() { return --base(); }
    empty_check_iterator operator--(int) { return empty_check_iterator{not_empty_base()--}; }

    bool empty() const
    {
        return sonia::empty(base_);
    }

    template <typename T = IteratorT>
    enable_if_t<iterators::has_method_flush_v<T, void()>> flush() { base_.flush(); }

    template <typename T = IteratorT>
    enable_if_t<iterators::has_method_close_v<T, void()>> close() { base_.close(); }

private:
    IteratorT base_;
};

}

#endif // SONIA_UTILITY_EMPTY_CHECK_ITERATOR_HPP
