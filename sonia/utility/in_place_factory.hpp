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
struct callable_constructor
{
    explicit callable_constructor(void * place) : place_(place) {}

    template <typename ... Args>
    T * operator()(Args&& ... args) const
    {
        return new(place_) T(std::forward<Args>(args)...);
    }
    
    void * place_;
};

template <typename FunctorT>
struct in_place_functor_factory
    : public boost::in_place_factory_base
{
public:
    explicit in_place_functor_factory(FunctorT const& f) : ftor_{f} {}

    template <typename ProductT> void * apply(void * vplace) const
    {
        ProductT * place = reinterpret_cast<ProductT*>(vplace);
        ftor_(place);
        return vplace;
    }

private:
    FunctorT ftor_;
};


template <typename ... ArgsT>
struct in_place_factory_impl : bind_tuple_t<ArgsT ...>
{
    typedef bind_tuple_t<ArgsT ...> tuple_type;

    explicit in_place_factory_impl(ArgsT&& ... args) : tuple_type(std::forward<ArgsT>(args) ...) {}

    template <typename T, typename ... AdditionalArgsT>
    T * apply(void * place, AdditionalArgsT&& ...) const
    {
        return std::apply(callable_constructor<T>(place), static_cast<tuple_type const&>(*this));
    }
};

template <typename ... ArgsT> struct is_in_place_factory<in_place_factory_impl<ArgsT...>> : true_type {};

template <class T> using is_in_place_factory_t = typename is_in_place_factory<T>::type;

template <typename ... ArgsT>
in_place_factory_impl<ArgsT ...> in_place_factory(ArgsT&& ... args)
{
    return in_place_factory_impl<ArgsT ...>(std::forward<ArgsT>(args)...);
}

}

#endif // SONIA_IN_PLACE_FACTORY_HPP
