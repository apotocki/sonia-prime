// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_ALLOCATE_UNIQUE_HPP
#define AGNOSTIC_STD_ALLOCATE_UNIQUE_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATE_NEW
#   include "allocate_new.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR_DELETE
#   include "allocator_delete.hpp"
#endif

namespace std {

template <class T, class Alloc, class... Args>
unique_ptr<T, allocator_delete<T, Alloc>> allocate_unique(const Alloc& alloc, Args&&... args)
{
    return unique_ptr<T, allocator_delete<T, Alloc>>(
        allocate_new<T>(alloc, std::forward<Args>(args)..., alloc);
    );
}

}

#endif // AGNOSTIC_STD_ALLOCATE_UNIQUE_HPP
