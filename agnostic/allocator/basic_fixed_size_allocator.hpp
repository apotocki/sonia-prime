// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#include "BareAllocator.hpp"

namespace agnostic {

//template <BareAllocator BA, size_t SzV, std::align_val_t AV = alignof(std::max_align_t)>
template <BareAllocator BA, size_t SzV, std::align_val_t AV>
struct basic_fixed_size_allocator : BA
{
    using BA::BA;

    [[nodiscard]] constexpr typename BA::value_type * allocate()
    {
        return BA::allocate(AV, SzV);
    }

    [[nodiscard]] constexpr typename BA::value_type* allocate(std::nothrow_t)
    {
        return BA::allocate(AV, SzV, std::nothrow);
    }

    void deallocate(void* ptr) noexcept
    {
        BA::deallocate(ptr, SzV);
    }
};

}
