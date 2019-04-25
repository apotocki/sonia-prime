//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_TCP_WRITE_OUTPUT_ITERATOR_HPP
#define SONIA_IO_TCP_WRITE_OUTPUT_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>
#include "sonia/exceptions.hpp"
#include "tcp_socket.hpp"
#include "sockets.hpp"

namespace sonia::io {

class tcp_write_output_iterator
    : public boost::iterator_facade<
          tcp_write_output_iterator
        , array_view<const char>
        , std::output_iterator_tag
    >
{
    friend class boost::iterator_core_access;

    bool equal(tcp_write_output_iterator const& rhs) const
    {
        return false;
    }

    array_view<const char>& dereference() const
    {
        return buff_;
    }

    void increment()
    {
        while (buff_.size()) {
            size_t sz = psoc_->write_some(buff_);
            if (!sz) throw eof_exception();
            buff_ = array_view<const char>(buff_.begin() + sz, buff_.size() - sz);
        }
    }

public:
    explicit tcp_write_output_iterator(tcp_socket & soc) 
        : psoc_(&soc)
    {}

    tcp_write_output_iterator(tcp_write_output_iterator const&) = delete;
    tcp_write_output_iterator(tcp_write_output_iterator && rhs) = default;
    tcp_write_output_iterator& operator= (tcp_write_output_iterator const&) = delete;
    tcp_write_output_iterator& operator= (tcp_write_output_iterator && rhs) = default;

private:
    tcp_socket * psoc_;
    mutable array_view<const char> buff_;
};

}

#endif // SONIA_IO_TCP_WRITE_OUTPUT_ITERATOR_HPP
