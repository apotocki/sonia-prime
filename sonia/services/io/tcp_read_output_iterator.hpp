//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_TCP_READ_OUTPUT_ITERATOR_HPP
#define SONIA_IO_TCP_READ_OUTPUT_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>
#include "sonia/utility/iterators/proxy.hpp"

#include "tcp_socket.hpp"

namespace sonia { namespace io {

class tcp_read_output_iterator
    : public boost::iterator_facade<
          tcp_read_output_iterator
        , array_view<const char>
        , std::output_iterator_tag
        , wrapper_iterator_proxy<basic_dereference_wrapper<tcp_read_output_iterator, array_view<const char>>>
    >
{
    using dereference_wrapper_type = basic_dereference_wrapper<tcp_read_output_iterator, array_view<const char>>;

    friend class boost::iterator_core_access;

    bool equal(tcp_read_output_iterator const& rhs) const
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
            size_t sz = psoc_->read_some(buff_);
            buff_ = array_view<const char>(buff_.begin() + sz, buff_.size() - sz);
        }
    }

public:
    tcp_read_output_iterator(tcp_socket & soc, array_view<char> buff) 
        : psoc_(&soc), buff_(buff)
    {}

    tcp_read_output_iterator(tcp_read_output_iterator const&) = delete;
    tcp_read_output_iterator(tcp_read_output_iterator && rhs) = default;
    tcp_read_output_iterator& operator= (tcp_read_output_iterator const&) = delete;
    tcp_read_output_iterator& operator= (tcp_read_output_iterator && rhs) = default;

private:
    tcp_socket * psoc_;
    mutable array_view<char> buff_;
};

}}

#endif // SONIA_IO_TCP_READ_OUTPUT_ITERATOR_HPP
