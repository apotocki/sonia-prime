// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_TO_ADDRESS_HPP
#define AGNOSTIC_STD_TO_ADDRESS_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_IS_FUNCTION
#   include "../type_traits/is_function.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_VOID
#   include "../type_traits/void.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_DECLVAL
#   include "../utility/declval.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_POINTER_TRAITS
#   include "pointer_traits.hpp"
#endif

namespace std {

namespace to_address_detail {

template <class T, class = void>
inline constexpr bool has_to_address_v = false;

template <class T>
inline constexpr bool
has_to_address_v<T, void_t<decltype(pointer_traits<T>::to_address(declval<T const&>()))>> = true;

}

template <class T>
constexpr T* to_address(T* p) noexcept
{
    static_assert(!is_function_v<T>);
    return p;
}

template <class T>
constexpr auto to_address(const T& p) noexcept
{
    if constexpr (to_address_detail::has_to_address_v<T>) {
        return pointer_traits<T>::to_address(p);
    } else {
        return to_address(p.operator->());
    }
}

}

#endif // AGNOSTIC_STD_TO_ADDRESS_HPP
