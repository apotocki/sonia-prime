// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_VOID

namespace std {

template <typename... Ts> using void_t = void;

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <type_traits>
#endif
