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
            auto e = boost::begin(r);
            std::advance(e, limit);
            r = value_t(b, e);
            *base = r;
        }
        return r;
	}

	void set_dereference(value_t rng)
	{
        *base = rng;
	}

	void increment()
	{
		value_t r = *base;
        size_t cursz = boost::size(r);
        BOOST_ASSERT (cursz <= limit);
        limit -= cursz;
        ++base;
	}

public:
    iterator_of_ranges_with_limit(IteratorT it, size_t limitsz)
        : base(std::move(it)), limit(limitsz)
    {}

    bool empty() const { return !limit; }

    IteratorT base;
    size_t limit;
};

}

#endif // SONIA_UTILITY_ITERATOR_OF_RANGES_WITH_LIMIT_HPP
