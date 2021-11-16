// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ENABLE_IF
#   include "agnostic/std/type_traits/enable_if.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_SAME
#   include "agnostic/std/type_traits/is_same.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR_TRAITS
#   include "agnostic/std/memory/allocator_traits.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR_DELETE
#   include "agnostic/std/memory/allocator_delete.hpp"
#endif

namespace stdext {

using namespace std;

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
    return allocate_new<T, typename allocator_traits<Alloc>::template rebind_alloc<T>>(alloc, forward<Args>(args)...);
}

}
