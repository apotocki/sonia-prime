//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/exceptions.hpp"
#include "sonia/array_view.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"

namespace sonia {

template <class SocketT>
class socket_write_iterator
	: public boost::iterator_facade<
          socket_write_iterator<SocketT>
		, std::span<const char>
		, std::output_iterator_tag
		, wrapper_iterator_proxy<ptr_proxy_wrapper<socket_write_iterator<SocketT> const*, void>>
	>
{
	friend class boost::iterator_core_access;
	template <class, class> friend class ptr_proxy_wrapper;

	decltype(auto) dereference() const
	{
		return iterators::make_value_proxy(this);
	}

	void set_dereference(std::span<const char> rng)
	{
        while (!rng.empty()) {
            if (auto expwsz = psoc_->write_some(rng.data(), rng.size()); expwsz.has_value()) {
                rng = rng.subspan(expwsz.value());
            } else {
                empty_ = true;
                break;
            }
        }
	}

    void increment() {}

public:
    explicit socket_write_iterator(SocketT & soc) : psoc_(std::addressof(soc)) {}
	
    bool empty() const { return empty_; }

	void flush() {}

private:
	SocketT * psoc_;
    bool empty_{false};
};

}
