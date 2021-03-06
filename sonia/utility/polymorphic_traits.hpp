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

template <class T>
class polymorphic_factory
{
public:
    virtual ~polymorphic_factory() = default;
    virtual T* create(void* address, size_t sz) const = 0;
};

class polymorphic_clonable
{
public:
    virtual ~polymorphic_clonable() = default;
    [[noreturn]] virtual polymorphic_clonable* clone(void* address, size_t sz) const { THROW_NOT_SUPPORTED_ERROR("copy"); }
};

class polymorphic_movable
{
public:
    virtual ~polymorphic_movable() = default;
    [[noreturn]] virtual polymorphic_movable* move(void* address, size_t sz) { THROW_NOT_SUPPORTED_ERROR("move"); }
};

class polymorphic_clonable_and_movable
{
public:
    virtual ~polymorphic_clonable_and_movable() = default;
    [[noreturn]] virtual polymorphic_clonable_and_movable* clone(void* address, size_t sz) const { THROW_NOT_SUPPORTED_ERROR("copy"); }
    [[noreturn]] virtual polymorphic_clonable_and_movable* move(void* address, size_t sz) { THROW_NOT_SUPPORTED_ERROR("move"); }
};

template <typename T> struct is_polymorphic_clonable : public bool_constant<is_base_of_v<polymorphic_clonable, T> || is_base_of_v<polymorphic_clonable_and_movable, T>> {};
template <typename T> struct is_polymorphic_movable : public bool_constant<is_base_of_v<polymorphic_movable, T> || is_base_of_v<polymorphic_clonable_and_movable, T>> {};

template <typename T> constexpr bool is_polymorphic_clonable_v = is_polymorphic_clonable<T>::value;
template <typename T> constexpr bool is_polymorphic_movable_v = is_polymorphic_movable<T>::value;

#define SONIA_POLYMORPHIC_MOVABLE_IMPL(clsnm)                       \
polymorphic_movable* move(void* address, size_t sz) override        \
{                                                                   \
    BOOST_ASSERT (sizeof(clsnm) <= sz);                             \
    new (address) clsnm{std::move(*this)};                          \
    return reinterpret_cast<clsnm*>(address);                       \
}

#define SONIA_POLYMORPHIC_CLONABLE_IMPL(clsnm)                      \
polymorphic_clonable* clone(void* address, size_t sz) const override\
{                                                                   \
    BOOST_ASSERT (sizeof(clsnm) <= sz);                             \
    new (address) clsnm{*this};                                     \
    return reinterpret_cast<clsnm*>(address);                       \
}

#define SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(clsnm)                          \
polymorphic_clonable_and_movable* move(void* address, size_t sz) override       \
{                                                                               \
    BOOST_ASSERT (sizeof(clsnm) <= sz);                                         \
    new (address) clsnm{std::move(*this)};                                      \
    return reinterpret_cast<clsnm*>(address);                                   \
}                                                                               \
polymorphic_clonable_and_movable* clone(void* address, size_t sz) const override\
{                                                                               \
    BOOST_ASSERT (sizeof(clsnm) <= sz);                                         \
    new (address) clsnm{*this};                                                 \
    return reinterpret_cast<clsnm*>(address);                                   \
}

}

#endif // SONIA_UTILITY_POLYMORPHIC_TRAITS_HPP
