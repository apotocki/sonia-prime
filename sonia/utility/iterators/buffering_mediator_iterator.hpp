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

// IteratorT is Output Read Iterator of ranges!
template <class IteratorT, class CategoryOrTraversalT = forward_traversal_tag>
class buffering_mediator_iterator
	: public boost::iterator_facade<
		  buffering_mediator_iterator<IteratorT, CategoryOrTraversalT>
		, array_view<remove_pointer_t<iterator_dereferenced_range_iterator_pointer_t<IteratorT>>>
		, CategoryOrTraversalT
        , wrapper_iterator_proxy<
            ptr_proxy_wrapper<
                buffering_mediator_iterator<IteratorT, CategoryOrTraversalT> const*,
                array_view<remove_pointer_t<iterator_dereferenced_range_iterator_pointer_t<IteratorT>>>
            >
          >
	>
{
	friend class boost::iterator_core_access;
    template <class, class> friend class ptr_proxy_wrapper;

	using elem_t = remove_pointer_t<iterator_dereferenced_range_iterator_pointer_t<IteratorT>>;
	using buff_elem_t = iterator_dereferenced_range_iterator_value_t<IteratorT>;
	using allocator_t = std::allocator<char>;
	using base_holder_t = std::tuple<shared_ptr<IteratorT>, size_t>; // size_t used_buffer_size, =0 if base is not initialized;
	using buffer_ptr = single_linked_buffer_ptr<buff_elem_t, base_holder_t, allocator_t>;
    using proxy_t = wrapper_iterator_proxy<ptr_proxy_wrapper<buffering_mediator_iterator const*, array_view<elem_t>>>;

	buffer_ptr pbuff_;
    mutable buff_elem_t * begin_{ nullptr };
    mutable size_t size_{ 0 };

    bool equal(buffering_mediator_iterator const& rhs) const
    {
        return (begin_ ? begin_ : pbuff_->begin()) == (rhs.begin_ ? rhs.begin_ : rhs.pbuff_->begin());
    }

#if 0
    void increment()
    {
		if (pbuff_->next) {
			buffer_ptr tmp = pbuff_->next;
			pbuff_ = std::move(tmp);
            begin_ = pbuff_->begin();
            size_ = std::get<1>((base_holder_t&)*pbuff_);
		} else if (1 == pbuff_->refs) {
			std::get<1>((base_holder_t&)*pbuff_) = 0;
            size_ = 0;
		} else {
            begin_ += size_;
            base_holder_t& tpl = *pbuff_;
            size_t used_buffer_size = std::get<1>(tpl); // another part could change used size
            elem_t * end = pbuff_->begin() + used_buffer_size;
            if (end > begin_) {
                size_ = end - begin_;
                return;
            }
            BOOST_ASSERT(begin_ == end);
            size_ = 0;
            if (end < pbuff_->end()) { // not complete current buffer is used yet
                return;
            }
            // need to allocate new buffer
			auto tmp = pbuff_->get_or_make_next();
            ((base_holder_t&)*tmp) = { std::get<0>((base_holder_t&)*pbuff_), 0 };
            pbuff_ = std::move(tmp);
            begin_ = pbuff_->begin();
		}
    }
#endif

    void increment()
    {
        begin_ += size_;
        base_holder_t& tpl = *pbuff_;
        size_t used_buffer_size = std::get<1>(tpl); // another part could change used size
        elem_t * end = pbuff_->begin() + used_buffer_size;
        if (end > begin_) {
            size_ = end - begin_;
            return;
        }
        BOOST_ASSERT(begin_ == end);
        size_ = 0;
        if (end < pbuff_->end()) { // the current buffer is not fully used
            return;
        }
        if (pbuff_->next) {
            buffer_ptr tmp = pbuff_->next;
            pbuff_ = std::move(tmp);
            begin_ = pbuff_->begin();
            size_ = std::get<1>((base_holder_t&)*pbuff_);
        } else if (1 == pbuff_->refs) {
            std::get<1>((base_holder_t&)*pbuff_) = 0;
            begin_ = nullptr;
            // we expect that callee will call empty() next
        } else {
            // need to allocate new buffer
			auto tmp = pbuff_->get_or_make_next();
            ((base_holder_t&)*tmp) = { std::get<0>((base_holder_t&)*pbuff_), 0 };
            pbuff_ = std::move(tmp);
            begin_ = pbuff_->begin();
        }
    }

    proxy_t dereference() const
    {
        return iterators::make_value_proxy<array_view<const char>>(this);
    }

	array_view<elem_t> get_dereference() const
    {
        BOOST_ASSERT(begin_);
		return {begin_, size_};
    }

    void set_dereference(array_view<elem_t> d)
    {
        d.is_subset_of(get_dereference());
        begin_ += (d.begin() - begin_);
        size_ = d.size();
    }
	
public:
    buffering_mediator_iterator(IteratorT it, size_t bfsz)
    {
		pbuff_ = make_single_linked_buffer<buff_elem_t, base_holder_t>(allocator_t(), bfsz, make_shared<IteratorT>(std::move(it)), 0);
    }

    template <typename ... ArgsT>
    buffering_mediator_iterator(in_place_t, size_t bfsz, ArgsT && ... args)
    {
        pbuff_ = make_single_linked_buffer<buff_elem_t, base_holder_t>(allocator_t(), bfsz, make_shared<IteratorT>(std::forward<ArgsT>(args) ...), 0);
    }

	bool empty() const
	{
        if (size_) return false;

        buff_elem_t * buff_begin = pbuff_->begin();
        if (!begin_) {
            begin_ = buff_begin;
        }

		auto &[pit, used_buffer_size] = (base_holder_t&)*pbuff_;
        size_t offset = begin_ - buff_begin;
        if (used_buffer_size == offset) {
			if (pit->empty()) return true;
            **pit = array_view{begin_, pbuff_->end()};
			++(*pit);
			if (pit->empty()) return true;
			array_view<elem_t> r = **pit;
            BOOST_ASSERT(r.begin() == begin_);
            size_ = r.size();
            used_buffer_size += size_;
        } else { // another part changed used_buffer_size, update available size_
            BOOST_ASSERT (used_buffer_size > offset);
            size_ = used_buffer_size - offset;
        }
		return false;
	}

    void flush() {}

    IteratorT & base() const { return *std::get<0>((base_holder_t&)*pbuff_); }
};

}

#endif // SONIA_UTILITY_BUFFERING_MEDIATOR_ITERATOR_HPP
