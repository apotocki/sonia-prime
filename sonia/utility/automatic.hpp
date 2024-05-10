//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <utility>
#include <iosfwd>

#include "sonia/type_traits.hpp"

namespace sonia {

template <typename T>
class automatic
{
public:
    using value_type = T;
    using reference = add_lvalue_reference_t<T>;
    using const_reference = add_lvalue_reference_t<add_const_t<T>>;

    // potentially dangerous
    automatic(null_t) {}

    automatic() { new (buffer_) T; }

    template <typename ... ArgsT>
    explicit automatic(in_place_t, ArgsT && ... args)
    {
        new (buffer_) T(std::forward<ArgsT>(args) ...);
    }

    template <typename ArgT>
    explicit automatic(ArgT && arg)
    {
        using pure_arg_t = remove_cvref_t<ArgT>;
        if constexpr (is_same_v<automatic, pure_arg_t>) {
            new (buffer_) T(*std::forward<ArgT>(arg));
        } else if constexpr (is_in_place_factory_v<pure_arg_t>) {
            arg.template apply<T>(buffer_);
        } else if constexpr (is_typed_in_place_factory_v<pure_arg_t>) {
            arg.apply(buffer_);
        } else {
            new (buffer_) T(std::forward<ArgT>(arg));
        }
    }

    automatic(automatic const& rhs)
    {
        new (buffer_) T(*rhs);
    }

    automatic(automatic && rhs)
    {
        new (buffer_) T(std::move(*rhs));
    }

    automatic & operator= (automatic const& rhs)
    {
        get() = rhs.get();
        return *this;
    }

    automatic & operator= (automatic && rhs)
    {
        get() = std::move(rhs.get());
        return *this;
    }

    template <typename ... ArgsT>
    T & emplace(ArgsT && ... args)
    {
        get() = T(std::forward<ArgsT>(args) ...);
        return get();
    }

    ~automatic()
    {
        std::destroy_at(get_pointer());
    }

    const_reference get() const& { return *get_pointer(); }
    reference & get() & { return *get_pointer(); }
    T && get() && { return std::move(*get_pointer()); }

    T const* get_pointer() const { return std::launder(reinterpret_cast<T const*>(buffer_)); }
    T * get_pointer() { return std::launder(reinterpret_cast<T*>(buffer_)); }

    const_reference operator*() const& { return get(); }
    reference operator*() & { return get(); }
    T && operator*() && { return get(); }

    T const* operator->() const { return get_pointer(); }
    T * operator->() { return get_pointer(); }

private:
    alignas(T) std::byte buffer_[sizeof(T)];
};

template <typename CharT, class TraitsT, typename T>
std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, automatic<T> val)
{
    return os << *val;
}

template <typename T> T * get_pointer(automatic<T> & p) { return p.get_pointer(); }
template <typename T> T const* get_pointer(automatic<T> const& p) { return p.get_pointer(); }

}
