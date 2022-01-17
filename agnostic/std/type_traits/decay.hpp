// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_DECAY

#include "agnostic/std/type_traits/remove_reference.hpp"
#include "agnostic/std/type_traits/remove_extent.hpp"
#include "agnostic/std/type_traits/remove_cv.hpp"
#include "agnostic/std/type_traits/conditional.hpp"
#include "agnostic/std/type_traits/is_array.hpp"
#include "agnostic/std/type_traits/is_function.hpp"
#include "agnostic/std/type_traits/add_pointer.hpp"

namespace std {

template <typename T>
struct decay
{
private:
    using U = remove_reference_t<T>;
public:
    using type = conditional_t<
        is_array_v<U>,
        remove_extent_t<U>*,
        conditional_t<
            is_function_v<U>,
            add_pointer_t<U>,
            remove_cv_t<U>
        >
    >;
};

template <typename T> using decay_t = typename decay<T>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
