//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_ITERATOR_OF_RANGES_WITH_LIMIT_HPP
#define SONIA_UTILITY_ITERATOR_OF_RANGES_WITH_LIMIT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/size.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include "sonia/iterator_traits.hpp"
#include "sonia/optional.hpp"
#include "sonia/utility/iterators/proxy.hpp"

namespace sonia {

template <class IteratorT>
class iterator_of_ranges_with_limit
    : public boost::iterator_facade<
		  iterator_of_ranges_with_limit<IteratorT>
		, iterator_value_t<IteratorT>
		, iterator_category_t<IteratorT>
		, wrapper_iterator_proxy<ptr_proxy_wrapper<iterator_of_ranges_with_limit<IteratorT> const*, iterator_value_t<IteratorT>>>
	>
{
	friend class boost::iterator_core_access;
	template <class, class> friend class ptr_proxy_wrapper;

    using value_t = typename iterator_of_ranges_with_limit::value_type; // alias to this_class::value_type

    bool equal(iterator_of_ranges_with_limit const& rhs) const
    {
        return empty() && rhs.empty();
    }

    decltype(auto) dereference() const
	{
		return iterators::make_value_proxy<iterator_value_t<IteratorT>>(this);
	}

    value_t get_dereference() const
	{
		value_t r = *base;
        size_t cursz = boost::size(r);
        if (cursz > limit) {
            auto b = boost::begin(r);
            auto e = b;
            std::advance(e, limit);
            *base = value_t{e, boost::end(r)};
            r = value_t{b, e};
        }
        if (!current_size) {
            current_size = boost::size(r);
        }
        return r;
	}

	void set_dereference(value_t rng)
	{
        *base = rng;
	}

	void increment()
	{
        if (!current_size) {
            value_t r = *base;
            current_size = boost::size(r);
        }
        limit -= *current_size;
        current_size.reset();
        ++base;
	}

public:
    iterator_of_ranges_with_limit(IteratorT it, uint64_t limitsz)
        : base{std::move(it)}, limit{limitsz}
    {}

    bool empty() const { return !limit; }

    template <typename T = IteratorT>
    enable_if_t<iterators::has_method_flush_v<T, void()>> flush() { base.flush(); }

    IteratorT base;
    uint64_t limit;
    mutable optional<size_t> current_size;
};

}

#endif // SONIA_UTILITY_ITERATOR_OF_RANGES_WITH_LIMIT_HPP
