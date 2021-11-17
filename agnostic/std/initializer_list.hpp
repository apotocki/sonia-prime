// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_INITIALIZER_LIST

namespace std {

template <class T>
class initializer_list
{
public:
    using value_type = T;
    using reference = const T&;
    using const_reference = const T&;
    using size_type = size_t;

    using iterator = const T*;
    using const_iterator = const T*;

    constexpr initializer_list() noexcept : first_(nullptr), last_(nullptr) {}

    constexpr initializer_list(const T* first_arg, const T* last_arg) noexcept
        : first_(first_arg), last_(last_arg)
    {}

    [[nodiscard]] constexpr const T* begin() const noexcept { return first_; }

    [[nodiscard]] constexpr const T* end() const noexcept { return last_; }

    [[nodiscard]] constexpr size_t size() const noexcept { return static_cast<size_t>(last_ - first_); }

private:
    const T* first_;
    const T* last_;
};

template <class T>
[[nodiscard]] constexpr const T* begin(initializer_list<T> il) noexcept { return il.begin(); }

template <class T>
[[nodiscard]] constexpr const T* end(initializer_list<T> il) noexcept { return il.end(); }

}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <initializer_list>
#endif
