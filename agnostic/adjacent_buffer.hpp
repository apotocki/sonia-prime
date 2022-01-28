// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_IS_TRIVIALLY_DESTRUCTIBLE
#   include "agnostic/std/type_traits/is_trivially_destructible.hpp"
#endif

#include "agnostic/std/utility/forward.hpp"

#ifndef DO_NOT_USE_AGNOSTIC_UNINITIALIZED_COPY
#   include "agnostic/std/memory/uninitialized_copy.hpp"
#endif


#include "agnostic/std/new/launder.hpp"


#ifndef DO_NOT_USE_AGNOSTIC_UNINITIALIZED_DEFAULT_CONSTRUCT
#   include "agnostic/std/memory/uninitialized_default_construct.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ITERATOR_BEGIN
#   include "agnostic/std/iterator/begin.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ITERATOR_END
#   include "agnostic/std/iterator/end.hpp"
#endif

#include "agnostic/utility/ceiling.hpp"
#include "agnostic/utility/uninitialized.hpp"
#include "agnostic/utility/default_constructed.hpp"

#include "agnostic/allocator/BareAllocator.hpp"

namespace agnostic {

/*
struct Traits
{
    using value_type = unspecified_type;
    using size_type = unspecified_type;
    using base_t = unspecified_type;
    static constexpr size_type first = ...;
    static inline void const* first(base_t const&) { ... }
    static inline size_type size(base_t const&) { ... }
    static inline void set_size(base_t &, size_type) { ... }
    static void destroy(base_t&) noexcept { ... }
};
*/

template <typename T, typename BaseT, typename SizeT = size_t>
struct basic_adjacent_buffer_traits
{
    using value_type = T;
    using size_type = SizeT;

    struct base_t : BaseT {
        size_type size_;
        alignas(T) char first_[sizeof(T)];

        template <typename ... ArgsT>
        explicit base_t(size_type sz, ArgsT&& ... args) : BaseT(std::forward<ArgsT>(args)), size_{ sz } {}
    };

    static constexpr size_type first = offsetof(base_t, first_);
    static inline size_type size(base_t const& self) noexcept { return self.size_; }
    static inline void destroy(base_t&) noexcept { }
};

template <typename TraitsT>
struct adjacent_buffer : TraitsT::base_t
{
    using value_type = typename TraitsT::value_type;
    using size_type = typename TraitsT::size_type;
    using base_t = typename TraitsT::base_t;
    static constexpr size_t type_alignment = alignof(value_type);

    template <typename ... ArgsT>
    adjacent_buffer(uninitialized_t, size_type sz, ArgsT&& ... args)
        : base_t{sz, std::forward<ArgsT>(args) ...}
    {
    }

    template <typename ... ArgsT>
    adjacent_buffer(default_constructed_t, size_type sz, ArgsT&& ... args)
        : base_t(sz, std::forward<ArgsT>(args) ...)
    {
        std::uninitialized_default_construct(begin(), end());
    }

    template <typename RngT, typename ... ArgsT>
    adjacent_buffer(RngT&& rng, size_type sz, ArgsT&& ... args)
        : base_t(sz, std::forward<ArgsT>(args) ...)
    {
        std::uninitialized_copy(std::begin(rng), std::end(rng), begin());
    }

    adjacent_buffer(adjacent_buffer const& rhs) = delete;
    adjacent_buffer(adjacent_buffer&& rhs) = delete;
    adjacent_buffer& operator= (adjacent_buffer const&) = delete;
    adjacent_buffer& operator= (adjacent_buffer&&) = delete;

    ~adjacent_buffer() noexcept
    {
        TraitsT::destroy(*this);
    }
    
    inline value_type* begin() noexcept { return std::launder(reinterpret_cast<value_type*>(reinterpret_cast<uint8_t*>(this) + TraitsT::first)); }
    inline value_type const* begin() const noexcept { return std::launder(reinterpret_cast<value_type const*>(reinterpret_cast<uint8_t const*>(this) + TraitsT::first)); }
    inline value_type const* cbegin() const noexcept { return begin(); }

    inline value_type* end() noexcept { return begin() + capacity(); }
    inline value_type const* end() const noexcept { return begin() + capacity(); }
    inline value_type const* cend() const noexcept { return end(); }

    inline value_type& front() noexcept { return *begin(); }
    inline value_type const& front() const noexcept { return *begin(); }

    inline value_type& back() noexcept { return *(end() - 1); }
    inline value_type const& back() const noexcept { return *(end() - 1); }

    inline value_type& operator[](size_t ind) noexcept { return begin()[ind]; }
    inline value_type const& operator[](size_t ind) const noexcept { return begin()[ind]; }


    inline size_type capacity() const noexcept { return TraitsT::size(*this); }
    inline bool empty() const noexcept { return !capacity(); }
    inline explicit operator bool() const noexcept { return !empty(); }
};

template <typename TraitsT, BareAllocator AllocatorT, typename ... ArgsT>
[[nodiscard]] adjacent_buffer<TraitsT>* allocate_adjacent_buffer(AllocatorT alloc, typename TraitsT::size_type sz, ArgsT&& ... args)
{
    assert(sz);

    using buffer_t = adjacent_buffer<TraitsT>;
    constexpr size_t buffsz = sizeof(buffer_t);
    //const size_t allocsz = offsetof(buffer_t, first_) + sizeof(T) * sz;
    const size_t allocsz = (std::max)(buffsz, typename TraitsT::first + sizeof(typename TraitsT::value_type) * sz);
    
    buffer_t* ptr = reinterpret_cast<buffer_t*>(alloc.allocate((std::align_val_t)alignof(buffer_t), allocsz));

    try {
        return new (ptr) buffer_t{ uninitialized_t{}, sz, std::forward<ArgsT>(args) ... };
    } catch (...) {
        alloc.deallocate(ptr, allocsz);
        throw;
    }
}

template <BareAllocator AllocatorT, typename TraitsT>
void deallocate_adjacent_buffer(AllocatorT alloc, adjacent_buffer<TraitsT>* ptr) noexcept
{
    using buffer_t = adjacent_buffer<TraitsT>;
    //const size_t allocsz = offsetof(buffer_t, first_) + sizeof(T) * ptr->capacity();
    const size_t allocsz = sizeof(buffer_t) + sizeof(typename TraitsT::value_type) * (ptr->capacity() - 1);

    std::destroy_at(ptr);
    alloc.deallocate(reinterpret_cast<char*>(ptr), allocsz);
}

}
