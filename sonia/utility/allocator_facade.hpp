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
    using value_type = typename AllocatorT::value_type;

    template <typename ... ArgsT>
    inline void construct(value_type * ptr, ArgsT && ... args)
    {
        if constexpr (sizeof ...(ArgsT) != 1) {
            std::allocator_traits<AllocatorT>::construct(*this, ptr, std::forward<ArgsT>(args) ...);
        } else {
            do_construct(ptr, std::forward<ArgsT>(args) ...);
        }
    }

private:
    template <typename ArgT>
    inline void do_construct(value_type * ptr, ArgT && arg)
    {
        using clear_t = remove_cvref_t<ArgT>;
        if constexpr (is_base_of_v<boost::in_place_factory_base, clear_t>) {
            arg.apply<typename AllocatorT::value_type>(ptr);
        } else if constexpr (is_base_of_v<boost::typed_in_place_factory_base, clear_t>) {
            using allocator_value_type = typename AllocatorT::value_type;
            using factory_value_type = typename TypedFactoryT::value_type;
            static_assert(is_base_of_v<allocator_value_type, factory_value_type>);

            f.apply(ptr);
            //do_construct(ptr, arg);
        } else {
            std::allocator_traits<AllocatorT>::construct(*this, ptr, std::forward<ArgT>(arg));
        }
    }

    //template <class TypedFactoryT>
    //void do_construct(value_type * ptr, TypedFactoryT const& f)
    //{
    //    using allocator_value_type = typename AllocatorT::value_type;
    //    using factory_value_type = typename TypedFactoryT::value_type;
    //    static_assert(is_base_of_v<allocator_value_type, factory_value_type>);
    //    
    //    f.apply(ptr);
    //}
};

}

#include <memory>

namespace std {

template <class AllocatorT>
struct allocator_traits<sonia::allocator_facade<AllocatorT>>
    : allocator_traits<AllocatorT>
{
    using base_t = allocator_traits<AllocatorT>;
    using alloc_t = sonia::allocator_facade<AllocatorT>;
    using value_type = typename base_t::value_type;

    template <typename T>
    using rebind_alloc = sonia::allocator_facade<typename base_t::template rebind_alloc<T>>;

    template <typename T>
    using rebind_traits = std::allocator_traits<rebind_alloc<T>>;

    template <typename ... ArgsT>
    static inline void construct(alloc_t& alloc, value_type * ptr, ArgsT &&... args)
    {
        alloc.construct(ptr, std::forward<ArgsT>(args) ...);
    }
};

}

#endif // SONIA_UTILITY_ALLOCATOR_FACADE_HPP
