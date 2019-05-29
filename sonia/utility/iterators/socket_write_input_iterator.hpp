//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_SOCKET_WRITE_INPUT_ITERATOR_HPP
#define SONIA_UTILITY_SOCKET_WRITE_INPUT_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/utility/iterators/proxy.hpp"

namespace sonia {

// provided buffer is used as a circular data buffer

template <class SocketT, size_t MinRngSzV = 128>
class socket_write_input_iterator
    : public boost::iterator_facade<
          socket_write_input_iterator<SocketT>
        , array_view<char>
        , std::input_iterator_tag
        , wrapper_iterator_proxy<ptr_proxy_wrapper<socket_write_input_iterator<SocketT> const*, array_view<char>>>
    >
{
    using proxy_type = wrapper_iterator_proxy<ptr_proxy_wrapper<socket_write_input_iterator const*, array_view<char>>>;

    friend class boost::iterator_core_access;
    template <class, class> friend class ptr_proxy_wrapper;

    bool equal(socket_write_input_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    proxy_type dereference() const { return proxy_type(this); }

    array_view<char> get_dereference() const
    {
        BOOST_ASSERT (end_ - begin_ >= MinRngSzV);
        return {begin_, end_};
    }

    void set_dereference(array_view<char> span)
    {
        BOOST_ASSERT(span.begin() == begin_);
        end_ = span.end();
    }

    void write(const char * edata)
    {
        expected<size_t, std::error_code> r = psoc_->write_some(array_view<const char>(wrpos_, edata));
        if (r.has_value() && r.value()) {
            wrpos_ += r.value();
            if (wrpos_ == wrend_) {
                wrpos_ = buff_.begin();
                wrend_ = buff_.end();
            }
        } else {
            wrpos_ = nullptr;
            throw eof_exception("socket closed");
        }
    }

    void increment()
    {
        if (begin_ != end_) { // otherwise if set_dereference({begin_, begin_}) was called
            begin_ = end_;
            //write(wrpos_ < begin_ ? begin_ : wrend_);
            //if (begin_ == buff_.end()) begin_ = buff_.begin();
        }

        for (;;) {
            if (begin_ < wrpos_) {
                end_ = wrpos_;
            } else if ((buff_.end() - begin_) >= MinRngSzV) {
                end_ = buff_.end();
                return;
            } else if (begin_ == wrpos_) { // all data was written
                wrpos_ = begin_ = buff_.begin();
                wrend_ = end_ = buff_.end();
            } else { // begin_ > wrpos_
                wrend_ = begin_;
                begin_ = buff_.begin();
                end_ = wrpos_;
            }

            if (end_ - begin_ >= MinRngSzV) break;
            write(wrend_);
        }
    }
    
public:
    explicit socket_write_input_iterator(SocketT & soc, array_view<char> buff) 
        : psoc_(&soc), buff_(buff), begin_(buff.begin()), end_(buff.end()), wrpos_(buff.begin()), wrend_(buff.end())
    {
        if (buff.size() < MinRngSzV) {
            THROW_INTERNAL_ERROR("too small buffer size");
        }
    }

    socket_write_input_iterator(socket_write_input_iterator const&) = delete;
    socket_write_input_iterator(socket_write_input_iterator && rhs) = default;
    socket_write_input_iterator& operator= (socket_write_input_iterator const&) = delete;
    socket_write_input_iterator& operator= (socket_write_input_iterator && rhs) = default;

    bool empty() const { return !wrpos_; }

    void flush()
    {
        BOOST_ASSERT (begin_ != buff_.end());
        BOOST_ASSERT (wrpos_ != wrend_);
        while (wrpos_ != begin_)
        {
            write(wrpos_ < begin_ ? begin_ : wrend_);
        }
        wrpos_ = begin_ = buff_.begin();
        wrend_ = end_ = buff_.end();
    }

private:
    SocketT * psoc_;
    mutable array_view<char> buff_;
    // ready for new data interval descriptor {begin_, end_} or {begin_, buff_.end()}V{buff_.begin(), end_}
    char * begin_, *end_;
    char * wrpos_, *wrend_;
};

}

#endif // SONIA_UTILITY_SOCKET_WRITE_INPUT_ITERATOR_HPP
