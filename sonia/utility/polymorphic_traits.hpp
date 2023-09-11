//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

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

template <typename T> struct is_polymorphic_clonable : public is_base_of<polymorphic_clonable, T> {};
template <typename T> struct is_polymorphic_movable : public is_base_of<polymorphic_movable, T> {};

template <typename T> constexpr bool is_polymorphic_clonable_v = is_polymorphic_clonable<T>::value;
template <typename T> constexpr bool is_polymorphic_movable_v = is_polymorphic_movable<T>::value;

#define SONIA_POLYMORPHIC_MOVABLE_IMPL(clsnm)                                               \
polymorphic_movable* move(void* address, size_t sz) override                                \
{                                                                                           \
    BOOST_ASSERT_MSG (sizeof(clsnm) <= sz,                                                  \
        ("%1% <= %2%"_fmt % sizeof(clsnm) % sz).str().c_str());                             \
    new (address) clsnm{std::move(*this)};                                                  \
    return reinterpret_cast<clsnm*>(address);                                               \
}

#define SONIA_POLYMORPHIC_CLONABLE_IMPL(clsnm)                                              \
polymorphic_clonable* clone(void* address, size_t sz) const override                        \
{                                                                                           \
    BOOST_ASSERT_MSG (sizeof(clsnm) <= sz,                                                  \
        ("%1% <= %2%"_fmt % sizeof(clsnm) % sz).str().c_str());                             \
    new (address) clsnm{*this};                                                             \
    return reinterpret_cast<clsnm*>(address);                                               \
}

#define SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(clsnm)                                      \
polymorphic_movable* move(void* address, size_t sz) override                                \
{                                                                                           \
    BOOST_ASSERT_MSG (sizeof(clsnm) <= sz,                                                  \
        ("%1% <= %2%"_fmt % sizeof(clsnm) % sz).str().c_str());                             \
    new (address) clsnm{std::move(*this)};                                                  \
    return reinterpret_cast<clsnm*>(address);                                               \
}                                                                                           \
polymorphic_clonable* clone(void* address, size_t sz) const override                        \
{                                                                                           \
    BOOST_ASSERT_MSG (sizeof(clsnm) <= sz,                                                  \
        ("%1% <= %2%"_fmt % sizeof(clsnm) % sz).str().c_str());                             \
    new (address) clsnm{*this};                                                             \
    return reinterpret_cast<clsnm*>(address);                                               \
}

}
