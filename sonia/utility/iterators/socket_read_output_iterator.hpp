//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <span>
#include <boost/throw_exception.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

namespace sonia {

template <class SocketT>
class socket_read_output_iterator
    : public boost::iterator_facade<
          socket_read_output_iterator<SocketT>
        , std::span<const char>
        , std::output_iterator_tag
        , wrapper_iterator_proxy<ptr_proxy_wrapper<socket_read_output_iterator<SocketT> const*, std::span<const char>>>
    >
{
    friend class boost::iterator_core_access;
    template <class, class> friend class ptr_proxy_wrapper;

    using proxy_t = wrapper_iterator_proxy<ptr_proxy_wrapper<socket_read_output_iterator const*, std::span<const char>>>;

    proxy_t dereference() const
    {
        return iterators::make_value_proxy<std::span<const char>>(this);
    }

    std::span<const char> get_dereference() const
    {
        return buff_;
    }

    void set_dereference(std::span<char> rng)
    {
        buff_ = rng;
    }

    void increment()
    {
        if (auto exprsz = psoc_->read_some(buff_.data(), buff_.size()); exprsz.has_value() && exprsz.value()) {
            buff_ = std::span{buff_.data(), exprsz.value()};
        } else {
            empty_ = true;
        }
    }

public:
    explicit socket_read_output_iterator(SocketT & soc) : psoc_(std::addressof(soc)) {}

    bool empty() const { return empty_; }

private:
    SocketT * psoc_;
    std::span<char> buff_;
    bool empty_{false};
};

}
