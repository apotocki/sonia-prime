//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_ITERATOR_TRAITS_HPP
#define SONIA_ITERATOR_TRAITS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_categories.hpp>

#include <boost/range.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/functional/has.hpp"

namespace sonia { namespace iterators {

HAS_MEMBER_TRAIT_DEF(empty);
HAS_MEMBER_TRAIT_DEF(flush);

} // sonia::iterators

using boost::iterator_value;
template <class IteratorT> using iterator_value_t = typename iterator_value<IteratorT>::type;

using boost::iterator_reference;
template <class IteratorT> using iterator_reference_t = typename iterator_reference<IteratorT>::type;

using boost::iterator_pointer;
template <class IteratorT> using iterator_pointer_t = typename iterator_pointer<IteratorT>::type;

using boost::iterator_difference;
template <class IteratorT> using iterator_difference_t = typename iterator_difference<IteratorT>::type;

using boost::iterator_category;
template <class IteratorT> using iterator_category_t = typename iterator_category<IteratorT>::type;

using boost::iterator_traversal;
template <class IteratorT> using iterator_traversal_t = typename iterator_traversal<IteratorT>::type;

template <class I, typename E = iterator_value_t<I>, typename = void>
class is_output_iterator : public false_type {};

template <class I, typename E>
class is_output_iterator<I, E, void_t<
    iterator_category_t<I>, decltype(*std::declval<I>() = std::declval<E>())>> : public true_type {};

template <class I, typename E = iterator_value_t<I>>
constexpr bool is_output_iterator_v = is_output_iterator<I, E>::value;

using boost::no_traversal_tag;
using boost::incrementable_traversal_tag;
using boost::single_pass_traversal_tag;
using boost::forward_traversal_tag;
using boost::bidirectional_traversal_tag;
using boost::random_access_traversal_tag;

using boost::range_iterator;
template <class RangeT> using range_iterator_t = typename range_iterator<RangeT>::type;

}

#endif // SONIA_ITERATOR_TRAITS_HPP
