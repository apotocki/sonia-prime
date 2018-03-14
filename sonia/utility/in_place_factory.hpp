//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IN_PLACE_FACTORY_HPP
#define SONIA_IN_PLACE_FACTORY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/utility/in_place_factory.hpp>
#include <boost/utility/typed_in_place_factory.hpp>

#include <utility>
#include <memory>

#include "sonia/type_traits.hpp"
#include "sonia/utility/bind.hpp"

namespace sonia {

template <typename T>
struct callable_constructor {
    explicit callable_constructor(void * place) : place_(place) {}

    template <typename ... Args>
    T * operator()(Args&& ... args) const {
        return new(place_) T(std::forward<Args>(args)...);
    }
    
    void * place_;
};

template <typename ... ArgsT>
struct in_place_factory_impl : bind_tuple_t<ArgsT ...> {
    typedef bind_tuple_t<ArgsT ...> tuple_type;

    explicit in_place_factory_impl(ArgsT&& ... args) : tuple_type(std::forward<ArgsT>(args) ...) {}

    template <typename T, typename ... AdditionalArgsT>
    T * apply(void * place, AdditionalArgsT&& ... args) const {
        return std::apply(callable_constructor<T>(place), static_cast<tuple_type const&>(*this));
    }
};

template <class T, typename Enabler = void> struct is_in_place_factory : false_type {};
template <class T> constexpr bool is_in_place_factory_v = is_in_place_factory<T>::value;

template <class T> struct is_in_place_factory<T, enable_if_t<is_base_of_v<boost::in_place_factory_base, T>>> : true_type {};
template <typename ... ArgsT> struct is_in_place_factory<in_place_factory_impl<ArgsT...>> : true_type {};

template <class T> using is_in_place_factory_t = typename is_in_place_factory<T>::type;

template <typename ... ArgsT>
in_place_factory_impl<ArgsT ...> in_place_factory(ArgsT&& ... args) {
    return in_place_factory_impl<ArgsT ...>(std::forward<ArgsT>(args)...);
}

}

#endif // SONIA_IN_PLACE_FACTORY_HPP
