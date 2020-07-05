// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#include "basic_fixed_size_allocator.hpp"

namespace agnostic {

template <size_t SzV, std::align_val_t AV = alignof(std::max_align_t)>
using default_fixed_size_allocator = basic_fixed_size_allocator<default_bare_allocator, SzV, AV>;

struct default_fixed_size_allocator_factory
{
    template <size_t SzV, std::align_val_t AV>
    using allocator_type = default_fixed_size_allocator<SzV, AV>;
    
    template <size_t SzV, std::align_val_t AV = alignof(std::max_align_t), typename ... Args>
    allocator_type<SzV, AV> create(Args && ... args)
    {
        return default_fixed_size_allocator<SzV, AV>{std::forward<Args>(args)...};
    }
};

}
