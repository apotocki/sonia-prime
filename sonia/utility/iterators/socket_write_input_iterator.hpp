//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/span.hpp"
#include "sonia/exceptions.hpp"

namespace sonia {

// provided buffer is used as a circular data buffer

template <class SocketT, size_t MinInputRngSzV = 128, size_t MinSocketWriteRngSzV = 1024>
class socket_write_input_iterator
{
public:
    using value_type = std::span<char>;
    using pointer = value_type*;
    using reference = std::span<char>&;
    using difference_type = ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    socket_write_input_iterator() : wrb_(nullptr) {}

    // (db_ , de_] - interval for data placing
    explicit socket_write_input_iterator(SocketT& soc, std::span<char> buff)
        : psoc_{ &soc }, buff_{ buff }, value_{ buff }, db_{ buff.data() }, de_{ data_end(buff) }, wrb_{ buff.data() }, wre_{ buff.data() }
    {
        // here value === (db_ , de_] 
        
        if (buff.size() < MinInputRngSzV + MinSocketWriteRngSzV) {
            THROW_INTERNAL_ERROR("too small buffer size");
        }
    }

    socket_write_input_iterator(socket_write_input_iterator const&) = delete;
    socket_write_input_iterator(socket_write_input_iterator&& rhs) = default;
    socket_write_input_iterator& operator= (socket_write_input_iterator const&) = delete;
    socket_write_input_iterator& operator= (socket_write_input_iterator&& rhs) = default;

    bool empty() const { return !wrb_; }

    bool operator== (socket_write_input_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    bool operator!=(socket_write_input_iterator const& rhs) const
    {
        return !this->operator==(rhs);
    }

    reference operator*() const
    {
        BOOST_ASSERT(wrb_);
        return value_;
    }

    socket_write_input_iterator& operator++()
    {
        BOOST_ASSERT(wrb_);
        value_ = std::span{ data_end(value_), de_ };
        // (db_, data_end(value_)] - interval that can be append to sending data
        // ? wre_ == db_ || wrb_ == de_
        if (wre_ == db_) {
            db_ = wre_ = value_.data();
            return inc_wre_db();
        } else {
            BOOST_ASSERT(wrb_ == de_);
            BOOST_ASSERT(db_ == buff_.data() + MinSocketWriteRngSzV);
            while (value_.size() < MinInputRngSzV)
            {
                size_t wrsz = wre_ - wrb_;
                if (wrsz < MinSocketWriteRngSzV) {
                    db_ -= wrsz;
                    std::memcpy(db_, wrb_, wrsz);
                    wrb_ = db_;
                    db_ = wre_ = value_.data();
                    de_ = data_end(buff_);
                    value_ = std::span{ db_, de_ };
                    return inc_wre_db();
                }
                write();
                if (!wrb_) return *this;
                de_ = wrb_;
                value_ = std::span{ value_.data(), de_ };
            }
            return *this;
        }
    }

    void close()
    {
        flush();
    }

    void flush()
    {
        if (wre_ == db_) {
            flush_wre_db();
        } else {
            for (;;) {
                size_t wrsz = wre_ - wrb_;
                if (wrsz < MinSocketWriteRngSzV) {
                    db_ -= wrsz;
                    std::memcpy(db_, wrb_, wrsz);
                    wrb_ = db_;
                    db_ = wre_ = value_.data();
                    de_ = data_end(buff_);
                    value_ = std::span{ db_, de_ };
                    return flush_wre_db();
                }
                write();
                if (!wrb_) return;
                de_ = wrb_;
                value_ = std::span{ value_.data(), de_ };
            }
        }
    }

private:
    void write() noexcept
    {
        BOOST_ASSERT (wrb_ != wre_);
        auto r = psoc_->write_some(std::span<const char>{wrb_, wre_});
        if (r.has_value() && r.value()) {
            wrb_ += r.value();
        } else {
            wrb_ = nullptr;
        }
    }

    void flush_wre_db()
    {
        while (wre_ != wrb_) {
            write();
            if (!wrb_) return;
        }
        db_ = wre_;
    }

    socket_write_input_iterator& inc_wre_db()
    {
        BOOST_ASSERT(wre_ == db_);
        BOOST_ASSERT(de_ == data_end(buff_));

        if (value_.size() >= MinInputRngSzV) return *this;

        BOOST_ASSERT(wre_ - buff_.data() >= MinInputRngSzV);
        while (wrb_ - buff_.data() < (MinInputRngSzV + MinSocketWriteRngSzV)) {
            write();
            if (!wrb_) return *this;
        }
        size_t wrsz = wre_ - wrb_;
        if (wrsz < MinSocketWriteRngSzV) {
            std::memcpy(buff_.data(), wrb_, wrsz);
            wrb_ = buff_.data();
            db_ = wre_ = wrb_ + wrsz;
            de_ = data_end(buff_);
        } else {
            db_ = buff_.data() + MinSocketWriteRngSzV; // reserve space
            de_ = wrb_;
        }
        value_ = std::span{ db_, de_ };
        return *this;
    }

private:
    SocketT * psoc_;
    mutable std::span<char> buff_;
    mutable std::span<char> value_;
    char *db_, *de_, *wrb_, *wre_;
};

}
