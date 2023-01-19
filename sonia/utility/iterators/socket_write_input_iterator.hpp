//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/span.hpp"
#include "sonia/exceptions.hpp"

namespace sonia {

// provided buffer is used as a circular data buffer

template <class SocketT/*, size_t MinRngSzV = 128*/>
class socket_write_input_iterator
    : public boost::iterator_facade<
          socket_write_input_iterator<SocketT>
        , std::span<char>
        , std::input_iterator_tag
        , std::span<char>&
    >
{
    friend class boost::iterator_core_access;
    template <class, class> friend class ptr_proxy_wrapper;

    bool equal(socket_write_input_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    std::span<char> & dereference() const
    {
        if (!wrpos_) {
            throw closed_exception();
        }
        return value_;
    }

    void write() noexcept
    {
        BOOST_ASSERT (wrpos_ != wrend_);
        auto r = psoc_->write_some(std::span<const char>{wrpos_, wrend_});
        if (r.has_value() && r.value()) {
            wrpos_ += r.value();
        } else {
            wrpos_ = nullptr;
        }
    }

    std::span<char> available_buffer()
    {
        if (value_.data() >= wrend_) {
            return {wrend_, data_end(buff_)};
        } else {
            return {buff_.data(), wrpos_};
        }
    }

    void increment()
    {
        if (!wrpos_) {
            throw closed_exception();
        }
        std::span<char> avlbf = available_buffer();

        if (value_.end() != avlbf.end()) {
            value_ = {value_.end(), avlbf.end()};
        } else {
            if (value_.data() >= wrend_) {
                wrend_ = value_.data() + value_.size();
            }
            write();
            if (wrpos_) {
                value_ = {buff_.data(), wrpos_};
                if (wrpos_ == wrend_) {
                    wrpos_ = wrend_ = buff_.data();
                }
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

    explicit socket_write_input_iterator(SocketT & soc, std::span<char> buff)
        : psoc_(&soc), buff_{buff}, value_{buff}, wrpos_{buff.data()}, wrend_{buff.data()}
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
        if (value_.data() < wrend_) {
            while (wrpos_ && wrpos_ != wrend_)
            {
                write();
            }
            if (wrpos_) wrpos_ = buff_.data();
        }
        
        wrend_ = value_.data();
        
        while (wrpos_ && wrpos_ != wrend_)
        {
            write();
        }
        if (wrpos_) {
            wrpos_ = wrend_ = buff_.data();
            value_ = buff_;
        }
    }

private:
    SocketT * psoc_;
    mutable std::span<char> buff_;
    // ready for new data interval descriptor {begin_, end_} or {begin_, buff_.end()}V{buff_.begin(), end_}
    mutable std::span<char> value_;
    char * wrpos_, *wrend_;
};

}
