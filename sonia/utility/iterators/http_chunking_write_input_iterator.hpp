//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_HTTP_CHUNKING_WRITE_INPUT_ITERATOR_HPP
#define SONIA_HTTP_CHUNKING_WRITE_INPUT_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/sal.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

namespace sonia {

template <typename WriteInputIteratorT>
class http_chunking_write_input_iterator
    : public boost::iterator_facade<
          http_chunking_write_input_iterator<WriteInputIteratorT>
        , array_view<char>
        , std::input_iterator_tag
        , wrapper_iterator_proxy<ptr_proxy_wrapper<http_chunking_write_input_iterator<WriteInputIteratorT> const*, array_view<char>>>
    >
{
    using proxy_type = wrapper_iterator_proxy<ptr_proxy_wrapper<http_chunking_write_input_iterator const*, array_view<char>>>;

    friend class boost::iterator_core_access;
    template <class, class> friend class ptr_proxy_wrapper;

    bool equal(http_chunking_write_input_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    proxy_type dereference() const { return proxy_type(this); }

    array_view<char> get_dereference() const
    {
        if (chunk_.empty()) {
            chunk_ = *base;
            uint8_t max_hex_digit_count = get_max_hex_digit_count(chunk_.size() - 3);
            BOOST_ASSERT (chunk_.size() > max_hex_digit_count + 4);
            chunk_.advance_front(max_hex_digit_count + 2); // +reserved \r\n
            chunk_.advance_back(-2);
        }
        return chunk_;
    }

    void set_dereference(array_view<char> span)
    {
        BOOST_ASSERT (max_hex_digit_count_);
        BOOST_ASSERT (span.begin() == chunk_.begin());
        chunk_ = span;
    }

    void increment()
    {
        BOOST_ASSERT (max_hex_digit_count_);
        size_t chunksz = chunk_.size();
        char * buffpos = chunk_.begin() - max_hex_digit_count_ - 2;
        for (uint8_t d = max_hex_digit_count_; d > 0; --d) {
            uint8_t dval = (0xf & (chunksz >> 4 * (d - 1)));
            *buffpos++ = dval < 10 ? '0' + dval : 'A' + dval - 10;
        }
        *buffpos++ = '\r'; *buffpos++ = '\n';
        BOOST_ASSERT (buffpos == chunk_.begin());
        buffpos = chunk_.end();
        *buffpos++ = '\r'; *buffpos++ = '\n';
        *base = array_view(chunk_.begin() - max_hex_digit_count_ - 2, buffpos);
        ++base;
        chunk_.reset();
        max_hex_digit_count_ = 0;
    }

    int8_t get_max_hex_digit_count(size_t sz) const
    {
        if (!max_hex_digit_count_) {
            max_hex_digit_count_ = (sal::log2(sz) / 4) + 1;
        }
        return  max_hex_digit_count_;
    }

    mutable int8_t max_hex_digit_count_{0};
    mutable array_view<char> chunk_;

public:
    explicit http_chunking_write_input_iterator(WriteInputIteratorT base_it) : base(std::move(base_it)) {}
    
    //~http_chunking_write_input_iterator()
    //{
    //    try {
    //        close();
    //    } catch (...) { /* ignore*/ }
    //}

    bool empty() const
    {
        return sonia::empty(base);
    }

    void close()
    {
        if (empty()) return;
        array_view<char> chunk = *base;
        BOOST_ASSERT (chunk.size() >= 5);
        const char * estr = "0\r\n\r\n";
        std::memcpy(chunk.begin(), estr, 5);
        *base = array_view<char>{chunk.begin(), 5};
        ++base;
        if constexpr (iterators::has_method_flush_v<WriteInputIteratorT, void()>) {
            base.flush();
        }
    }

    template <typename T = WriteInputIteratorT>
    enable_if_t<iterators::has_method_flush_v<T, void()>> flush() { base.flush(); }

    WriteInputIteratorT base;
};

}

#endif // SONIA_HTTP_CHUNKING_WRITE_INPUT_ITERATOR_HPP
