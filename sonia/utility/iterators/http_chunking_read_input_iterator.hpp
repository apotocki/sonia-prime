//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_HTTP_CHUNKING_READ_INPUT_ITERATOR_HPP
#define SONIA_HTTP_CHUNKING_READ_INPUT_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

namespace sonia {

template <typename ReadInputIteratorT>
class http_chunking_read_input_iterator
    : public boost::iterator_facade<
          http_chunking_read_input_iterator<ReadInputIteratorT>
        , array_view<const char>
        , std::input_iterator_tag
        , wrapper_iterator_proxy<ptr_proxy_wrapper<http_chunking_read_input_iterator<ReadInputIteratorT> const*, array_view<const char>>>
    >
{
    using proxy_type = wrapper_iterator_proxy<ptr_proxy_wrapper<http_chunking_read_input_iterator const*, array_view<const char>>>;

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
        CHUNK,
        STREAM_EOF
    };

    static constexpr size_t modebits = 2;
    mutable size_t chsz_: sizeof(size_t) * CHAR_BIT - modebits;
    mutable size_t mode_: modebits;

    mutable array_view<const char> chunk_;

    char read_char() const
    {
        for (;;) {
            if (chunk_) {
                char r = chunk_.front();
                chunk_.advance_front(1);
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

    void read_delim(char c) const
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

    array_view<const char> get_dereference() const
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
                    mode_ = (size_t)mode::STREAM_EOF;
                    if (!chunk_.empty() && !sonia::empty(base)) {
                        array_view<const char> tmp = *base;
                        *base = array_view<const char>{tmp.begin(), chunk_.begin()};
                    }
                    return {};
                }
                mode_ = (size_t)mode::CHUNK;
            }

            case mode::CHUNK:
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
                        return {chunk_.begin(), sz};
                    }
                }

            //case mode::STREAM_EOF:
            //    return {};

            default:
                THROW_INTERNAL_ERROR("unexpected mode: %1%"_fmt % (int)mode_);
        }
    }

    void set_dereference(array_view<const char> span)
    {
        BOOST_ASSERT ((mode)mode_ == mode::CHUNK);
        BOOST_ASSERT (span.is_subset_of(chunk_));

        array_view<const char> tmp = *base;
        *base = array_view<const char>{tmp.begin(), span.end()};
        chunk_ = array_view<const char>{chunk_.begin(), span.end()};
    }

    void increment()
    {
        //if ((mode)mode_ == mode::STREAM_EOF) return;
        BOOST_ASSERT ((mode)mode_ == mode::CHUNK);
        auto sz = (std::min)(chsz_, chunk_.size());
        chsz_ -= sz;
        chunk_.advance_front(sz);
        if (0 == chsz_) {
            read_delim(read_char());
            mode_ = (size_t)mode::CHUNK_SIZE;
            get_dereference(); // to test on eof
            return;
        }
    }

public:
    http_chunking_read_input_iterator() = default;
    explicit http_chunking_read_input_iterator(ReadInputIteratorT base_it) : base(std::move(base_it)), mode_(0) {}
    
    bool empty() const
    {
        return (mode)mode_ == mode::STREAM_EOF;
    }

    mutable ReadInputIteratorT base;
};

}

#endif // SONIA_HTTP_CHUNKING_READ_INPUT_ITERATOR_HPP
