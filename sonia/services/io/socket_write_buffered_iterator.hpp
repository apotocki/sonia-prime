//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_TCP_WRITE_INPUT_ITERATOR_HPP
#define SONIA_IO_TCP_WRITE_INPUT_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>
#include "sonia/exceptions.hpp"
#include "sonia/utility/iterators/proxy.hpp"

namespace sonia {

template <class SocketT>
class socket_write_buffered_iterator
    : public boost::iterator_facade<
          socket_write_buffered_iterator
        , array_view<char>
        , std::input_iterator_tag
        , wrapper_iterator_proxy<ptr_proxy_wrapper<socket_write_buffered_iterator const*, array_view<char>>>
    >
{
    using proxy_type = wrapper_iterator_proxy<ptr_proxy_wrapper<socket_write_buffered_iterator const*, array_view<char>>>;

    friend class boost::iterator_core_access;
    template <class, class> friend class ptr_proxy_wrapper;

    bool equal(tcp_write_input_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    proxy_type dereference() const { return proxy_type(this); }

    array_view<char> get_dereference() const
    {
        return {begin_, end_};
        if (begin_ < wrpos_) return {begin_, wrpos_};
        return {begin_, buff_.end()};
    }

    void set_dereference(array_view<char> span)
    {
        BOOST_ASSERT(span.begin() == begin_);
        end_ = span_.end();
    }

    void increment()
    {
        if (begin_ != end_) {
            begin_ = end_;
            size_t sz = psoc_->write_some({wrpos_, wrpos_ < begin_ ? begin_ : buff_.end()});
            if (sz) {
                wrpos_ += sz;
            } else {
                wrpos_ = nullptr;
                return;
            }
            if (begin_ == buffer_.end()) begin_ = buffer_.begin();
            if (wrpos_ == buffer_.end()) wrpos_ = buffer_.begin();
        }
        if (begin_ < wrpos_) {
            end_= wrpos_;
        } else {
            end_ = buffer_.end();
        }
    }
    
    void flush()
    {
        while (wrpos_ != begin_)
        increment();
    }

public:
    explicit socket_write_buffered_iterator(SocketT & soc, array_view<char> buff) 
        : psoc_(&soc), buff_(buff), begin_(buff.begin()), end_(buff_.end()), wrpos_(buff.begin())
    {}

    socket_write_buffered_iterator(socket_write_buffered_iterator const&) = delete;
    socket_write_buffered_iterator(socket_write_buffered_iterator && rhs) = default;
    socket_write_buffered_iterator& operator= (socket_write_buffered_iterator const&) = delete;
    socket_write_buffered_iterator& operator= (socket_write_buffered_iterator && rhs) = default;

    bool empty() const { return !wrpos_; }

private:
    SocketT * psoc_;
    mutable array_view<char> buff_;
    // ready for new data interval descriptor {begin_, end_} or {begin_, buff_.end()}V{buff_.begin(), end_}
    char * begin_, *end_;
    char * wrpos_;
};

}

#endif // SONIA_IO_TCP_WRITE_INPUT_ITERATOR_HPP
