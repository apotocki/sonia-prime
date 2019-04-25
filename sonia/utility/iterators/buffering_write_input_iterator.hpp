//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_BUFFERING_WRITE_INPUT_ITERATOR_HPP
#define SONIA_UTILITY_BUFFERING_WRITE_INPUT_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/iterator_traits.hpp"

namespace sonia {

template <typename WriteOutputIteratorT>
class buffering_write_input_iterator
    : public boost::iterator_facade<
          buffering_write_input_iterator<WriteOutputIteratorT>
        , array_view<char>
        , std::input_iterator_tag
        , array_view<char> &
    >
{
    friend class boost::iterator_core_access;
    
    bool equal(buffering_write_input_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    array_view<char> & dereference() const
    {
        return span_;
    }

    void increment()
    {
        *base = span_;
        ++base;
        span_ = buffer_;
    }

    mutable array_view<char> span_;
    array_view<char> buffer_;

public:
    buffering_write_input_iterator(WriteOutputIteratorT it, array_view<char> buff)
        : base{std::move(it)}, buffer_{buff}, span_{buff}
    {}

    bool empty() const
    {
        return sonia::empty(base);
    }

    template <typename T = WriteOutputIteratorT>
    enable_if_t<iterators::has_method_close_v<T, void()>> close() { base.close(); }

    template <typename T = WriteOutputIteratorT>
    enable_if_t<iterators::has_method_flush_v<T, void()>> flush() { base.flush(); }

    WriteOutputIteratorT base;
};

}

#endif // SONIA_UTILITY_BUFFERING_WRITE_INPUT_ITERATOR_HPP
