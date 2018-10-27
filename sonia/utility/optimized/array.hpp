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
#include "sonia/utility/optimized/holder.hpp"
#include "sonia/utility/buffer.hpp"
#include "sonia/functional/mover.hpp"
#include "sonia/functional/range_equal.hpp"
#include "sonia/functional/range_less.hpp"

namespace sonia {

// inheritance sequence: adjacent_buffer -> DerivedBaseT -> optimized_array_base -> optimized_base

template <typename T, class DerivedBaseT = void, class AllocatorT = std::allocator<T>>
class optimized_array_base : public optimized_base
{
public:
    optimized_base * clone() const override;
    void dispose() noexcept override;
};

template <typename T, class DerivedBaseT, class AllocatorT>
optimized_base * optimized_array_base<T, DerivedBaseT, AllocatorT>::clone() const {
    typedef conditional_t<is_same_v<void, DerivedBaseT>, optimized_array_base, DerivedBaseT> base_derived_t;
    typedef adjacent_buffer<T, base_derived_t> derived_t;
    derived_t const& self = static_cast<derived_t const&>(*this);
    return allocate_adjacent_buffer<T, base_derived_t, AllocatorT>(self.size(), self.to_array_view(), self);
}

template <typename T, class DerivedBaseT, class AllocatorT>
void optimized_array_base<T, DerivedBaseT, AllocatorT>::dispose() noexcept {
    typedef conditional_t<is_same_v<void, DerivedBaseT>, optimized_array_base, DerivedBaseT> base_derived_t;
    typedef adjacent_buffer<T, base_derived_t> derived_t;
    derived_t & self = static_cast<derived_t&>(*this);
    self.~derived_t();
    deallocate_adjacent_buffer<AllocatorT>(&self);
}

template <class ElementT, class HolderT>
struct optimized_array_impl
{
    typedef std::allocator<ElementT> allocator_t;
    typedef optimized_array_base<ElementT> optimized_collection_base_t;
    typedef adjacent_buffer<ElementT, optimized_collection_base_t> optimized_collection_t;

    static const size_t alv = alignment_of_v<ElementT>;
    static const size_t aligned_offs = ceiling_v<HolderT::begin_offs, alv>;
    static const size_t gmaxsz = (sizeof(HolderT) - aligned_offs) / sizeof(ElementT); // guessed
    static const size_t szbitscnt = gmaxsz ? boost::static_log2<gmaxsz ? gmaxsz : 1>::value + 1 : 1;
    static const size_t sz_mask = (1 << szbitscnt) - 1;
    static const size_t aligned_byte_offs = ceiling_v<(HolderT::used_bits + szbitscnt + CHAR_BIT - 1) / CHAR_BIT, alv>;
    static const size_t maxsz = (sizeof(HolderT) - aligned_byte_offs) / sizeof(ElementT);

    static ElementT const* begin(HolderT const* h) noexcept { return (ElementT const*)(h->data() + aligned_byte_offs); }
    static ElementT * begin(HolderT * h) noexcept { return (ElementT*)(h->data() + aligned_byte_offs); }
    static ElementT const* end(HolderT const* h) noexcept { return begin(h) + maxsz; }
    static ElementT * end(HolderT * h) noexcept { return begin(h) + maxsz; }

    template <typename RngT>
    static void init(HolderT * self, RngT && rng, size_t sz) {
        if (sz <= maxsz) {
            self->set_uint(sz);
            construct(begin(self), std::forward<RngT>(rng), sz);
        } else {
            optimized_collection_t * ptr = allocate_adjacent_buffer<ElementT, optimized_collection_base_t, allocator_t>(
                sz, std::forward<RngT>(rng)
            );
            self->set_pointer(ptr);
        }
    }

    template <typename RngT>
    static void init(HolderT * self, RngT && rng) {
        init(self, std::forward<RngT>(rng), boost::size(rng));
    }

    static array_view<ElementT> get(HolderT * self) {
        return self->is_ptr() ? ptr(self)->to_array_view() 
            : array_view<ElementT>(begin(self), self->get_uint() & sz_mask);
    }

    static array_view<const ElementT> get(HolderT const* self) {
        return self->is_ptr() ? ptr(self)->to_array_view() 
            : array_view<const ElementT>(begin(self), self->get_uint() & sz_mask);
    }

    static size_t size(HolderT const* self) {
        return self->is_ptr() ? ptr(self)->size() : (self->get_uint() & sz_mask);
    }

protected:
    static optimized_collection_t * ptr(HolderT * self) {
        return static_cast<optimized_collection_t *>(self->get_pointer());
    }

    static optimized_collection_t const* ptr(HolderT const* self) {
        return static_cast<optimized_collection_t const*>(self->get_pointer());
    }
};

template <typename ElementT, size_t btsz>
class optimized_array : optimized_holder<btsz, 0> {
    typedef optimized_holder<btsz, 0> holder_t;
    typedef optimized_array_impl<ElementT, holder_t> array_t;

public:
    template <typename RngT>
    explicit optimized_array(RngT && rng) {
        array_t::init(this, std::forward<RngT>(rng));
    }

    template <typename RngT>
    optimized_array(RngT && rng, size_t sz) {
        array_t::init(this, std::forward<RngT>(rng), sz);
    }

    optimized_array(optimized_array const&) = default;
    optimized_array(optimized_array &&) = default;
    optimized_array& operator=(optimized_array const&) = default;
    optimized_array& operator=(optimized_array &&) = default;

    operator array_view<ElementT>() noexcept { return array_t::get(this); }
    operator array_view<const ElementT>() const noexcept { return array_t::get(this); }

    ElementT const* begin() const noexcept { return array_t::get(this).begin(); }
    ElementT * begin() noexcept { return array_t::get(this).begin(); }
    ElementT const* end() const noexcept { return array_t::get(this).end();; }
    ElementT * end() noexcept { return array_t::get(this).end(); }

    size_t size() const noexcept { return array_t::size(this); }

    bool operator== (optimized_array const& rhs) const {
        return range_equal()(begin(), end(), rhs.begin(), end());
    }

    bool operator< (optimized_array const& rhs) const {
        return range_less()(begin(), end(), rhs.begin(), end());
    }
};

}

#endif // SONIA_UTILITY_OPTIMIZED_ARRAY_HPP
