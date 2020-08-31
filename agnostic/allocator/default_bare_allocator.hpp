// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#include <stdlib.h>

namespace agnostic {

struct default_bare_allocator
{
    using value_type = char;

    [[nodiscard]] void* allocate(std::align_val_t al, size_t sz)
    {
        if (void* result = allocate(al, sz, std::nothrow); result) {
            return result;
        }
        throw std::bad_alloc{};
    }

#ifdef _MSC_VER
    [[nodiscard]] char* allocate(std::align_val_t al, size_t sz, std::nothrow_t) noexcept
    {
        return reinterpret_cast<value_type*>(_aligned_malloc(sz, static_cast<size_t>(al)));
    }

    void deallocate(void* ptr, size_t) noexcept
    {
        _aligned_free(ptr);
    }

#else
    [[nodiscard]] char* allocate(std::align_val_t al, size_t sz, std::nothrow_t) noexcept
    {
        return reinterpret_cast<value_type*>(aligned_alloc(static_cast<size_t>(al), sz));
    }

    void deallocate(void* ptr, size_t) noexcept
    {
        free(ptr);
    }
#endif

    constexpr bool is_equal(default_bare_allocator const&) const noexcept { return true; }
};

}