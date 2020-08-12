// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_OPTIMIZED_VECTOR_HPP
#define AGNOSTIC_OPTIMIZED_VECTOR_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ALIGNMENT_OF
#   include "agnostic/std/type_traits/alignment_of.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR_TRAITS
#   include "agnostic/std/memory/allocator_traits.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REVERSE_ITERATOR
#   include "agnostic/std/iterator/reverse_iterator.hpp"
#endif

#include "agnostic/utility/static_log2.hpp"
#include "agnostic/utility/ceiling.hpp"

#include "holder.hpp"

namespace agnostic {

namespace vector_detail {

template <typename ElementT, size_t StackCapacityV>
struct helper
{
    static constexpr size_t szbits = StackCapacityV ? 1 + static_log2_v<StackCapacityV ? StackCapacityV : 1> : 0;
    static constexpr size_t begin_offs = ((szbits + 1) + (CHAR_BIT - 1)) / CHAR_BIT;
    static constexpr size_t alv = StackCapacityV ? std::alignment_of_v<ElementT> : 1;
    static constexpr size_t aligned_offs = ceiling_v<begin_offs, alv>;
    static constexpr size_t estimated_holder_sz = aligned_offs + sizeof(ElementT) * StackCapacityV;
    static constexpr size_t holder_sz = estimated_holder_sz < sizeof(uintptr_t) ? sizeof(uintptr_t) : estimated_holder_sz;

    using holder_t = optimized_holder<holder_sz, szbits>;
};

}

template <typename ElementT, size_t StackCapacityV, typename AllocatorT>
class vector
    : vector_detail::helper<ElementT, StackCapacityV>::holder_t
{
    using helper_t = vector_detail::helper<ElementT, StackCapacityV>;
    using holder_t = typename helper_t::holder_t;

public:
    using value_type = ElementT;
    using allocator_type = AllocatorT;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = typename std::allocator_traits<AllocatorT>::pointer;
    using const_pointer = typename std::allocator_traits<AllocatorT>::const_pointer;
    using iterator = ElementT*;
    using const_iterator = ElementT const*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    vector() noexcept
    {
        // set size bits to 0;
        std::memset(this->data(), 0, helper_t::begin_offs);
        holder_t::init_not_ptr();
    }

    size_t size() const noexcept
    {
        if (holder_t::is_ptr()) {
            return 0xff;
        }
        return static_cast<size_t>(holder_t::get_service_cookie());
    }
};

}

#endif // AGNOSTIC_OPTIMIZED_VECTOR_HPP
