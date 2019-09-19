//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_RANGE_BUFFERING_ITERATOR_HPP
#define SONIA_UTILITY_RANGE_BUFFERING_ITERATOR_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_facade.hpp>

#include "sonia/array_view.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/iterator_traits.hpp"
#include "sonia/utility/linked_buffers.hpp"

namespace sonia {

template <typename IteratorT>
class range_buffering_iterator
    : public boost::iterator_facade<
          range_buffering_iterator<IteratorT>
        , array_view<const char>
        , forward_traversal_tag
        , array_view<const char>&
    >
{
    friend class boost::iterator_core_access;
	template <class, class> friend class ptr_proxy_wrapper;

    using allocator_t = std::allocator<char>;
    using base_holder_t = std::tuple<shared_ptr<IteratorT>, size_t>; // size_t used_buffer_size, =0 if base is not initialized;
    using buffer_ptr = single_linked_buffer_ptr<char, base_holder_t, allocator_t>;

    buffer_ptr pbuff_;
    mutable array_view<const char> value_;

    bool equal(range_buffering_iterator const& rhs) const
    {
        return value_.begin() == rhs.value_.begin();
    }

    array_view<const char>& dereference() const
	{
		return value_;
	}

    void increment()
    {
        BOOST_ASSERT (value_.begin());
        auto &[pit, used_buffer_size] = (base_holder_t&)*pbuff_;
        auto tmp = pbuff_->to_array_view();
        if (tmp.begin() + used_buffer_size != value_.end()) {
            value_ = array_view<const char>{value_.end(), tmp.begin() + used_buffer_size};
            return;
        } else if (pbuff_->next) {
            buffer_ptr tmp = pbuff_->next;
            pbuff_ = std::move(tmp);
            value_ = array_view{pbuff_->begin(), std::get<1>((base_holder_t&)*pbuff_)};
            return;
        } else if (1 == pbuff_->refs) {
            std::get<1>((base_holder_t&)*pbuff_) = 0;
        } else {
            if (pbuff_->size() == used_buffer_size) {
                // run out of avaibale buffer space, need to allocate new buffer
                auto tmp = pbuff_->get_or_make_next();
                ((base_holder_t&)*tmp) = { std::get<0>((base_holder_t&)*pbuff_), 0 };
                pbuff_ = std::move(tmp);
            }
        }
        value_.reset();
        init();
    }

    void init()
    {
        auto &[pit, used_buffer_size] = (base_holder_t&)*pbuff_;
        if (sonia::empty(*pit)) return;
        auto buff = pbuff_->to_array_view();
        buff.advance_front(used_buffer_size);

        array_view<const char> tmp = **pit;
        size_t used_avail_sz = (std::min)(tmp.size(), buff.size());
        std::memcpy(buff.begin(), tmp.begin(), used_avail_sz);
        value_ = array_view<const char>{buff.begin(), used_avail_sz};
        used_buffer_size += used_avail_sz;
        tmp.advance_front(used_avail_sz);
        if (tmp.empty()) {
            ++(*pit);
        } else {
            **pit = tmp;
        }
    }

public:
    range_buffering_iterator(IteratorT it, size_t bfsz)
    {
        pbuff_ = make_single_linked_buffer<char, base_holder_t>(allocator_t(), bfsz, make_shared<IteratorT>(std::move(it)), 0);
        init();
    }

    template <typename ... ArgsT>
    range_buffering_iterator(in_place_t, size_t bfsz, ArgsT && ... args)
    {
        pbuff_ = make_single_linked_buffer<char, base_holder_t>(allocator_t(), bfsz, make_shared<IteratorT>(std::forward<ArgsT>(args) ...), 0);
        init();
    }

    bool empty() const
    {
        while (value_.empty()) {
            if (!value_.begin()) return true;
            const_cast<range_buffering_iterator*>(this)->increment();
        }
        return false;
    }

    IteratorT & get_base() { return std::get<0>((base_holder_t&)*pbuff_); }

    template <typename T = IteratorT, typename = enable_if_t<iterators::has_method_flush_v<T, void()>>>
    void flush() { get_base().flush(); }

    template <typename T = IteratorT, typename = enable_if_t<iterators::has_method_close_v<T, void()>>>
    void close() { get_base().close(); }
};

}

#endif // SONIA_UTILITY_RANGE_BUFFERING_ITERATOR_HPP
