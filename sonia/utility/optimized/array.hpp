//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <iosfwd>
#include <memory>

#include "sonia/string.hpp"
#include "sonia/type_traits.hpp"
#include "sonia/utility/buffer.hpp"
#include "sonia/utility/optimized/holder.hpp"
#include "sonia/utility/functional/mover.hpp"
#include "sonia/utility/functional/range_equal.hpp"
#include "sonia/utility/functional/range_less.hpp"
#include "sonia/utility/serialization/serialization_fwd.hpp"

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
    using element_t = remove_cv_t<ElementT>;
    using allocator_t = std::allocator<element_t>;
    using optimized_collection_base_t = optimized_array_base<element_t, typename HolderT::refcount_t>;
    using optimized_collection_t = adjacent_buffer<element_t, optimized_collection_base_t>;

    static constexpr size_t alv = alignment_of_v<ElementT>;
    static constexpr size_t aligned_offs = ceiling_v<HolderT::begin_offs, alv>;
    static constexpr size_t gmaxsz = (sizeof(HolderT) - aligned_offs) / sizeof(ElementT); // guessed
    static constexpr size_t szbitscnt = gmaxsz ? boost::static_log2<gmaxsz ? gmaxsz : 1>::value + 1 : 1;
    static constexpr size_t sz_mask = (1 << szbitscnt) - 1;
    static constexpr size_t aligned_byte_offs = ceiling_v<(HolderT::used_bits + szbitscnt + CHAR_BIT - 1) / CHAR_BIT, alv>;
    static constexpr size_t maxsz = (sizeof(HolderT) - aligned_byte_offs) / sizeof(ElementT);

    static element_t const* local_begin(HolderT const* h) noexcept { return (element_t const*)(h->data() + aligned_byte_offs); }
    static ElementT * local_begin(HolderT * h) noexcept { return (ElementT*)(h->data() + aligned_byte_offs); }
    static element_t const* local_end(HolderT const* h) noexcept { return begin(h) + (h->get_uint() & sz_mask); }
    static ElementT * local_end(HolderT * h) noexcept { return begin(h) + (h->get_uint() & sz_mask); }

    static void init(HolderT * self)
    {
        self->set_uint(0);
    }

    static void init(HolderT * self, size_t sz)
    {
        if (sz <= maxsz) {
            self->set_uint(sz);
            construct(ncbegin(self), sz);
        } else {
            optimized_collection_t * ptr = allocate_adjacent_buffer<element_t, optimized_collection_base_t>(allocator_t(), sz, in_place);
            self->set_pointer(ptr);
        }
    }

    template <typename RngT>
    static void init(HolderT * self, RngT && rng, size_t sz)
    {
        if (sz <= maxsz) {
            self->set_uint(sz);
            construct(ncbegin(self), std::forward<RngT>(rng), sz);
        } else {
            optimized_collection_t * ptr = allocate_adjacent_buffer<element_t, optimized_collection_base_t>(
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

    static element_t const* begin(HolderT const* self)
    {
        return self->is_ptr() ? ptr(self)->begin() : local_begin(self);
    }

    static ElementT* end(HolderT * self)
    {
        return self->is_ptr() ? ptr(self)->end() : local_end(self);
    }

    static element_t const* end(HolderT const* self)
    {
        return self->is_ptr() ? ptr(self)->end() : local_end(self);
    }

    static array_view<ElementT> get(HolderT * self)
	{
        return self->is_ptr() ? array_view<ElementT>{ptr(self)->to_array_view()}
            : array_view(local_begin(self), self->get_uint() & sz_mask);
    }

    static array_view<const element_t> get(HolderT const * self)
    {
        return self->is_ptr() ? ptr(self)->to_array_view()
            : array_view(local_begin(self), self->get_uint() & sz_mask);
    }

    static ElementT& front(HolderT * self)
    {
        return *begin(self);
    }

    static element_t const& front(HolderT const* self)
    {
        return *begin(self);
    }

    static ElementT& back(HolderT * self)
    {
        return *(end(self) - 1);
    }

    static element_t const& back(HolderT const* self)
    {
        return *(end(self) - 1);
    }

    static size_t size(HolderT const* self)
	{
        return self->is_ptr() ? ptr(self)->size() : (self->get_uint() & sz_mask);
    }

    static element_t * local_ncbegin(HolderT * h) noexcept { return (element_t*)(h->data() + aligned_byte_offs); }

    static element_t * ncbegin(HolderT * self)
    {
        return self->is_ptr() ? ptr(self)->begin() : local_ncbegin(self);
    }

    static array_view<element_t> ncget(HolderT * self)
    {
        return self->is_ptr() ? ptr(self)->to_array_view()
            : array_view(local_ncbegin(self), self->get_uint() & sz_mask);
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
    SONIA_DECLARE_SERIALIZATION_FRIENDLY;

protected:
    using holder_t = typename optimized_array::optimized_holder_t;
    using array_t = optimized_array_impl<ElementT, holder_t>;
    using element_t = typename array_t::element_t;

    holder_t & holder() { return *this; }
    holder_t const& holder() const { return *this; }

    template <class HolderArgT>
    optimized_array(in_place_type_t<holder_t>, HolderArgT && harg)
        : holder_t(std::forward<HolderArgT>(harg))
    {}

public:
    using value_type = ElementT;
    using iterator = ElementT *;
    using const_iterator = element_t const*;
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

    template <typename ET, size_t BSzV, typename RCT, typename SelfET = ElementT>
    optimized_array(optimized_array<ET, BSzV, RCT> const& rhs, enable_if_t<is_const_v<SelfET> && is_same_v<ET, remove_cv_t<ElementT>>> * enabler = nullptr)
        : holder_t(static_cast<holder_t const&>(rhs))
    {}

    template <typename ET, size_t BSzV, typename RCT, typename SelfET = ElementT>
    optimized_array(optimized_array<ET, BSzV, RCT> && rhs, enable_if_t<is_const_v<SelfET> && is_same_v<ET, remove_cv_t<ElementT>>> * enabler = nullptr)
        : holder_t(std::move(static_cast<holder_t&>(rhs)))
    {}

    optimized_array(optimized_array const&) = default;
    optimized_array(optimized_array &&) = default;
    optimized_array& operator=(optimized_array const&) = default;
    optimized_array& operator=(optimized_array &&) = default;

    optimized_array& operator=(array_view<const element_t> rhs)
    {
        holder_t::reset();
        array_t::init(this, rhs, rhs.size());
        return *this;
    }

    template <typename ET, size_t BSzV, typename RCT, typename SelfET = ElementT>
    enable_if_t<is_const_v<SelfET> && is_same_v<ET, remove_cv_t<ElementT>>, optimized_array&> operator=(optimized_array<ET, BSzV, RCT> const& rhs)
    {
        holder_t::operator= (static_cast<holder_t const&>(rhs));
        return *this;
    }

    template <typename ET, size_t BSzV, typename RCT, typename SelfET = ElementT>
    enable_if_t<is_const_v<SelfET> && is_same_v<ET, remove_cv_t<ElementT>>, optimized_array&> operator=(optimized_array<ET, BSzV, RCT> && rhs)
    {
        holder_t::operator= (std::move(static_cast<holder_t&>(rhs)));
        return *this;
    }

    operator array_view<ElementT>() noexcept { return array_t::get(this); }
    operator array_view<const element_t>() const noexcept { return array_t::get(this); }

    array_view<ElementT> to_array_view() noexcept { return array_t::get(this); }
    array_view<const element_t> to_array_view() const noexcept { return array_t::get(this); }

    element_t const* cbegin() const noexcept { return array_t::begin(this); }
    element_t const* begin() const noexcept { return array_t::begin(this); }
    ElementT * begin() noexcept { return array_t::begin(this); }
    element_t const* cend() const noexcept { return array_t::end(this); }
    element_t const* end() const noexcept { return array_t::end(this); }
    ElementT * end() noexcept { return array_t::end(this); }

    element_t const& front() const noexcept { return array_t::front(this); }
    ElementT & front() noexcept { return array_t::front(this); }
    element_t const& back() const noexcept { return array_t::back(this); }
    ElementT & back() noexcept { return array_t::back(this); }

    element_t const& operator[] (size_t idx) const noexcept { return cbegin()[idx]; }
    ElementT & operator[] (size_t idx) noexcept { return begin()[idx]; }

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
    SONIA_DECLARE_SERIALIZATION_FRIENDLY;

    using base_t = optimized_array<ElementT, ByteSzV, RefCountT>;
    using holder_t = typename base_t::holder_t;

    template <typename, size_t, typename> friend class shared_optimized_array;

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
        : base_t(in_place_type_t<holder_t>(), ref(rhs.holder()))
    {}

    template <typename ET, size_t BSzV, typename RCT, typename SelfET = ElementT>
    shared_optimized_array(shared_optimized_array<ET, BSzV, RCT> const& rhs, enable_if_t<is_const_v<SelfET> && is_same_v<ET, remove_cv_t<ElementT>>> * enabler = nullptr)
        : base_t(in_place_type_t<holder_t>(), ref(rhs.holder()))
    {}

    shared_optimized_array& operator=(shared_optimized_array const& rhs)
    {
        holder_t::operator=(ref(rhs.holder()));
        return *this;
    }

    template <typename ET, size_t BSzV, typename RCT, typename SelfET = ElementT>
    enable_if_t<is_const_v<SelfET> && is_same_v<ET, remove_cv_t<ElementT>>, shared_optimized_array&> operator=(shared_optimized_array<ET, BSzV, RCT> const& rhs)
    {
        holder_t::operator= (ref(rhs.holder()));
        return *this;
    }

    shared_optimized_array(shared_optimized_array &&) = default;
    shared_optimized_array& operator=(shared_optimized_array &&) = default;

    using base_t::operator=;
};

template <typename CharT, class TraitsT, typename ElementT, size_t ByteSzV, typename RefCountT>
std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, shared_optimized_array<ElementT, ByteSzV, RefCountT> const& arr)
{
    return os << arr.to_array_view();
}

template <typename ElementT, size_t ByteSzV, typename RefCountT>
inline size_t hash_value(shared_optimized_array<ElementT, ByteSzV, RefCountT> const& sa)
{
    return hasher()(sa.to_array_view());
}

}
