//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_BUFFERING_MEDIATOR_ITERATOR_HPP
#define SONIA_UTILITY_BUFFERING_MEDIATOR_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/range/begin.hpp>
#include <boost/range/size.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include "sonia/iterator_traits.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/array_view.hpp"
#include "sonia/utility/linked_buffers.hpp"

namespace sonia {

template <class IteratorT, class CategoryOrTraversalT = forward_traversal_tag>
class buffering_mediator_iterator
	: public boost::iterator_facade<
		  buffering_mediator_iterator<IteratorT, CategoryOrTraversalT>
		, array_view<remove_pointer_t<iterator_dereferenced_range_iterator_pointer_t<IteratorT>>>
		, CategoryOrTraversalT
		, array_view<remove_pointer_t<iterator_dereferenced_range_iterator_pointer_t<IteratorT>>>
	>
{
	friend class boost::iterator_core_access;

	using elem_t = remove_pointer_t<iterator_dereferenced_range_iterator_pointer_t<IteratorT>>;
	using buff_elem_t = iterator_dereferenced_range_iterator_value_t<IteratorT>;
	using allocator_t = std::allocator<char>;
	using base_holder_t = std::tuple<shared_ptr<IteratorT>, size_t>; // size_t used_buffer_size, =0 if base is not initialized;
	using buffer_ptr = single_linked_buffer_ptr<buff_elem_t, base_holder_t, allocator_t>;

	buffer_ptr pbuff_;

    bool equal(buffering_mediator_iterator const& rhs) const
    {
        return pbuff_.get() == rhs.pbuff_.get();
    }

    void increment()
    {
		if (pbuff_->next) {
			buffer_ptr tmp = pbuff_->next;
			pbuff_ = std::move(tmp);
		} else if (1 == pbuff_->refs) {
			std::get<1>((base_holder_t&)*pbuff_) = 0;
		} else {
			pbuff_ = pbuff_->get_or_make_next();
		}
    }

	typename buffering_mediator_iterator::reference dereference() const
    {
		size_t used_buffer_size = std::get<1>((base_holder_t&)*pbuff_);
		return {pbuff_->begin(), used_buffer_size};
    }
	
public:
    buffering_mediator_iterator(IteratorT it, size_t bfsz)
    {
		pbuff_ = make_single_linked_buffer<buff_elem_t, base_holder_t>(allocator_t(), bfsz, make_shared<IteratorT>(std::move(it)), 0);
    }

	bool empty() const
	{
		auto &[pit, used_buffer_size] = (base_holder_t&)*pbuff_;
		if (!used_buffer_size) {
			if (pit->empty()) return true;
			**pit = pbuff_->to_array_view();
			++(*pit);
			if (pit->empty()) return true;
			array_view<elem_t> r = **pit;
			used_buffer_size = r.size();
		}
		return false;
	}

    IteratorT & base() const { return *std::get<0>((base_holder_t&)*pbuff_); }
};

}

#endif // SONIA_UTILITY_BUFFERING_MEDIATOR_ITERATOR_HPP
