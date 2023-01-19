//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/span.hpp"
#include "sonia/iterator_traits.hpp"

namespace sonia {

template <typename WriteOutputIteratorT>
class buffering_write_input_iterator
    : public boost::iterator_facade<
          buffering_write_input_iterator<WriteOutputIteratorT>
        , std::span<char>
        , std::input_iterator_tag
        , std::span<char> &
    >
{
    friend class boost::iterator_core_access;
    
    bool equal(buffering_write_input_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    std::span<char> & dereference() const
    {
        return span_;
    }

    void increment()
    {
        if (span_.end() != buffer_.end()) {
            span_ = {data_end(span_), data_end(buffer_)};
        } else {
            *base = buffer_;
            ++base;
            span_ = buffer_;
        }
    }

    mutable std::span<char> span_;
    std::span<char> buffer_;

public:
    buffering_write_input_iterator(WriteOutputIteratorT it, std::span<char> buff)
        : base{std::move(it)}, buffer_{buff}, span_{buff}
    {}

    bool empty() const
    {
        return sonia::empty(base);
    }

    void flush()
    {
        *base = std::span<char>{buffer_.data(), span_.data()};
        ++base;
        if constexpr (iterators::has_method_flush_v<WriteOutputIteratorT, void()>) {
            base.flush();
        }
        span_ = buffer_;
    }

    void close()
    {
        flush();
        if constexpr (iterators::has_method_close_v<WriteOutputIteratorT, void()>) {
            base.close();
        }
    }

    WriteOutputIteratorT base;
};

}
