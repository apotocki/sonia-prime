// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_FORWARD_HPP
#define AGNOSTIC_STD_FORWARD_HPP

#pragma once

namespace std {

template <typename T>
inline T&& forward(remove_reference_t<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template <typename T>
inline T&& forward(remove_reference_t<T>&& t) noexcept
{
    return static_cast<T&&>(t);
}

}

#endif // AGNOSTIC_STD_FORWARD_HPP
