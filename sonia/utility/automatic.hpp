//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_AUTOMATIC_HPP
#define SONIA_AUTOMATIC_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <iosfwd>
#include <boost/utility/in_place_factory.hpp>

#include "sonia/type_traits.hpp"

namespace sonia {

template <typename T>
class automatic
{
public:
    using value_type = T;

    // potentially dangerous
    automatic(null_t) {}

    automatic() { new (get_pointer()) T; }

    template <typename ... ArgsT>
    explicit automatic(in_place_t, ArgsT && ... args)
    {
        new (get_pointer()) T(std::forward<ArgsT>(args) ...);
    }

    template <typename ArgT>
    explicit automatic(ArgT && arg)
    {
        if constexpr (is_same_v<automatic, remove_cvref_t<ArgT>>) {
            new (get_pointer()) T(*std::forward<ArgT>(arg));
        } else if constexpr (is_base_of_v<boost::in_place_factory_base, remove_cvref_t<ArgT>>) {
            arg.template apply<T>(get_pointer());
        } else {
            new (get_pointer()) T(std::forward<ArgT>(arg));
        }
    }

    automatic(automatic const& rhs)
    {
        new (get_pointer()) T(*rhs);
    }

    automatic(automatic && rhs)
    {
        new (get_pointer()) T(std::move(*rhs));
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
        get_pointer()->~T();
    }

    T const& get() const& { return *get_pointer(); }
    T & get() & { return *get_pointer(); }
    T && get() && { return std::move(*get_pointer()); }

    T const* get_pointer() const { return std::launder(reinterpret_cast<T const*>(buffer_)); }
    T * get_pointer() { return std::launder(reinterpret_cast<T*>(buffer_)); }

    T const& operator*() const& { return get(); }
    T & operator*() & { return get(); }
    T && operator*() && { return get(); }

    T const* operator->() const { return get_pointer(); }
    T * operator->() { return get_pointer(); }

private:
    alignas(T) char buffer_[sizeof(T)];
};

template <typename CharT, class TraitsT, typename T>
std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, automatic<T> val) {
    return os << *val;
}

template <typename T> T * get_pointer(automatic<T> & p) { return p.get_pointer(); }
template <typename T> T const* get_pointer(automatic<T> const& p) { return p.get_pointer(); }

}

#endif // SONIA_AUTOMATIC_HPP
