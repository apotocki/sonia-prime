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

template <typename T, class AllocatorT = std::allocator<T>> class single_linked_buffer_base;

template <typename T, class AllocatorT = std::allocator<T>>
using single_linked_buffer = adjacent_buffer<T, single_linked_buffer_base<T, AllocatorT>>;

template <typename T, class AllocatorT = std::allocator<T>>
using single_linked_buffer_ptr = boost::intrusive_ptr<single_linked_buffer<T, AllocatorT>>;

template <typename T, class AllocatorT>
single_linked_buffer_ptr<T, AllocatorT> make_single_linked_buffer(size_t);

template <typename T, class AllocatorT>
class single_linked_buffer_base {
public:
    typedef single_linked_buffer_ptr<T, AllocatorT> buffer_ptr;
    
    single_linked_buffer_base() : refs_(1) {}

    ~single_linked_buffer_base() {
        // avoid stack overflow for long chains
        while (next_ && 1 == next_->refs_) {
            buffer_ptr nnext = std::move(next_->next_);
            next_ = std::move(nnext);
        }
    }

    buffer_ptr next(size_t preferred_sz = 0) { // 0 means the same size
        if (!next_) {
            next_ = make_single_linked_buffer<T, AllocatorT>(
                preferred_sz ? preferred_sz : static_cast<single_linked_buffer<T>*>(this)->size()    
            );
        }
        return next_;
    }

    friend void intrusive_ptr_add_ref(single_linked_buffer_base * p) {
        ++p->refs_;
    }

    friend void intrusive_ptr_release(single_linked_buffer_base * p) {
        if (0 == --p->refs_) {
            deallocate_adjacent_buffer<AllocatorT>(static_cast<single_linked_buffer<T>*>(p));
        }
    }

private:
    buffer_ptr next_;
    int refs_;
};

template <typename T, class AllocatorT>
single_linked_buffer_ptr<T, AllocatorT> make_single_linked_buffer(size_t sz) {
    return single_linked_buffer_ptr<T, AllocatorT>(
        allocate_adjacent_buffer<T, single_linked_buffer_base<T, AllocatorT>, AllocatorT>(sz, in_place), false
    );
}

template <typename T>
single_linked_buffer_ptr<T> make_single_linked_buffer(size_t sz) {
    return make_single_linked_buffer<T, std::allocator<T>>(sz);
}

}

#endif // SONIA_UTILITY_LINKED_BUFFERS_HPP
