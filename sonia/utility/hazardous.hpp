//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_HAZARDOUS_HPP
#define SONIA_HAZARDOUS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <iosfwd>
#include <boost/utility/in_place_factory.hpp>

#include "sonia/type_traits.hpp"

namespace sonia {

template <typename T>
class hazardous
{
    hazardous(hazardous const&) = delete;
    hazardous(hazardous &&) = delete;
    hazardous& operator=(hazardous const&) = delete;
    hazardous& operator=(hazardous &&) = delete;

public:
    using value_type = T;

    hazardous() = default;
    ~hazardous() noexcept = default;

    template <typename ArgT>
    T & emplace(ArgT && arg)
    {
        T * place = get_pointer();
        if constexpr (is_in_place_factory_v<remove_cvref_t<ArgT>>) {
            arg.template apply<T>(place);
        } else {
            new (place) T(std::forward<ArgT>(arg));
        }
        return *place;
    }

    template <typename ArgT0, typename ArgT1, typename ... ArgsT>
    T & emplace(ArgT0 && arg0, ArgT1 && arg1, ArgsT && ... args)
    {
        T * place = get_pointer();
        new (place) T(std::forward<ArgT0>(arg0), std::forward<ArgT1>(arg1), std::forward<ArgsT>(args) ...);
        return *place;
    }

    template <typename ... ArgsT>
    T & replace(ArgsT && ... args)
    {
        get() = T(std::forward<ArgsT>(args) ...);
        return get();
    }

    void destroy()
    {
        std::destroy_at(get_pointer());
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
std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, hazardous<T> val)
{
    return os << *val;
}

template <typename T> T * get_pointer(hazardous<T> & p) { return p.get_pointer(); }
template <typename T> T const* get_pointer(hazardous<T> const& p) { return p.get_pointer(); }

template <typename T>
void hazardous_copy_as_optional(hazardous<T> const& src, bool has_src, hazardous<T> & dest, bool has_dest)
{
    if (has_dest) {
        if (has_src) {
            *dest = *src;
        } else {
            dest.destroy();
        }
    } else if (has_src) {
        dest.emplace(*src);
    }
}

template <typename T>
void hazardous_move_as_optional(hazardous<T> & src, bool has_src, hazardous<T> & dest, bool has_dest)
{
    if (has_dest) {
        if (has_src) {
            *dest = std::move(*src);
        } else {
            dest.destroy();
        }
    }
    if (has_src) {
        dest.emplace(std::move(*src));
    }
}

template <typename T>
void hazardous_destroy_as_optional(hazardous<T> & target, bool has_target)
{
    if (has_target) {
        target.destroy();
    }
}

}

#endif // SONIA_HAZARDOUS_HPP
