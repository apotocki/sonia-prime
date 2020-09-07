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

template <typename T, typename BaseT>
struct adjacent_buffer : BaseT
{
    using value_type = T;
    using size_type = size_t;
    static constexpr size_t type_alignment = alignof(T);

    template <typename ... ArgsT>
    adjacent_buffer(uninitialized_t, size_type sz, ArgsT&& ... args)
        : BaseT{std::forward<ArgsT>(args) ...}, sz_(sz)
    {

    }

    template <typename ... ArgsT>
    adjacent_buffer(default_constructed_t, size_type sz, ArgsT&& ... args)
        : BaseT(std::forward<ArgsT>(args) ...), sz_(sz)
    {
        std::uninitialized_default_construct(begin(), end());
    }

    template <typename RngT, typename ... ArgsT>
    adjacent_buffer(RngT&& rng, size_type sz, ArgsT&& ... args)
        : BaseT(std::forward<ArgsT>(args) ...), sz_(sz)
    {
        std::uninitialized_copy(std::begin(rng), std::end(rng), begin());
    }

    adjacent_buffer(adjacent_buffer const& rhs) = delete;
    adjacent_buffer(adjacent_buffer&& rhs) = delete;
    adjacent_buffer& operator= (adjacent_buffer const&) = delete;
    adjacent_buffer& operator= (adjacent_buffer&&) = delete;

    ~adjacent_buffer() noexcept
    {
        BaseT::destroy();
    }

    T* begin() noexcept { return std::launder(reinterpret_cast<T*>(first_)); }
    T const* begin() const noexcept { return std::launder(reinterpret_cast<const T*>(first_)); }
    T const* cbegin() const noexcept { return begin(); }

    T* end() noexcept { return begin() + sz_; }
    T const* end() const noexcept { return begin() + sz_; }
    T const* cend() const noexcept { return end(); }

    T& front() noexcept { return *begin(); }
    T const& front() const noexcept { return *begin(); }

    T& back() noexcept { return *(end() - 1); }
    T const& back() const noexcept { return *(end() - 1); }

    T& operator[](size_t ind) noexcept { return begin()[ind]; }
    T const& operator[](size_t ind) const noexcept { return begin()[ind]; }


    inline size_type capacity() const noexcept { return sz_; }
    inline bool empty() const noexcept { return !sz_; }
    inline explicit operator bool() const noexcept { return !empty(); }

private:
    size_type sz_;
    alignas(T) char first_[sizeof(T)];
};

template <typename T, typename BaseT, BareAllocator AllocatorT, typename ... ArgsT>
[[nodiscard]] adjacent_buffer<T, BaseT>* allocate_adjacent_buffer(AllocatorT alloc, size_t sz, ArgsT&& ... args)
{
    assert(sz);

    using buffer_t = adjacent_buffer<T, BaseT>;

    //const size_t allocsz = offsetof(buffer_t, first_) + sizeof(T) * sz;
    const size_t allocsz = sizeof(buffer_t) + sizeof(T) * (sz - 1);

    buffer_t* ptr = reinterpret_cast<buffer_t*>(alloc.allocate((std::align_val_t)alignof(buffer_t), allocsz));

    try {
        return new (ptr) buffer_t{ uninitialized_t{}, sz, std::forward<ArgsT>(args) ... };
    } catch (...) {
        alloc.deallocate(ptr, allocsz);
        throw;
    }
}

template <BareAllocator AllocatorT, typename T, typename BaseT>
void deallocate_adjacent_buffer(AllocatorT alloc, adjacent_buffer<T, BaseT>* ptr) noexcept
{
    using buffer_t = adjacent_buffer<T, BaseT>;
    //const size_t allocsz = offsetof(buffer_t, first_) + sizeof(T) * ptr->capacity();
    const size_t allocsz = sizeof(buffer_t) + sizeof(T) * (ptr->capacity() - 1);

    std::destroy_at(ptr);
    alloc.deallocate(reinterpret_cast<char*>(ptr), allocsz);
}

}
