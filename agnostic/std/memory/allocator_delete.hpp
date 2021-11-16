// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ALLOCATOR_DELETE_HPP
#define AGNOSTIC_STD_ALLOCATOR_DELETE_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR_TRAITS
#   include "allocator_traits.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_MOVE
#   include "../utility/move.hpp"
#endif

namespace std {

template <class T, class AllocatorT>
struct allocator_delete 
    : private allocator_traits<AllocatorT>::template rebind_alloc<T>
{
    using allocator_t = typename allocator_traits<AllocatorT>::template rebind_alloc<T>;
    
    constexpr allocator_delete() noexcept = default;

    explicit allocator_delete(allocator_t const& arg) : allocator_t(arg) {}
    explicit allocator_delete(allocator_t&& arg) : allocator_t(std::move(arg)) {}

    template <class U, class A>
    allocator_delete(const allocator_delete<U, A>& d) : allocator_t(d.allocator()) {}

    allocator_t& allocator() const noexcept { return const_cast<allocator_t&>(static_cast<allocator_t const&>(*this)); }

    void operator()(T* ptr) const
    {
        allocator_traits<allocator_t>::destroy(allocator(), ptr);
        allocator_traits<allocator_t>::deallocate(allocator(), ptr, 1);
    }

    template <class U>
    void operator()(U* ptr) const = delete;
};

}

#endif // AGNOSTIC_STD_ALLOCATOR_DELETE_HPP
