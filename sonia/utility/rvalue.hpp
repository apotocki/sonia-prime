//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_RVALUE_WRAPPER_HPP
#define SONIA_UTILITY_RVALUE_WRAPPER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <functional>
#include "sonia/type_traits.hpp"

namespace sonia {

template <class T>
class rvalue_reference_wrapper
{
public:
    static_assert(std::is_object_v<T> || std::is_function_v<T>,
        "rvalue_reference_wrapper<T> requires T to be an object type "
        "or a function type.");

    using type = T;

    rvalue_reference_wrapper(T&& val) noexcept : ptr_(std::addressof(val)) {}

    operator T&&() const noexcept { return std::move(*ptr_); }

    T&& get() const noexcept { return std::move(*ptr_); }

    template <class ... ArgsT>
    auto operator()(ArgsT&&... args) const
        -> decltype(std::invoke(get(), std::forward<ArgsT>(args)...))
    {
        return (std::invoke(get(), std::forward<ArgsT>(args)...));
    }

    rvalue_reference_wrapper(T& val) = delete;

private:
    T * ptr_;
};

template <class T>
class rvalue_wrapper
{
public:
    static_assert(std::is_object_v<T> || std::is_function_v<T>,
        "rvalue_wrapper<T> requires T to be an object type "
        "or a function type.");

    using type = T;

    rvalue_wrapper(T&& val) noexcept : val_(std::move(val)) {}

    operator T&&() const noexcept { return std::move(val_); }

    T&& get() const noexcept { return std::move(val_); }

    template <class ... ArgsT>
    auto operator()(ArgsT&&... args) const
        -> decltype(std::invoke(get(), std::forward<ArgsT>(args)...))
    {
        return (std::invoke(get(), std::forward<ArgsT>(args)...));
    }

    rvalue_wrapper(T& val) = delete;

private:
    mutable T val_;
};


template <typename T>
enable_if_t<is_rvalue_reference_v<T&&>, rvalue_reference_wrapper<remove_reference_t<T>>> rvref(T && val) {
    return rvalue_reference_wrapper<remove_reference_t<T>>(std::forward<T>(val));
}

template <typename T>
rvalue_reference_wrapper<T> rvref(T & val) {
    return rvalue_reference_wrapper<T>(std::move(val));
}

}

#endif // SONIA_UTILITY_RVALUE_WRAPPER_HPP
