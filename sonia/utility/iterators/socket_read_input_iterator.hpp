//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_SOCKET_READ_INPUT_ITERATOR_HPP
#define SONIA_UTILITY_SOCKET_READ_INPUT_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/exceptions.hpp"

namespace sonia {

// provided buffer is used as circular data buffer

template <class SocketT>
class socket_read_input_iterator
    : public boost::iterator_facade<
          socket_read_input_iterator<SocketT>
        , array_view<const char>
        , std::input_iterator_tag
        , array_view<const char>&
    >
{
    friend class boost::iterator_core_access;

    bool equal(socket_read_input_iterator const& rhs) const
    {
        return empty() && rhs.empty();
    }

    array_view<const char> & dereference() const
    {
        BOOST_ASSERT (psoc_);
        if (!ready_buff_.begin()) {
            auto exprsz = psoc_->read_some(buff_);
            if (!exprsz.has_value() || !exprsz.value()) {
                psoc_ = nullptr;
                throw eof_exception();
            } else {
                ready_buff_ = {buff_.begin(), exprsz.value()};
                ready_pos_ = ready_buff_.end();
            }
        }
        return ready_buff_;
    }

    void increment()
    {
        BOOST_ASSERT (psoc_);
        if (ready_buff_.end() == ready_pos_) {
            ready_buff_.reset();
        } else {
            ready_buff_ = {ready_buff_.end(), ready_pos_};
        }
    }
    
public:
    socket_read_input_iterator() : psoc_{nullptr} {}

    explicit socket_read_input_iterator(SocketT & soc, array_view<char> buff, size_t rsz = 0) 
        : psoc_{&soc}, buff_{buff}, ready_buff_{rsz ? buff_.begin() : nullptr, rsz}, ready_pos_{rsz ? buff_.begin() + rsz : nullptr}
    {}

    socket_read_input_iterator(socket_read_input_iterator const&) = delete;
    socket_read_input_iterator(socket_read_input_iterator && rhs) = default;
    socket_read_input_iterator& operator= (socket_read_input_iterator const&) = delete;
    socket_read_input_iterator& operator= (socket_read_input_iterator && rhs) = default;

    bool empty() const { return !psoc_; }

private:
    mutable SocketT * psoc_;
    array_view<char> buff_;
    mutable array_view<const char> ready_buff_;
    mutable const char * ready_pos_{nullptr};
};

}

#endif // SONIA_UTILITY_SOCKET_READ_INPUT_ITERATOR_HPP
