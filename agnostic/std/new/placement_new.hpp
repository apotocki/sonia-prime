// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_PLACEMENT_NEW_HPP
#define AGNOSTIC_STD_PLACEMENT_NEW_HPP

#pragma once

inline void* __cdecl operator new(size_t, void* location) noexcept
{
    return location;
}

#endif // AGNOSTIC_STD_PLACEMENT_NEW_HPP
