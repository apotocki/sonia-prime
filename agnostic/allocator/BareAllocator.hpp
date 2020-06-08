// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_INTEGRAL_CONSTANT
#   include "agnostic/std/type_traits/integral_constant.hpp"
#endif

namespace agnostic {

template <typename T>
concept BareAllocator = 
    std::is_same_v<typename T::value_type, char> &&
    requires(T & a, T const& b) {
        {a.allocate(std::declval<std::align_val_t>(), std::declval<size_t>())} -> std::convertible_to<void*>;
        {a.allocate(std::declval<std::align_val_t>(), std::declval<size_t>(), std::nothrow)} -> std::convertible_to<void*>;
        {a.deallocate(std::declval<void*>(), std::declval<size_t>())} -> std::same_as<void>;
        {b.is_equal(std::declval<T const&>())} -> std::same_as<bool>;
    };

template <typename T, BareAllocator BA>
struct stl_allocator_adapter : private BA
{
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    template <class U> struct rebind { using other = stl_allocator_adapter<U, BA>; };
    using is_always_equal = std::false_type;

    explicit stl_allocator_adapter() = default;
    stl_allocator_adapter(BA const& rhs) : BA(rhs) {}

    [[nodiscard]] constexpr T* allocate(size_t n)
    {
        return std::launder(reinterpret_cast<T*>(BA::allocate((std::align_val_t)alignof(T), sizeof(T) * n)));
    }

    void deallocate(T* p, size_t n)
    {
        BA::deallocate(p, sizeof(T) * n);
    }
};

}

#ifndef DO_NOT_USE_AGNOSTIC_ALLOCATOR_TRAITS
#   include "agnostic/std/memory/allocator_traits.hpp"
#endif

namespace std {

template <agnostic::BareAllocator BA>
struct allocator_traits<BA>
{
    using allocator_type = BA;
    using value_type = char;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using void_pointer = void*;
    using const_void_pointer = const void*;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::false_type;
    using is_always_equal = std::false_type;
    template <class T> using rebind_alloc = agnostic::stl_allocator_adapter<T, BA>;
    template <class T> using rebind_traits = allocator_traits<rebind_alloc<T>>;

    [[nodiscard]] static pointer allocate(BA& a, size_type n)
    {
        return a.allocate(alignof(std::max_align_t), n);
    }

    [[nodiscard]] static pointer allocate(BA& a, size_type n, const_void_pointer hint)
    {
        return a.allocate(alignof(std::max_align_t), n);
    }

    static void deallocate(BA& a, pointer p, size_type n)
    {
        a.deallocate(p, n);
    }

    template <class T, class... Args>
    static void construct(BA&, T* p, Args&&... args)
    {
        construct_at(p, forward<Args>(args)...);
    }

    template <class T>
    static void destroy(BA&, T* p)
    {
        destroy_at(p);
    }

    static constexpr size_type max_size(BA const&) noexcept
    {
        return numeric_limits<size_type>::max();
    }

    static BA select_on_container_copy_construction(BA const& a)
    {
        return a;
    }
};

}
