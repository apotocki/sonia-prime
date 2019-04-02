//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_OPTIMIZED_ARRAY_HPP
#define SONIA_UTILITY_OPTIMIZED_ARRAY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <memory>

#include "sonia/string.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/utility/buffer.hpp"
#include "sonia/utility/optimized/holder.hpp"
#include "sonia/utility/functional/mover.hpp"
#include "sonia/utility/functional/range_equal.hpp"
#include "sonia/utility/functional/range_less.hpp"

namespace sonia {

// inheritance sequence: adjacent_buffer -> DerivedBaseT -> optimized_array_base -> optimized_base

template <typename T, typename RefCountT, class DerivedBaseT = void, class AllocatorT = std::allocator<T>>
class optimized_array_base 
	: public optimized_base<RefCountT>
	, AllocatorT
{
public:
    optimized_base<RefCountT> * clone() const override;
    void dispose() noexcept override;

	AllocatorT & allocator() const { return const_cast<AllocatorT&>(static_cast<AllocatorT const&>(*this)); }
};

template <typename T, typename RefCountT, class DerivedBaseT, class AllocatorT>
optimized_base<RefCountT> * optimized_array_base<T, RefCountT, DerivedBaseT, AllocatorT>::clone() const
{
    using base_derived_t = conditional_t<is_same_v<void, DerivedBaseT>, optimized_array_base, DerivedBaseT>;
    using derived_t = adjacent_buffer<T, base_derived_t>;
    derived_t const& self = static_cast<derived_t const&>(*this);
    return allocate_adjacent_buffer<T, base_derived_t>(allocator(), self.size(), self.to_array_view(), self);
}

template <typename T, typename RefCountT, class DerivedBaseT, class AllocatorT>
void optimized_array_base<T, RefCountT, DerivedBaseT, AllocatorT>::dispose() noexcept
{
    using base_derived_t = conditional_t<is_same_v<void, DerivedBaseT>, optimized_array_base, DerivedBaseT>;
    using derived_t = adjacent_buffer<T, base_derived_t>;
    deallocate_adjacent_buffer(allocator(), static_cast<derived_t*>(this));
}

template <class ElementT, class HolderT>
struct optimized_array_impl
{
    using allocator_t = std::allocator<ElementT>;
    using optimized_collection_base_t = optimized_array_base<ElementT, typename HolderT::refcount_t>;
    using optimized_collection_t = adjacent_buffer<ElementT, optimized_collection_base_t>;

    static const size_t alv = alignment_of_v<ElementT>;
    static const size_t aligned_offs = ceiling_v<HolderT::begin_offs, alv>;
    static const size_t gmaxsz = (sizeof(HolderT) - aligned_offs) / sizeof(ElementT); // guessed
    static const size_t szbitscnt = gmaxsz ? boost::static_log2<gmaxsz ? gmaxsz : 1>::value + 1 : 1;
    static const size_t sz_mask = (1 << szbitscnt) - 1;
    static const size_t aligned_byte_offs = ceiling_v<(HolderT::used_bits + szbitscnt + CHAR_BIT - 1) / CHAR_BIT, alv>;
    static const size_t maxsz = (sizeof(HolderT) - aligned_byte_offs) / sizeof(ElementT);

    static ElementT const* local_begin(HolderT const* h) noexcept { return (ElementT const*)(h->data() + aligned_byte_offs); }
    static ElementT * local_begin(HolderT * h) noexcept { return (ElementT*)(h->data() + aligned_byte_offs); }
    static ElementT const* local_end(HolderT const* h) noexcept { return begin(h) + (h->get_uint() & sz_mask); }
    static ElementT * local_end(HolderT * h) noexcept { return begin(h) + (h->get_uint() & sz_mask); }

    static void init(HolderT * self)
    {
        self->set_uint(0);
    }

    static void init(HolderT * self, size_t sz)
    {
        if (sz <= maxsz) {
            self->set_uint(sz);
            construct(begin(self), sz);
        } else {
            optimized_collection_t * ptr = allocate_adjacent_buffer<ElementT, optimized_collection_base_t>(allocator_t(), sz, in_place);
            self->set_pointer(ptr);
        }
    }

    template <typename RngT>
    static void init(HolderT * self, RngT && rng, size_t sz)
    {
        if (sz <= maxsz) {
            self->set_uint(sz);
            construct(begin(self), std::forward<RngT>(rng), sz);
        } else {
            optimized_collection_t * ptr = allocate_adjacent_buffer<ElementT, optimized_collection_base_t>(
				allocator_t(), sz, std::forward<RngT>(rng)
            );
            self->set_pointer(ptr);
        }
    }

    template <typename RngT>
    static void init(HolderT * self, RngT && rng)
	{
        init(self, std::forward<RngT>(rng), boost::size(rng));
    }

    static ElementT* begin(HolderT * self)
    {
        return self->is_ptr() ? ptr(self)->begin() : local_begin(self);
    }

    static ElementT const* begin(HolderT const* self)
    {
        return self->is_ptr() ? ptr(self)->begin() : local_begin(self);
    }

    static ElementT* end(HolderT * self)
    {
        return self->is_ptr() ? ptr(self)->end() : local_end(self);
    }

    static ElementT const* end(HolderT const* self)
    {
        return self->is_ptr() ? ptr(self)->end() : local_end(self);
    }

    static array_view<ElementT> get(HolderT * self)
	{
        return self->is_ptr() ? ptr(self)->to_array_view() 
            : array_view<ElementT>(local_begin(self), self->get_uint() & sz_mask);
    }

    static array_view<const ElementT> get(HolderT const * self)
    {
        return self->is_ptr() ? ptr(self)->to_array_view()
            : array_view<const ElementT>(local_begin(self), self->get_uint() & sz_mask);
    }

    static size_t size(HolderT const* self)
	{
        return self->is_ptr() ? ptr(self)->size() : (self->get_uint() & sz_mask);
    }

protected:
    static optimized_collection_t * ptr(HolderT * self)
	{
        return static_cast<optimized_collection_t *>(self->get_pointer());
    }

    static optimized_collection_t const* ptr(HolderT const* self)
	{
        return static_cast<optimized_collection_t const*>(self->get_pointer());
    }
};

template <typename ElementT, size_t ByteSzV, typename RefCountT>
class optimized_array : protected optimized_holder<ByteSzV, 0, RefCountT>
{
protected:
    using holder_t = typename optimized_array::optimized_holder_t;
    using array_t = optimized_array_impl<ElementT, holder_t>;

    template <class HolderArgT>
    optimized_array(in_place_type_t<holder_t>, HolderArgT && harg)
        : holder_t(std::forward<HolderArgT>(harg))
    {}

public:
    using iterator = ElementT *;
    using const_iterator = ElementT const*;
    using size_type = size_t;

    optimized_array()
    {
        array_t::init(this);
    }

    explicit optimized_array(std::initializer_list<ElementT> l)
    {
        array_t::init(this, l);
    }

    template <typename RngT>
    explicit optimized_array(RngT && rng, disable_if_same_ref_t<optimized_array, RngT> * dummy = nullptr)
    {
        array_t::init(this, std::forward<RngT>(rng));
    }

    template <typename RngT>
    optimized_array(RngT && rng, size_t sz)
    {
        array_t::init(this, std::forward<RngT>(rng), sz);
    }

    explicit optimized_array(size_t sz)
    {
        array_t::init(this, sz);
    }

    optimized_array(optimized_array const&) = default;
    optimized_array(optimized_array &&) = default;
    optimized_array& operator=(optimized_array const&) = default;
    optimized_array& operator=(optimized_array &&) = default;

    operator array_view<ElementT>() noexcept { return array_t::get(this); }
    operator array_view<const ElementT>() const noexcept { return array_t::get(this); }

    array_view<ElementT> to_array_view() noexcept { return array_t::get(this); }
    array_view<const ElementT> to_array_view() const noexcept { return array_t::get(this); }

    ElementT const* cbegin() const noexcept { return array_t::begin(this); }
    ElementT const* begin() const noexcept { return array_t::begin(this); }
    ElementT * begin() noexcept { return array_t::begin(this); }
    ElementT const* cend() const noexcept { return array_t::end(this); }
    ElementT const* end() const noexcept { return array_t::end(this); }
    ElementT * end() noexcept { return array_t::end(this); }

    size_t size() const noexcept { return array_t::size(this); }

    bool empty() const noexcept { return !array_t::size(this); }

    bool operator== (optimized_array const& rhs) const
    {
        return range_equal()(begin(), end(), rhs.begin(), rhs.end());
    }

    bool operator!= (optimized_array const& rhs) const
    {
        return !operator==(rhs);
    }

    bool operator< (optimized_array const& rhs) const
    {
        return range_less()(begin(), end(), rhs.begin(), rhs.end());
    }
};

template <typename ElementT, size_t ByteSzV, typename RefCountT>
class shared_optimized_array : public optimized_array<ElementT, ByteSzV, RefCountT>
{
    using base_t = optimized_array<ElementT, ByteSzV, RefCountT>;
    using holder_t = typename base_t::holder_t;

public:
    template <typename RngT>
    explicit shared_optimized_array(RngT && rng, disable_if_same_ref_t<shared_optimized_array, RngT> * dummy = nullptr)
        : base_t(std::forward<RngT>(rng))
    {}

    template <typename ... ArgsT>
    explicit shared_optimized_array(ArgsT&& ... args)
        : base_t(std::forward<ArgsT>(args) ...)
    {}

    shared_optimized_array(shared_optimized_array const& rhs)
        : base_t(in_place_type_t<holder_t>(), ref(static_cast<holder_t const&>(rhs)))
    {}

    shared_optimized_array& operator=(shared_optimized_array const& rhs)
    {
        holder_t::operator=(ref(static_cast<holder_t const&>(rhs)));
        return *this;
    }

    shared_optimized_array(shared_optimized_array &&) = default;
    shared_optimized_array& operator=(shared_optimized_array &&) = default;
};

template <typename ElementT, size_t ByteSzV, typename RefCountT>
array_view<const ElementT> to_array_view(shared_optimized_array<ElementT, ByteSzV, RefCountT> const& sa) { return sa.to_array_view(); }

template <typename ElementT, size_t ByteSzV, typename RefCountT>
array_view<ElementT> to_array_view(shared_optimized_array<ElementT, ByteSzV, RefCountT> & sa) { return sa.to_array_view(); }

}

#endif // SONIA_UTILITY_OPTIMIZED_ARRAY_HPP
