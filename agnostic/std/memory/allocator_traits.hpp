// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_CONSTRUCT_AT
#   include "construct_at.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_DESTROY_AT
#   include "destroy_at.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_POINTER_TRAITS
#   include "pointer_traits.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_FORWARD
#   include "../utility/forward.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_MAKE_UNSIGNED
#   include "../type_traits/make_unsigned.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_IS_EMPTY
#   include "../type_traits/is_empty.hpp"
#endif

namespace std {

namespace allocator_detail {

template <class Alloc, class = void> struct get_pointer_type{ using type = typename Alloc::value_type *; };
template <class Alloc> struct get_pointer_type<Alloc, void_t<typename Alloc::pointer>> { using type = typename Alloc::pointer; };

template <class Alloc, class = void> struct get_const_pointer_type
{
    using type = typename pointer_traits<typename get_pointer_type<Alloc>::type>::template rebind<const typename Alloc::value_type>;
};
template <class Alloc> struct get_const_pointer_type<Alloc, void_t<typename Alloc::pointer>> { using type = typename Alloc::pointer; };

template <class Alloc, class = void> struct get_void_pointer_type
{
    using type = typename pointer_traits<typename get_pointer_type<Alloc>::type>::template rebind<void>;
};
template <class Alloc> struct get_void_pointer_type<Alloc, void_t<typename Alloc::void_pointer>> { using type = typename Alloc::void_pointer; };

template <class Alloc, class = void> struct get_const_void_pointer_type
{
    using type = typename pointer_traits<typename get_pointer_type<Alloc>::type>::template rebind<const void>;
};
template <class Alloc> struct get_const_void_pointer_type<Alloc, void_t<typename Alloc::const_void_pointer>> { using type = typename Alloc::const_void_pointer; };

template <class Alloc, class = void> struct get_difference_type
{
    using type = typename pointer_traits<typename get_pointer_type<Alloc>::type>::difference_type;
};
template <class Alloc> struct get_difference_type<Alloc, void_t<typename Alloc::difference_type>> { using type = typename Alloc::difference_type; };

template <class Alloc, class = void> struct get_size_type
{
    using type = typename make_unsigned<typename allocator_detail::get_difference_type<Alloc>::type>::type;
};
template <class Alloc> struct get_size_type<Alloc, void_t<typename Alloc::size_type>> { using type = typename Alloc::size_type; };

template <class Alloc, class = void> struct get_propagate_on_container_copy_assignment : false_type {};
template <class Alloc> struct get_propagate_on_container_copy_assignment<Alloc, void_t<typename Alloc::propagate_on_container_copy_assignment>> { using type = typename Alloc::propagate_on_container_copy_assignment; };

template <class Alloc, class = void> struct get_propagate_on_container_move_assignment : false_type {};
template <class Alloc> struct get_propagate_on_container_move_assignment<Alloc, void_t<typename Alloc::propagate_on_container_move_assignment>> { using type = typename Alloc::propagate_on_container_move_assignment; };

template <class Alloc, class = void> struct get_propagate_on_container_swap : false_type {};
template <class Alloc> struct get_propagate_on_container_swap<Alloc, void_t<typename Alloc::propagate_on_container_swap>> { using type = typename Alloc::propagate_on_container_swap; };

template <class Alloc, class = void> struct get_is_always_equal : is_empty<Alloc> {};
template <class Alloc> struct get_is_always_equal<Alloc, void_t<typename Alloc::is_always_equal>> { using type = typename Alloc::is_always_equal; };

template <class U, class Alloc> struct replace_first_parameter;
template <class U, template <class, class...> class Alloc, class T, class ... Args> struct replace_first_parameter<U, Alloc<T, Args...>>
{
    using type = Alloc<U, Args...>;
};

template <class Alloc, class T, class = void> struct get_rebind_alloc : replace_first_parameter<T, Alloc> {};
template <class Alloc, class T> struct get_rebind_alloc<Alloc, T, void_t<typename Alloc::template rebind<T>::other>>
{ using type = typename Alloc::template rebind<T>::other; };

} // allocator_detail

template <class Alloc>
struct allocator_traits
{
    using allocator_type = Alloc;
    using value_type = typename Alloc::value_type;
    using pointer = typename allocator_detail::get_pointer_type<Alloc>::type;
    using const_pointer = typename allocator_detail::get_const_pointer_type<Alloc>::type;
    using void_pointer = typename allocator_detail::get_void_pointer_type<Alloc>::type;
    using const_void_pointer = typename allocator_detail::get_const_void_pointer_type<Alloc>::type;
    using difference_type = typename allocator_detail::get_difference_type<Alloc>::type;
    using size_type = typename allocator_detail::get_size_type<Alloc>::type;
    using propagate_on_container_copy_assignment = typename allocator_detail::get_propagate_on_container_copy_assignment<Alloc>::type;
    using propagate_on_container_move_assignment = typename allocator_detail::get_propagate_on_container_move_assignment<Alloc>::type;
    using propagate_on_container_swap = typename allocator_detail::get_propagate_on_container_swap<Alloc>::type;
    using is_always_equal = typename allocator_detail::get_is_always_equal<Alloc>::type;

    template <class T> using rebind_alloc = typename allocator_detail::get_rebind_alloc<Alloc, T>::type;
    template <class T> using rebind_traits = allocator_traits<rebind_alloc<T>>;

    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n)
    {
        return a.allocate(n);
    }

    [[nodiscard]] static constexpr pointer allocate(Alloc& a, size_type n, const_void_pointer hint)
    {
        return a.allocate(n, hint);
    }

    static constexpr void deallocate(Alloc& a, pointer p, size_type n)
    {
        a.deallocate(p, n);
    }

    template <class T, class... Args>
    static constexpr void construct(Alloc&, T* p, Args&&... args)
    {
        construct_at(p, forward<Args>(args)...);
    }

    template <class T>
    static constexpr void destroy(Alloc&, T* p)
    {
        destroy_at(p);
    }

    static constexpr size_type max_size(Alloc const& a) noexcept
    {
        return a.max_size();
        //return numeric_limits<size_type>::max() / sizeof(value_type);
    }

    static constexpr Alloc select_on_container_copy_construction(Alloc const& a)
    {
        return a.select_on_container_copy_construction();
    }
};

}
