//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/span.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"
#include "sonia/utility/parsers/utility.hpp"

namespace sonia {

template <typename ReadInputIteratorT>
class http_chunking_read_input_iterator
    : public boost::iterator_facade<
          http_chunking_read_input_iterator<ReadInputIteratorT>
        , std::span<const char>
        , not_more_traversal_t<forward_traversal_tag, iterator_traversal_t<ReadInputIteratorT>>
        , wrapper_iterator_proxy<ptr_proxy_wrapper<http_chunking_read_input_iterator<ReadInputIteratorT> const*, std::span<const char>>>
    >
{
    using proxy_type = wrapper_iterator_proxy<ptr_proxy_wrapper<http_chunking_read_input_iterator const*, std::span<const char>>>;

    friend class boost::iterator_core_access;
    template <class, class> friend class ptr_proxy_wrapper;

    bool equal(http_chunking_read_input_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    proxy_type dereference() const { return proxy_type(this); }

    enum class mode {
        START = 0,
        CHUNK_SIZE,
        STREAM_EOF,
        VALUE_READY
    };

    static constexpr size_t modebits = 2;
    mutable size_t chsz_: sizeof(size_t) * CHAR_BIT - modebits;
    mutable size_t mode_: modebits;

    std::span<const char> chunk_;
    std::span<const char> value_;

    char read_char()
    {
        for (;;) {
            if (!chunk_.empty()) {
                char r = chunk_.front();
                chunk_ = chunk_.subspan(1);
                return r;
            }
            ++base;
            if (sonia::empty(base)) {
                mode_ = (size_t)mode::STREAM_EOF;
                throw eof_exception("unexpected");
            }
            chunk_ = *base;
        }
    }

    void read_delim(char c)
    {
        if (c != '\r') {
            mode_ = (size_t)mode::STREAM_EOF;
            throw exception("wrong chunk size delimiter %1%(\\r is expected)"_fmt % c);
        }
        c = read_char();
        if (c != '\n') {
            mode_ = (size_t)mode::STREAM_EOF;
            throw exception("wrong chunk size delimiter %1%(\\n is expected)"_fmt % c);
        }
    }

    std::span<const char> get_dereference() const
    {
        BOOST_ASSERT ((mode)mode_ == mode::VALUE_READY);
        return value_;
    }

    void set_dereference(std::span<const char> span)
    {
        BOOST_ASSERT ((mode)mode_ == mode::VALUE_READY);
        //BOOST_ASSERT (span.is_subset_of(value_));
        value_ = span;
    }

    void proc()
    {
        switch ((mode)mode_)
        {
            case mode::START:
                if (sonia::empty(base)) {
                    mode_ = (size_t)mode::STREAM_EOF;
                    throw eof_exception("unexpected");
                }
                chunk_ = *base;
                mode_ = (size_t)mode::CHUNK_SIZE;

            case mode::CHUNK_SIZE:
            {
                chsz_ = 0;
                char c;
                for (;;) {
                    c = read_char();
                    uint8_t hd = parsers::get_hexdigit(c);
                    if (hd == 0xff) break;
                    chsz_ = chsz_ * 16 + hd;
                }
                read_delim(c);
                if (0 == chsz_) {
                    read_delim(read_char());
                    if (!sonia::empty(base)) {
                        if (chunk_.empty()) {
                            ++base;
                        } else {
                            *base = chunk_;
                        }
                    }
                    mode_ = (size_t)mode::STREAM_EOF;
                    break;
                }
            }

            case mode::VALUE_READY:
                for (;;) {
                    auto sz = (std::min)(chsz_, chunk_.size());
                    if (0 == sz) {
                        ++base;
                        if (sonia::empty(base)) {
                            mode_ = (size_t)mode::STREAM_EOF;
                            throw eof_exception("unexpected");
                        }
                        chunk_ = *base;
                    } else {
                        value_ = {chunk_.begin(), sz};
                        mode_ = (size_t)mode::VALUE_READY;
                        return;
                    }
                }
            default:
                THROW_INTERNAL_ERROR("unexpected mode: %1%"_fmt % (int)mode_);
        }
    }

    void increment()
    {
        BOOST_ASSERT ((mode)mode_ == mode::VALUE_READY);
        auto sz = (std::min)(chsz_, chunk_.size());
        if (value_.end() < chunk_.begin() + sz) {
            value_ = std::span{data_end(value_), chunk_.data() + sz};
        } else {
            chsz_ -= sz;
            chunk_ = chunk_.subspan(sz);
            if (0 == chsz_) {
                read_delim(read_char());
                mode_ = (size_t)mode::CHUNK_SIZE;
            }
            proc();
        }
    }

public:
    http_chunking_read_input_iterator() : mode_{(size_t)mode::STREAM_EOF} {}

    explicit http_chunking_read_input_iterator(ReadInputIteratorT base_it)
        : base{std::move(base_it)}, mode_{0}
    {
        proc();
    }
    
    bool empty() const
    {
        return (mode)mode_ == mode::STREAM_EOF;
    }

    mutable ReadInputIteratorT base;
};

}
