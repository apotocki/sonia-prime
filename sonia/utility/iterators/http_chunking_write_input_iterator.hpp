//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/sal.hpp"
#include "sonia/span.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

namespace sonia {

template <typename WriteInputIteratorT>
class http_chunking_write_input_iterator
    : public boost::iterator_facade<
          http_chunking_write_input_iterator<WriteInputIteratorT>
        , std::span<char>
        , std::input_iterator_tag
        , wrapper_iterator_proxy<ptr_proxy_wrapper<http_chunking_write_input_iterator<WriteInputIteratorT> const*, std::span<char>>>
    >
{
    using proxy_type = wrapper_iterator_proxy<ptr_proxy_wrapper<http_chunking_write_input_iterator const*, std::span<char>>>;

    friend class boost::iterator_core_access;
    template <class, class> friend class ptr_proxy_wrapper;

    bool equal(http_chunking_write_input_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    proxy_type dereference() const { return proxy_type(this); }

    std::span<char> get_chunk() const
    {
        BOOST_ASSERT(!empty());
        std::span<char> chunk = *base;
        uint8_t max_hex_digit_count = get_max_hex_digit_count(chunk.size() - 5); // lets suppose 1 digit for size, 2 for prolog "\r\n" and 2 for epilog "\r\n"
        BOOST_ASSERT (chunk.size() > max_hex_digit_count + 4); // size digits + 2 for prolog "\r\n" + 2 for epilog "\r\n"
        chunk = chunk.subspan(max_hex_digit_count + 2); // size digits + 2 for prolog "\r\n"
        chunk = advance_back(chunk, -2); // 2 for epilog "\r\n"
        return chunk;
    }

    std::span<char> get_dereference() const
    {
        if (chunk_.empty()) {
            max_hex_digit_count_ = 0;
            chunk_ = get_chunk();
        }
        return chunk_;
    }

    void set_dereference(std::span<char> span)
    {
        BOOST_ASSERT (is_subset_of(span, chunk_));
        chunk_ = span;
    }

    void flush(std::span<char> val)
    {
        size_t chunksz = val.size();
        char * buffpos = val.data() - max_hex_digit_count_ - 2;
        for (uint8_t d = max_hex_digit_count_; d > 0; --d) {
            uint8_t dval = (0xf & (chunksz >> 4 * (d - 1)));
            *buffpos++ = dval < 10 ? '0' + dval : 'A' + dval - 10;
        }
        *buffpos++ = '\r'; *buffpos++ = '\n';
        BOOST_ASSERT (buffpos == val.data());
        buffpos = data_end(val);
        *buffpos++ = '\r'; *buffpos++ = '\n';
        *base = std::span{val.data() - max_hex_digit_count_ - 2, buffpos};
        ++base;
    }

    void increment()
    {
        std::span<char> orig  = get_chunk();
        if (chunk_.end() != orig.end()) {
            chunk_ = std::span{data_end(chunk_), data_end(orig)};
        } else {
            flush(orig);
            chunk_ = {};
        }
    }

    int8_t get_max_hex_digit_count(size_t sz) const
    {
        if (!max_hex_digit_count_) {
            max_hex_digit_count_ = (sal::log2(sz) / 4) + 1;
        }
        return  max_hex_digit_count_;
    }

    mutable int8_t max_hex_digit_count_{0};
    mutable std::span<char> chunk_;

public:
    explicit http_chunking_write_input_iterator(WriteInputIteratorT base_it) : base(std::move(base_it)) {}
    
    bool empty() const
    {
        return sonia::empty(base);
    }

    void flush()
    {
        if (!chunk_.empty()) {
            std::span<char> orig  = get_chunk();
            flush({orig.data(), chunk_.data()});
            chunk_ = {};
        }
        if constexpr (iterators::has_method_flush_v<WriteInputIteratorT, void()>) {
            base.flush();
        }
    }

    void close()
    {
        flush();
        if (empty()) return;
        std::span<char> chunk = *base;
        BOOST_ASSERT (chunk.size() >= 5);
        const char * estr = "0\r\n\r\n";
        std::memcpy(chunk.data(), estr, 5);
        *base = std::span<char>{chunk.data(), 5};
        ++base;
        if constexpr (iterators::has_method_close_v<WriteInputIteratorT, void()>) {
            base.close();
        }
    }

    WriteInputIteratorT base;
};

}
