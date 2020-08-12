// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_ITERATOR_TRAITS

#include "agnostic/std/type_traits/void.hpp"
#include "agnostic/std/type_traits/remove_cv.hpp"
#include "agnostic/std/iterator/tags.hpp"

namespace std {

namespace iterator_traits_detail {

template <typename IterT, typename = void> struct get_pointer { using type = void; };
template <typename IterT> struct get_pointer<IterT, void_t<typename IterT::pointer>> { using type = typename IterT::pointer; };

}

template <class IterT, class = void> struct iterator_traits_impl {};

template <class IterT> struct iterator_traits_impl <IterT, void_t<
    typename IterT::value_type,
    typename IterT::reference,
    typename IterT::iterator_category,
    typename IterT::difference_type
    >>
{
    using value_type = typename IterT::value_type;
    using pointer = typename iterator_traits_detail::get_pointer<IterT>::type;
    using reference = typename IterT::reference;
    using iterator_category = typename IterT::iterator_category;
    using difference_type = typename IterT::difference_type;
};

template <class IterT> struct iterator_traits : iterator_traits_impl<IterT> { using _from_primary = iterator_traits; };

template <class T> struct iterator_traits<T*>
{
    using value_type = remove_cv_t<T>;
    using pointer = T*;
    using reference = T&;
    using iterator_category = random_access_iterator_tag;
    using difference_type = ptrdiff_t;
};

template <class T> struct iterator_traits<const T*>
{
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using iterator_category = random_access_iterator_tag;
    using difference_type = ptrdiff_t;
};

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <iterator>
#endif
