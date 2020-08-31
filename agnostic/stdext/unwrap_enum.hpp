// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#include "agnostic/std/type_traits/is_enum.hpp"
#include "agnostic/std/type_traits/underlying_type.hpp"

namespace stdext {

using namespace std;

template <typename T, bool = is_enum_v<T>>
struct unwrap_enum
{
    using type = underlying_type_t<T>;
};

template <typename T>
struct unwrap_enum<T, false>
{
    using type = T;
};

template <class T>
using unwrap_enum_t = typename unwrap_enum<T>::type;

}
