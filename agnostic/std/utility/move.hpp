// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_MOVE_HPP
#define AGNOSTIC_STD_MOVE_HPP

#pragma once

namespace std {

template <class T>
inline T&& move(T& t) noexcept
{
    return static_cast<remove_reference_t<T>&&>(t);
}

}

#endif // AGNOSTIC_STD_MOVE_HPP
