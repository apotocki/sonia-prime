// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

namespace stdext::copy_cv_detail {

template <typename F>
struct impl
{
    template <typename T> using apply = T;
};

template <typename F>
struct impl<const F>
{
    template <typename T> using apply = const T;
};

template <typename F>
struct impl<volatile F>
{
    template <typename T> using apply = volatile T;
};

template <typename F>
struct impl<const volatile F>
{
    template <typename T> using apply = const volatile T;
};

}

namespace stdext {

template <typename F, typename T>
using copy_cv = typename copy_cv_detail::impl<F>::template apply<T>;

}
