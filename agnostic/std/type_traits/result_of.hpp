// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_RESULT_OF

#include "agnostic/std/utility/declval.hpp"

namespace std { namespace result_of_detail {

}

template <class> struct result_of;

template <class F, class... ArgTypes>
struct result_of<F(ArgTypes...)>
{
    using type = decltype(detail::INVOKE(declval<F>(), declval<ArgTypes>()...));
};

template< class T >
using result_of_t = typename result_of<T>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
