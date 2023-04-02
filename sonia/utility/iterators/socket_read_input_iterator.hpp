//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/span.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/iterators/proxy.hpp"

namespace sonia {

// provided buffer is used as circular data buffer

template <class SocketT>
class socket_read_input_iterator
{
public:
    using value_type = std::span<const char>;
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type;

    using proxy_t = wrapper_iterator_proxy<ptr_proxy_wrapper<socket_read_input_iterator const*, value_type>>;

    socket_read_input_iterator() : psoc_{nullptr} {}

    socket_read_input_iterator(SocketT & soc, std::span<char> buff, size_t rsz = 0)
        : psoc_{&soc}, buff_{buff}, ready_buff_{rsz ? buff_.data() : nullptr, rsz}
        , ready_pos_{rsz ? buff_.data() + rsz : nullptr }
    {
        
    }

    socket_read_input_iterator(socket_read_input_iterator const&) = delete;
    socket_read_input_iterator(socket_read_input_iterator && rhs) = default;
    socket_read_input_iterator& operator= (socket_read_input_iterator const&) = delete;
    socket_read_input_iterator& operator= (socket_read_input_iterator && rhs) = default;

    bool empty() const { return !psoc_; }

    bool operator==(socket_read_input_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    proxy_t operator* () const
    {
        return iterators::make_value_proxy<value_type>(this);
    }

    value_type get_dereference() const
    {
        if (!psoc_) {
            throw eof_exception();
        }
        if (ready_buff_.empty()) {
            auto exprsz = psoc_->read_some(buff_);
            if (!exprsz.has_value() || !exprsz.value()) {
                psoc_ = nullptr;
                throw eof_exception();
            } else {
                ready_buff_ = { buff_.data(), exprsz.value() };
                ready_pos_ = data_end(ready_buff_);
            }
        }
        return ready_buff_;
    }

    void set_dereference(value_type d)
    {
        ready_buff_ = d;
    }

    socket_read_input_iterator& operator++()
    {
        BOOST_ASSERT(psoc_);
        if (data_end(ready_buff_) == ready_pos_) {
            ready_buff_ = {};
        } else {
            ready_buff_ = { data_end(ready_buff_), ready_pos_ };
        }
        return *this;
    }

    SocketT* socket() { return psoc_; }

    void close()
    {
        if (psoc_) {
            psoc_->shutdown(io::shutdown_opt::read);
        }
    }

private:
    mutable SocketT * psoc_;
    std::span<char> buff_;
    mutable std::span<const char> ready_buff_;
    mutable const char* ready_pos_;
};

}
