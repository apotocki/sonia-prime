//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_categories.hpp>

#include <boost/range/has_range_iterator.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/optional.hpp"
#include "sonia/utility/functional/has.hpp"

namespace sonia { namespace iterators {

HAS_METHOD_TRAIT_DEF(empty);
HAS_METHOD_TRAIT_DEF(flush);
HAS_METHOD_TRAIT_DEF(close);
HAS_METHOD_TRAIT_DEF(increment);
HAS_METHOD_TRAIT_DEF(decrement);
HAS_METHOD_TRAIT_DEF(next);
HAS_METHOD_TRAIT_DEF(base);
HAS_METHOD_TRAIT_DEF(name);
HAS_METHOD_TRAIT_DEF(set);

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

using boost::iterator_category_to_traversal;
template <class CategoryOrTraversalT> using iterator_category_to_traversal_t = typename iterator_category_to_traversal<CategoryOrTraversalT>::type;

template <class LimitTraversalT, class CategoryOrTraversalT>
using at_least_traversal_t = conditional_t<
    is_base_of_v<iterator_category_to_traversal_t<CategoryOrTraversalT>, LimitTraversalT>,
    iterator_category_to_traversal_t<CategoryOrTraversalT>,
    LimitTraversalT>;

template <class LimitTraversalT, class CategoryOrTraversalT>
using not_more_traversal_t = conditional_t<
    is_base_of_v<iterator_category_to_traversal_t<CategoryOrTraversalT>, LimitTraversalT>,
    LimitTraversalT,
    iterator_category_to_traversal_t<CategoryOrTraversalT>>;

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

using boost::range_value;
using boost::range_iterator;
using boost::range_const_iterator;

template <class RangeT> using range_value_t = typename range_value<RangeT>::type;
template <class RangeT> using range_iterator_t = typename range_iterator<RangeT>::type;
template <class RangeT> using range_const_iterator_t = typename range_const_iterator<RangeT>::type;
template <class RangeT> using range_iterator_value_t = iterator_value_t<range_iterator_t<RangeT>>;
template <class RangeT> using range_iterator_reference_t = iterator_reference_t<range_iterator_t<RangeT>>;

template <class IteratorT> using iterator_dereferenced_range_t = iterator_value_t<IteratorT>;
template <class IteratorT> using iterator_dereferenced_range_iterator_t = range_iterator_t<iterator_dereferenced_range_t<IteratorT>>;
template <class IteratorT> using iterator_dereferenced_range_iterator_value_t = iterator_value_t<iterator_dereferenced_range_iterator_t<IteratorT>>;
template <class IteratorT> using iterator_dereferenced_range_iterator_pointer_t = iterator_pointer_t<iterator_dereferenced_range_iterator_t<IteratorT>>;
template <class IteratorT> using iterator_dereferenced_range_iterator_reference_t = iterator_reference_t<iterator_dereferenced_range_iterator_t<IteratorT>>;

template <class T>
constexpr bool has_range_iterator_v = boost::has_range_iterator<T>::value;

template <typename IteratorT>
bool empty(IteratorT const& it)
{
    if constexpr (iterators::has_method_empty_v<IteratorT, bool()>) {
        return it.empty();
    } else {
        return it == IteratorT{};
    }
}

}
