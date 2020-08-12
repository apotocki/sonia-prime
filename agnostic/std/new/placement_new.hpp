// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once
#ifndef DO_NOT_USE_AGNOSTIC_PLACEMENT_NEW

inline void* __cdecl operator new(size_t, void* location) noexcept
{
    return location;
}

#elif !defined(DO_NO_USE_STL_HEADERS)
#   include <new>
#endif
