// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_POINTER_TRAITS_HPP
#define AGNOSTIC_STD_POINTER_TRAITS_HPP

#pragma once

namespace std {

namespace pointer_traits_detail {

template <class Ptr> struct get_element_type2;
template <template <class, class...> class Ptr, class T, class... Args> struct get_element_type2<Ptr<T, Args...> > { using type = T; };

template <class Ptr, class = void> struct get_element_type : get_element_type2<Ptr> {};
template <class Ptr> struct get_element_type<Ptr, void_t<typename Ptr::element_type>> { using type = typename Ptr::element_type; };

template <class Ptr, class = void> struct get_difference_type { using type = ptrdiff_t; };
template <class Ptr> struct get_difference_type<Ptr, void_t<typename Ptr::difference_type>> { using type = typename Ptr::difference_type; };


template <class U, class Ptr> struct replace_first_parameter;
template <class U, template <class, class...> class Ptr, class T, class ... Args> struct replace_first_parameter<U, Ptr<T, Args...>>
{ using type = Ptr<U, Args...>; };

template <class Ptr, class U, class = void> struct get_rebind { using type = typename replace_first_parameter<U, Ptr>::type; };
template <class Ptr, class U> struct get_rebind<Ptr, U, void_t<typename Ptr::template rebind<U>>> { using type = typename Ptr::template rebind<U>; };

}

template <class Ptr> struct pointer_traits
{
    using pointer = Ptr;
    using element_type = typename pointer_traits_detail::get_element_type<Ptr>::type;
    using difference_type = typename pointer_traits_detail::get_difference_type<Ptr>::type;

    template <class U> using rebind = typename pointer_traits_detail::get_rebind<Ptr, U>::type;

    static constexpr pointer pointer_to(element_type& r) noexcept { return Ptr::pointer_to(r); }
};

template <class T> struct pointer_traits<T*>
{
    using pointer = T*;
    using element_type = T;
    using difference_type = ptrdiff_t;

    template <class U> using rebind = U*;

    static constexpr pointer pointer_to(element_type& r) noexcept { return addressof(r); }
};

}

#endif // AGNOSTIC_STD_POINTER_TRAITS_HPP
