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
        BOOST_ASSERT(wrpos_);
        BOOST_ASSERT (end_ - begin_ >= MinRngSzV);
        return {begin_, end_};
    }

    void set_dereference(array_view<char> span)
    {
        BOOST_ASSERT(wrpos_);
        BOOST_ASSERT(span.begin() == begin_);
        end_ = span.end();
    }

    void write() noexcept
    {
        BOOST_ASSERT (wrpos_ != wrend_);
        auto r = psoc_->write_some(array_view<const char>{wrpos_, wrend_});
        if (r.has_value() && r.value()) {
            wrpos_ += r.value();
        } else {
            wrpos_ = nullptr;
        }
    }

    void increment()
    {
        BOOST_ASSERT(wrpos_);
        if (begin_ != end_) { // otherwise if set_dereference({begin_, begin_}) was called
            begin_ = end_;
            if (begin_ > wrpos_) {
                wrend_ = begin_;
            }
        }

        do {
            if (wrpos_ == wrend_) { // all data was written
                wrpos_ = wrend_ = begin_ = buff_.begin();
                end_ = buff_.end();
                return;
            } else if (begin_ < wrpos_) {
                end_ = wrpos_;
                if (end_ - begin_ >= MinRngSzV) return;
                write();
                continue;
            } else if ((buff_.end() - begin_) >= MinRngSzV) {
                end_ = buff_.end();
                return;
            } else { // (buff_.end() - begin_) < MinRngSzV
                write();
                begin_ = buff_.begin();
                continue;
            }
        } while (wrpos_);
    }
    
public:
    socket_write_input_iterator() : wrpos_(nullptr) {}

    explicit socket_write_input_iterator(SocketT & soc, array_view<char> buff) 
        : psoc_(&soc), buff_(buff), begin_(buff.begin()), end_(buff.end()), wrpos_(buff.begin()), wrend_(buff.begin())
    {
        if (buff.size() < MinRngSzV) {
            THROW_INTERNAL_ERROR("too small buffer size");
        }
    }

    socket_write_input_iterator(socket_write_input_iterator const&) = delete;
    socket_write_input_iterator(socket_write_input_iterator && rhs) = default;
    socket_write_input_iterator& operator= (socket_write_input_iterator const&) = delete;
    socket_write_input_iterator& operator= (socket_write_input_iterator && rhs) = default;

    ~socket_write_input_iterator() noexcept
    {
        while (wrpos_ && wrpos_ != wrend_)
        {
            write();
        }
    }

    bool empty() const { return !wrpos_; }

    void flush()
    {
        while (wrpos_ && wrpos_ != wrend_)
        {
            write();
        }
        wrpos_ = wrend_ = begin_ = buff_.begin();
        end_ = buff_.end();
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
