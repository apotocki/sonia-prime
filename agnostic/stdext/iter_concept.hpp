// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#include "agnostic/stdext/is_from_primary.hpp"
#include "agnostic/std/iterator/iterator_traits.hpp"

namespace stdext {

using namespace std;

template <class IteratorT, bool = is_from_primary<iterator_traits<IteratorT>>>
struct iter_concept_impl {};

template <class IteratorT>
requires requires {typename iterator_traits<IteratorT>::iterator_concept; }
struct iter_concept_impl<IteratorT, false>
{
    using type = typename iterator_traits<IteratorT>::iterator_concept;
};

template <class IteratorT>
requires (!requires { typename iterator_traits<IteratorT>::iterator_concept; })
    && (requires { typename iterator_traits<IteratorT>::iterator_category; })
struct iter_concept_impl<IteratorT, false>
{
    using type = typename iterator_traits<IteratorT>::iterator_category;
};

template <class IteratorT>
struct iter_concept_impl<IteratorT, true>
{
    using type = random_access_iterator_tag;
};

template <class IteratorT>
requires requires { typename iterator_traits<IteratorT>::iterator_concept; }
struct iter_concept_impl<IteratorT, true>
{
    using type = typename IteratorT::iterator_concept;
};

template <class IteratorT>
requires (!requires { typename iterator_traits<IteratorT>::iterator_concept; })
    && (requires { typename iterator_traits<IteratorT>::iterator_category; })
struct iter_concept_impl<IteratorT, true>
{
    using type = typename IteratorT::iterator_category;
};

template <class IteratorT>
using iter_concept = typename iter_concept_impl<IteratorT>::type;

}
