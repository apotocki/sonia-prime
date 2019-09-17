//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_RANGE_CACHEABLE_ITERATOR_HPP
#define SONIA_UTILITY_RANGE_CACHEABLE_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/utility/optimized_optional.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/iterators/proxy.hpp"
#include "sonia/utility/range/is_subset_of.hpp"

namespace sonia {

template <typename IteratorOfRangesT>
class range_cacheable_iterator
    : public boost::iterator_facade<
          DerivedT
        , iterator_value_t<IteratorOfRangesT>
        , not_more_traversal_t<forward_traversal_tag, iterator_traversal_t<IteratorOfRangesT>>
        , wrapper_iterator_proxy<ptr_proxy_wrapper<range_cacheable_iterator<IteratorOfRangesT> const*, iterator_value_t<IteratorOfRangesT>>>
{
    friend class boost::iterator_core_access;
	template <class, class> friend class ptr_proxy_wrapper;

    using value_t = iterator_value_t<IteratorOfRangesT>;
    bool equal(range_cacheable_iterator const& rhs) const
    {
        return base == rhs.base;
    }

    decltype(auto) dereference() const
	{
		return iterators::make_value_proxy<iterator_value_t<IteratorT>>(this);
	}

    value_t get_dereference() const
	{
        if (!cached_)) {
            cached_ = *base;
        }
        return *cached_;
    }

    void set_dereference(value_t rng)
	{
        BOOST_ASSERT (cached_ && ranges::is_subset_of(rng, *cached_));
        cached_ = rng;
    }

    void increment()
    {
        BOOST_ASSERT(cached_);
        value_t baserng = *base;
        if (boost::end(cached_) != boost::end(baserng)) {
            cached_ = value_t{boost::end(cached_), boost::end(baserng)};
        } else {
            BOOST_ASSERT(!empty(base));
            ++base;
            cached_.reset();
        }
    }

public:
    explicit range_cacheable_iterator(IteratorOfRangesT it) : base{std::move(it)} {}

    template <typename ArgsT>
    explicit range_cacheable_iterator(in_place_t, ArgsT && ... args) : base{std::forward<ArgsT>(args) ...} {}

    bool empty() const
    {
        if (!base_) return true;
        return sonia::empty(*base_);
    }

    template <typename T = IteratorOfRangesT, typename = enable_if_t<iterators::has_method_flush_v<T, void()>>>
    void flush() { base.flush(); }

    template <typename T = IteratorOfRangesT, typename = enable_if_t<iterators::has_method_close_v<T, void()>>>
    void close() { base.close(); }

    IteratorT base;

private:
    optimized_optional<value_t> cached_;
};

}

#endif // SONIA_UTILITY_RANGE_CACHEABLE_ITERATOR_HPP
