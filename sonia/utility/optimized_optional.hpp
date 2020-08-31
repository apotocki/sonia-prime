//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_OPTIMIZED_OPTIONAL_HPP
#define SONIA_UTILITY_OPTIMIZED_OPTIONAL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/optional.hpp"
#include "sonia/array_view.hpp"

namespace sonia {

template <typename T>
class optional_pointer
{
    T * value_;

public:
    constexpr explicit optional_pointer(T* val = nullptr) noexcept : value_(val) {}

    constexpr void reset() noexcept { value_ = nullptr; }

    explicit constexpr operator bool() const noexcept { return value; }

    constexpr T *& get() noexcept { return value_; }
    constexpr T *  get() const noexcept{ return value_; }

    constexpr T*& operator* () noexcept { return value_; }
    constexpr T*  operator* () const noexcept { return value_; }

    constexpr void operator= (T* val) noexcept { value_ = val; }
};

template <typename T> T * get_pointer(optional_pointer<T> const& opt) { return opt.get(); }

template <typename T>
class optional_array_view
{
    array_view<T> value_;

public:
    constexpr optional_array_view() = default;
    constexpr explicit optional_array_view(nullopt_t) {}
    constexpr explicit optional_array_view(array_view<T> val) noexcept : value_(val) {}

    optional_array_view & operator= (nullopt_t) { value_.reset(); return *this; }

    template <typename ... ArgsT>
    optional_array_view & emplace(ArgsT && ... args)
    {
        value_ = array_view<T>(std::forward<ArgsT>(args)...);
        return *this;
    }

    constexpr explicit operator bool() const noexcept { return !value_.begin(); }

    constexpr array_view<T> & get() noexcept { return value_; }
    constexpr array_view<T> get() const noexcept{ return value_; }

    constexpr array_view<T>& operator* () noexcept { return value_; }
    constexpr array_view<T> operator* () const noexcept { return value_; }

    constexpr array_view<T> * operator-> () noexcept { return &value_; }
    constexpr array_view<T> const* operator-> () const noexcept { return &value_; }
};

template <typename T> T * get_pointer(optional_array_view<T> const& opt) { return &*opt; }

template <typename T, typename EnablerT = void>
class optimized_optional_selector
{
    using type = optional<T>;
};

template <typename T>
class optimized_optional_selector<T*>
{
    using type = optional_pointer<T>;
};

template <typename T>
class optimized_optional_selector<array_view<T>>
{
    using type = optional_array_view<T>;
};

template <typename T>
using optimized_optional = typename optimized_optional_selector<T>::type;

}

#endif // SONIA_UTILITY_OPTIMIZED_OPTIONAL_HPP
