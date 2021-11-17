// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR_DELETE
#   include "allocator_delete.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_FORWARD
#   include "../utility/forward.hpp"
#endif

#include "agnostic/stdext/allocate_new.hpp"

namespace std {

template <class T, class Alloc, class... Args>
unique_ptr<T, allocator_delete<T, Alloc>> allocate_unique(const Alloc& alloc, Args&&... args)
{
    return unique_ptr<T, allocator_delete<T, Alloc>>(
        stdext::allocate_new<T>(alloc, forward<Args>(args)..., alloc);
    );
}

}
