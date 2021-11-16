// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_DEFAULT_DELETE_HPP
#define AGNOSTIC_STD_DEFAULT_DELETE_HPP

#pragma once

namespace std {

template <class T> struct default_delete
{
    constexpr default_delete() noexcept = default;

    template <class U>
    default_delete(const default_delete<U>& d) noexcept {}

    void operator()(T* ptr) const
    {
        delete ptr;
    }

    template <class U>
    void operator()(U* ptr) const = delete;
};

template <class T> struct default_delete<T[]>
{
    constexpr default_delete() noexcept = default;

    template <class U>
    default_delete(const default_delete<U[]> & d) noexcept {}

    void operator()(T* ptr) const
    {
        delete[] ptr;
    }
};

}

#endif // AGNOSTIC_STD_DEFAULT_DELETE_HPP
