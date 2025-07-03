//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include "sonia/type_traits.hpp"
#include "sonia/exceptions.hpp"

namespace sonia {

template <class T>
class NOVTABLE polymorphic_factory
{
public:
    virtual ~polymorphic_factory() = default;
    virtual T* create(void* address, size_t sz) const = 0;
};

class NOVTABLE polymorphic_clonable
{
public:
    virtual ~polymorphic_clonable() = default;
    [[noreturn]] virtual void* clone(void* address, size_t sz) const { THROW_NOT_SUPPORTED_ERROR("copy"); }
};

class NOVTABLE polymorphic_movable
{
public:
    virtual ~polymorphic_movable() = default;
    [[noreturn]] virtual void* move(void* address, size_t sz) { THROW_NOT_SUPPORTED_ERROR("move"); }
};

class NOVTABLE polymorphic_clonable_and_movable
{
public:
    virtual ~polymorphic_clonable_and_movable() = default;
    [[noreturn]] virtual void* clone(void* address, size_t sz) const { THROW_NOT_SUPPORTED_ERROR("copy"); }
    [[noreturn]] virtual void* move(void* address, size_t sz) { THROW_NOT_SUPPORTED_ERROR("move"); }
};

template <typename T>
concept clonable = requires(T a)
{
    { a.clone(nullptr, 0) } -> std::convertible_to<void*>;
};

template <typename T>
concept movable = requires(T a)
{
    { a.move(nullptr, 0) } -> std::convertible_to<void*>;
};

// Note: There is no standard way to test whether a function is declared virtual or not.
template <typename T> struct is_polymorphic_clonable : public false_type {};
template <clonable T> struct is_polymorphic_clonable<T> : public std::is_polymorphic<T> {};

template <typename T> struct is_polymorphic_movable : public false_type {};
template <movable T> struct is_polymorphic_movable<T> : public std::is_polymorphic<T> {};

template <typename T> constexpr bool is_polymorphic_clonable_v = is_polymorphic_clonable<T>::value;
template <typename T> constexpr bool is_polymorphic_movable_v = is_polymorphic_movable<T>::value;

#define SONIA_POLYMORPHIC_MOVABLE_IMPL(clsnm)                                               \
void* move(void* address, size_t sz) override                                               \
{                                                                                           \
    BOOST_ASSERT_MSG (sizeof(clsnm) <= sz,                                                  \
        ("%1% <= %2%"_fmt % sizeof(clsnm) % sz).str().c_str());                             \
    return new (address) clsnm{std::move(*this)};                                           \
}

#define SONIA_POLYMORPHIC_CLONABLE_IMPL(clsnm)                                              \
void* clone(void* address, size_t sz) const override                                        \
{                                                                                           \
    BOOST_ASSERT_MSG (sizeof(clsnm) <= sz,                                                  \
        ("%1% <= %2%"_fmt % sizeof(clsnm) % sz).str().c_str());                             \
    return new (address) clsnm{*this};                                                      \
}

#define SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(clsnm)                                      \
void* move(void* address, size_t sz) override                                               \
{                                                                                           \
    BOOST_ASSERT_MSG (sizeof(clsnm) <= sz,                                                  \
        ("%1% <= %2%"_fmt % sizeof(clsnm) % sz).str().c_str());                             \
    return new (address) clsnm{std::move(*this)};                                           \
}                                                                                           \
void* clone(void* address, size_t sz) const override                                        \
{                                                                                           \
    BOOST_ASSERT_MSG (sizeof(clsnm) <= sz,                                                  \
        ("%1% <= %2%"_fmt % sizeof(clsnm) % sz).str().c_str());                             \
    return new (address) clsnm{*this};                                                      \
}

}
