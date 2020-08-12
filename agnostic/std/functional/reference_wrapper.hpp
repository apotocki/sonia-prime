// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_REFERENCE_WRAPPER

#include "agnostic/std/utility/declval.hpp"
#include "agnostic/std/utility/forward.hpp"
#include "agnostic/std/memory/addressof.hpp"
#include "agnostic/std/type_traits/remove_cvref.hpp"
#include "agnostic/std/type_traits/is_same.hpp"
#include "agnostic/std/type_traits/enable_if.hpp"

namespace std { namespace reference_wrapper_detail {

template <class T> constexpr T& FUN(T& t) noexcept { return t; }
template <class T> void FUN(T&&) = delete;

}

template <class T>
class reference_wrapper {
public:
    using type = T;

    template <class U, class = decltype(
        reference_wrapper_detail::FUN<T>(declval<U>()), enable_if_t<!is_same_v<reference_wrapper, remove_cvref_t<U>>>()
    )>
    constexpr reference_wrapper(U&& u) noexcept(noexcept(reference_wrapper_detail::FUN<T>(forward<U>(u))))
        : _ptr(addressof(reference_wrapper_detail::FUN<T>(forward<U>(u))))
    {}

    reference_wrapper(const reference_wrapper&) noexcept = default;

    reference_wrapper& operator=(const reference_wrapper& x) noexcept = default;

    constexpr operator T& () const noexcept { return *_ptr; }
    constexpr T& get() const noexcept { return *_ptr; }

    template< class... ArgTypes >
    constexpr std::invoke_result_t<T&, ArgTypes...>
        operator() (ArgTypes&&... args) const
    {
        return std::invoke(get(), std::forward<ArgTypes>(args)...);
    }

private:
    T* _ptr;
};

// deduction guides
template<class T>
reference_wrapper(T&)->reference_wrapper<T>;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <functional>
#endif
