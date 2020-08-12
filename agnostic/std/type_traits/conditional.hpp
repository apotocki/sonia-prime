// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_CONDITIONAL

namespace std {

template <bool CV, typename T, typename F>
struct conditional { using type = T; };

template <typename T, typename F>
struct conditional<false, T, F> { using type = F; };

template <bool CV, typename T, typename F>
using conditional_t = typename conditional<CV, T, F>::type;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
