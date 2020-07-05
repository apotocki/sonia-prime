// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#include "agnostic/stdext/copy_cv.hpp"

namespace stdext {

// from N4810 [meta.trans.other]/2.1

template <class T1>
struct add_qualifiers
{
    template <class T2>
    using apply = copy_cv<T1, T2>;
};

template <class T1>
struct add_qualifiers<T1&>
{
    template <class T2>
    using apply = copy_cv<T1, T2>&;
};

template <class T1>
struct add_qualifiers<T1&&>
{
    template <class T2>
    using apply = copy_cv<T1, T2>&&;
};

}
