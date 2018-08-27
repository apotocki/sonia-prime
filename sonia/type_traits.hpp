//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_TYPE_TRAITS_HPP
#define SONIA_TYPE_TRAITS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <type_traits>
#include <boost/mpl/identity.hpp>
#include <boost/range.hpp>

namespace sonia {

using std::true_type;
using std::false_type;

using std::void_t;

using std::enable_if;
using std::enable_if_t;

template <bool Test, class T = void> using disable_if = enable_if<!Test, T>;
template <bool Test, class T = void> using disable_if_t = enable_if_t<!Test, T>;

using std::enable_if_t;

using std::is_integral;
using std::is_integral_v;

using std::is_signed;
using std::is_signed_v;

using std::is_unsigned;
using std::is_unsigned_v;

using std::is_floating_point;
using std::is_floating_point_v;

using std::is_same;
using std::is_same_v;

using std::is_base_of;
using std::is_base_of_v;

using std::is_const;
using std::is_const_v;

using std::is_pointer;
using std::is_pointer_v;

using std::is_rvalue_reference;
using std::is_rvalue_reference_v;

using std::is_pod;
using std::is_pod_v;

using std::is_trivially_destructible;
using std::is_trivially_destructible_v;

using std::has_virtual_destructor;
using std::has_virtual_destructor_v;

using std::aligned_storage;
using std::aligned_storage_t;
using std::alignment_of;
using std::alignment_of_v;

using std::remove_cv;
using std::remove_cv_t;
using std::remove_reference;
using std::remove_reference_t;
using std::add_const;
using std::add_const_t;
using std::add_pointer;
using std::add_pointer_t;
using std::add_lvalue_reference;
using std::add_lvalue_reference_t;
using std::add_rvalue_reference;
using std::add_rvalue_reference_t;

using std::conditional;
using std::conditional_t;

using std::in_place;
using std::in_place_t;
using std::in_place_type;
using std::in_place_type_t;

using boost::mpl::identity;

//using std::remove_cvref;
//using std::remove_cvref_t;

template <class T>
struct remove_cvref {
    typedef remove_cv_t<remove_reference_t<T>> type;
};

template <class T> using remove_cvref_t = typename remove_cvref<T>::type;

enum class endian
{
#ifdef _WIN32
    little = 0,
    big    = 1,
    native = little
#else
    little = __ORDER_LITTLE_ENDIAN__,
    big    = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
};

// range
using boost::range_value;
template <class T> using range_value_t = typename range_value<T>::type;

using boost::range_iterator;
template <class T> using range_iterator_t = typename range_iterator<T>::type;

template <size_t val, size_t acc>
constexpr size_t ceiling_v = ((val + acc - 1) / acc) * acc;

}

#endif // SONIA_TYPE_TRAITS_HPP
