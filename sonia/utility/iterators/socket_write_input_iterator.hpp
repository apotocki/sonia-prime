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

namespace sonia {

// provided buffer is used as a circular data buffer

template <class SocketT/*, size_t MinRngSzV = 128*/>
class socket_write_input_iterator
    : public boost::iterator_facade<
          socket_write_input_iterator<SocketT>
        , array_view<char>
        , std::input_iterator_tag
        , array_view<char>&
    >
{
    friend class boost::iterator_core_access;
    template <class, class> friend class ptr_proxy_wrapper;

    bool equal(socket_write_input_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    array_view<char> & dereference() const
    {
        BOOST_ASSERT(wrpos_);
        return value_;
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

    array_view<char> available_buffer()
    {
        if (value_.begin() >= wrend_) {
            return {wrend_, buff_.end()};
        } else {
            return {buff_.begin(), wrpos_};
        }
    }

    void increment()
    {
        BOOST_ASSERT(wrpos_);
        array_view<char> avlbf = available_buffer();

        if (value_.end() != avlbf.end()) {
            value_ = {value_.end(), avlbf.end()};
        } else {
            if (value_.begin() >= wrend_) {
                wrend_ = value_.end();
            }
            write();
            value_ = {buff_.begin(), wrpos_};
            if (wrpos_ == wrend_) {
                wrpos_ = wrend_ = buff_.begin();
            }
        }
#if 0
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
#endif
    }
    
public:
    socket_write_input_iterator() : wrpos_(nullptr) {}

    explicit socket_write_input_iterator(SocketT & soc, array_view<char> buff) 
        : psoc_(&soc), buff_{buff}, value_{buff}, wrpos_(buff.begin()), wrend_(buff.begin())
    {
        //if (buff.size() < MinRngSzV) {
        //    THROW_INTERNAL_ERROR("too small buffer size");
        //}
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

    void close()
    {
        flush();
    }

    void flush()
    {
        if (value_.begin() < wrend_) {
            while (wrpos_ && wrpos_ != wrend_)
            {
                write();
            }
            if (wrpos_) wrpos_ = buff_.begin();
        }
        
        wrend_ = value_.begin();
        
        while (wrpos_ && wrpos_ != wrend_)
        {
            write();
        }
        if (wrpos_) {
            wrpos_ = wrend_ = buff_.begin();
            value_ = buff_;
        }
    }

private:
    SocketT * psoc_;
    mutable array_view<char> buff_;
    // ready for new data interval descriptor {begin_, end_} or {begin_, buff_.end()}V{buff_.begin(), end_}
    mutable array_view<char> value_;
    char * wrpos_, *wrend_;
};

}

#endif // SONIA_UTILITY_SOCKET_WRITE_INPUT_ITERATOR_HPP
