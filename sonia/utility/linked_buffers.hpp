//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_LINKED_BUFFERS_HPP
#define SONIA_UTILITY_LINKED_BUFFERS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <memory>

#include <boost/intrusive_ptr.hpp>

#include "sonia/utility/buffer.hpp"

namespace sonia {

template <typename T, class BaseT, class AllocatorT> class single_linked_buffer_base;

template <typename T, class BaseT = empty_t, class AllocatorT = std::allocator<T>>
using single_linked_buffer = adjacent_buffer<T, single_linked_buffer_base<T, BaseT, AllocatorT>>;

template <typename T, class BaseT = empty_t, class AllocatorT = std::allocator<T>>
using single_linked_buffer_ptr = boost::intrusive_ptr<single_linked_buffer<T, BaseT, AllocatorT>>;

template <typename T, class BaseT, class AllocatorT, typename ... BaseArgsT>
auto make_single_linked_buffer(AllocatorT const& alloc, size_t sz, BaseArgsT && ... args)
{
	return single_linked_buffer_ptr<T, BaseT, AllocatorT>(
		allocate_adjacent_buffer<T, single_linked_buffer_base<T, BaseT, AllocatorT>>(alloc, sz, in_place, alloc, std::forward<BaseArgsT>(args) ...), false
	);
}

template <typename T, class BaseT, class AllocatorT>
class single_linked_buffer_base : public BaseT, AllocatorT
{
public:
    using buffer_ptr = single_linked_buffer_ptr<T, BaseT, AllocatorT>;
    
	template <typename ... BaseArgsT>
    explicit single_linked_buffer_base(AllocatorT const& alloc, BaseArgsT && ... args) 
		: BaseT(std::forward<BaseArgsT>(args) ...)
		, AllocatorT(alloc)
		, refs(1)
	{}

    ~single_linked_buffer_base()
	{
        // avoid stack overflow for long chains
        while (next && 1 == next->refs) {
            buffer_ptr nnext = std::move(next->next);
            next = std::move(nnext);
        }
    }

	AllocatorT const& allocator() const { return *this; }

    buffer_ptr get_or_make_next(size_t preferred_sz = 0) // 0 means the same size
	{
        if (!next) {
            next = make_single_linked_buffer<T, BaseT>(
                allocator(),
                preferred_sz ? preferred_sz : static_cast<single_linked_buffer<T, BaseT, AllocatorT>*>(this)->size()
            );
        }
        return next;
    }

    friend void intrusive_ptr_add_ref(single_linked_buffer_base * p)
	{
        ++p->refs;
    }

    friend void intrusive_ptr_release(single_linked_buffer_base * p)
	{
        if (0 == --p->refs) {
            deallocate_adjacent_buffer(p->allocator(), static_cast<single_linked_buffer<T, BaseT, AllocatorT>*>(p));
        }
    }

    buffer_ptr next;
    int refs;
};

template <typename T, class BaseT, typename ... BaseArgsT>
auto make_single_linked_buffer(size_t sz, BaseArgsT && ... args)
{
    return make_single_linked_buffer<T, BaseT>(std::allocator<T>(), sz, std::forward<BaseArgsT>(args) ...);
}

template <typename T>
auto make_single_linked_buffer(size_t sz)
{
	return make_single_linked_buffer<T, empty_t>(std::allocator<T>(), sz);
}

}

#endif // SONIA_UTILITY_LINKED_BUFFERS_HPP
