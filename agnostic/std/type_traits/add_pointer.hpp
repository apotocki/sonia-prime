// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_IS_FUNCTION
#   include "is_function.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_REMOVE_REFERENCE
#   include "remove_reference.hpp"
#endif

namespace std {

namespace detail
{
    template <typename T, bool IsFunctionV = false>
    struct add_pointer { using type = remove_reference_t<T>*; };

    template <typename T> struct add_pointer<T, true> { using type = T; };
    template <typename T, typename... Args> struct add_pointer<T(Args...), true> { using type = T(*)(Args...); };
    template <typename T, typename... Args> struct add_pointer<T(Args..., ...), true> { using type = T(*)(Args..., ...); };
}

template <typename T> struct add_pointer : detail::add_pointer<T, is_function_v<T>> {};
template <typename T> using add_pointer_t = typename add_pointer<T>::type;

}
