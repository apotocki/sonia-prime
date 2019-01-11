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
public:
    using value_type = T;

    hazardous() {}
    ~hazardous() {}

    template <typename ... ArgsT>
    T & emplace(ArgsT && ... args)
    {
        T * place = get_pointer();
        new (place) T(std::forward<ArgsT>(args) ...);
        return *place;
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

}

#endif // SONIA_HAZARDOUS_HPP
