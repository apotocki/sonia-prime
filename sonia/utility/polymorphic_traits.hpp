//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_POLYMORPHIC_TRAITS_HPP
#define SONIA_UTILITY_POLYMORPHIC_TRAITS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/type_traits.hpp"
#include "sonia/exceptions.hpp"

namespace sonia {

class polymorphic_clonable
{
public:
    virtual ~polymorphic_clonable() {}
    virtual polymorphic_clonable* clone(void* address, size_t sz) const { throw not_supported_operation_error("copy"); }
};

class polymorphic_movable
{
public:
    virtual ~polymorphic_movable() {}
    virtual polymorphic_movable* move(void* address, size_t sz) { throw not_supported_operation_error("move"); }
};

template <typename T> struct is_polymorphic_clonable : public is_base_of<polymorphic_clonable, T> {};
template <typename T> struct is_polymorphic_movable : public is_base_of<polymorphic_movable, T> {};

template <typename T> constexpr bool is_polymorphic_clonable_v = is_polymorphic_clonable<T>::value;
template <typename T> constexpr bool is_polymorphic_movable_v = is_polymorphic_movable<T>::value;

}

#endif // SONIA_UTILITY_POLYMORPHIC_TRAITS_HPP
