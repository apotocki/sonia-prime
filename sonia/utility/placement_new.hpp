//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_PLACEMENT_NEW_HPP
#define SONIA_PLACEMENT_NEW_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <memory>

#include "sonia/type_traits.hpp"
#include "sonia/utility/in_place_factory.hpp"

namespace sonia {

template <typename T, typename ArgT, typename EnablerT = void>
struct placement_new_impl {
    template <typename CArgT>
    T * operator()(void * place, CArgT && arg) const {
        return new(place) T(std::forward<CArgT>(arg));
    }
};

template <typename T, typename ArgT>
struct placement_new_impl<T, ArgT, enable_if_t<is_in_place_factory_v<ArgT>>> {
    template <typename FactoryT>
    T * operator()(void * place, FactoryT const& factory) const {
        return reinterpret_cast<T*>(factory.template apply<T>(place));
    }
};

template <typename T, typename ... ArgsT>
T * placement_new(void* place, ArgsT&& ... args) {
    return new(place) T(std::forward<ArgsT>(args) ...);
}

template <typename T, typename ArgT>
T * placement_new(void* place, ArgT&& arg) {
    return placement_new_impl<T, remove_cvref_t<ArgT>>()(place, std::forward<ArgT>(arg));
}

}

#endif // SONIA_PLACEMENT_NEW_HPP
