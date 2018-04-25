//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_TYPE_TRAITS_HPP
#define SONIA_TYPE_TRAITS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <type_traits>
#include <boost/mpl/identity.hpp>

namespace sonia {

using std::true_type;
using std::false_type;

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

using std::is_same;
using std::is_same_v;

using std::is_base_of;
using std::is_base_of_v;

using std::is_const;
using std::is_const_v;

using std::is_rvalue_reference;
using std::is_rvalue_reference_v;

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

//using boost::mpl::identity;

//using std::remove_cvref;
//using std::remove_cvref_t;

template< class T >
struct remove_cvref {
    typedef remove_cv_t<remove_reference_t<T>> type;
};

template< class T >
using remove_cvref_t = typename remove_cvref<T>::type;

}

#endif // SONIA_TYPE_TRAITS_HPP
