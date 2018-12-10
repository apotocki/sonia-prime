//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_ALLOCATOR_FACADE_HPP
#define SONIA_UTILITY_ALLOCATOR_FACADE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "in_place_factory.hpp"

// allocator facade wrappes an allocator tu support in_place factory as a construction mechanic
namespace sonia {

template <class AllocatorT>
class allocator_facade : public AllocatorT
{
public:
    using AllocatorT::AllocatorT;

    template <typename ArgT>
    void construct(void * ptr, ArgT && arg)
    {
        using clear_t = remove_cvref_t<ArgT>;
        if constexpr (is_base_of_v<boost::in_place_factory_base, clear_t>) {
            f.apply<typename AllocatorT::value_type>(ptr);
        } else if constexpr (is_base_of_v<boost::typed_in_place_factory_base, clear_t>) {
            do_construct(ptr, arg);
        } else {
            AllocatorT::construct(ptr, std::forward<ArgT>(arg));
        }
    }

    template <typename ... ArgsT>
    void construct(void * ptr, ArgsT && ... args)
    {
        AllocatorT::construct(ptr, std::forward<ArgsT>(args) ...);
    }

private:
    template <class TypedFactoryT>
    void do_construct(void * ptr, TypedFactoryT const& f)
    {
        using allocator_value_type = typename AllocatorT::value_type;
        using factory_value_type = typename TypedFactoryT::value_type;
        static_assert(is_same_v<allocator_value_type, factory_value_type>);
        
        f.apply(ptr);
    }
};

}

#include <memory>

namespace std {

template <class AllocatorT>
struct allocator_traits<sonia::allocator_facade<AllocatorT>>
    : allocator_traits<AllocatorT>
{
    using base_t = allocator_traits<AllocatorT>;

    template <typename T>
    using rebind_alloc = sonia::allocator_facade<typename base_t::template rebind_alloc<T>>;

    template <typename T>
    using rebind_traits = std::allocator_traits<rebind_alloc<T>>;
};

}

#endif // SONIA_UTILITY_ALLOCATOR_FACADE_HPP
