// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ALLOCATE_NEW_HPP
#define AGNOSTIC_STD_ALLOCATE_NEW_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR_TRAITS
#   include "allocator_traits.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR_DELETE
#   include "allocator_delete.hpp"
#endif

namespace std {

template <class T, class Alloc, class... Args>
enable_if_t<
    is_same_v<T, typename allocator_traits<Alloc>::value_type>,
    typename allocator_traits<Alloc>::pointer
> allocate_new(const Alloc& alloc, Args&&... args)
{
    Alloc ncalloc(alloc);
    typename allocator_traits<Alloc>::pointer ptr = allocator_traits<Alloc>::allocate(ncalloc, 1);
    try {
        allocator_traits<Alloc>::construct(ncalloc, ptr, std::forward<Args>(args)...);
        return ptr;
    } catch (...) {
        allocator_traits<Alloc>::deallocate(ncalloc, ptr, 1);
        throw;
    }
}

template <class T, class Alloc, class... Args>
enable_if_t<
    !is_same_v<T, typename allocator_traits<Alloc>::value_type>,
    typename allocator_traits<Alloc>::pointer
> allocate_new(const Alloc& alloc, Args&&... args)
{
    return allocate_new<T, typename allocator_traits<Alloc>::template rebind_alloc<T>>(alloc, std::forward<Args>(args)...);
}

}

#endif // AGNOSTIC_STD_ALLOCATE_NEW_HPP
