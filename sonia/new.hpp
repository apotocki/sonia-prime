//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <new>

#ifdef BOOST_WINDOWS
namespace sonia {

inline void* allocate_buffer(size_t alignment, size_t size) {
    return _aligned_malloc(size, alignment);
}

inline void free_buffer(void* buff) {
    _aligned_free(buff);
}
}

#elif (defined(__APPLE__))

namespace sonia {

inline void* allocate_buffer(size_t alignment, size_t size) {
    void* ptr;
    alignment = (std::max)(alignment, sizeof(void *));
    if (posix_memalign(&ptr, alignment, size)) {
        ptr = nullptr;
    }
    return ptr;
}

inline void free_buffer(void* buff) {
    free(buff);
}

}

#elif (defined(__ANDROID__) && !defined(_LIBCPP_HAS_ALIGNED_ALLOC))
namespace sonia {

inline void* allocate_buffer(size_t alignment, size_t size)
{
    void* p1 = malloc(size + alignment + sizeof(void*));
    if (!p1) return p1;

    uintptr_t addr = (uintptr_t)p1 + alignment + sizeof(void*);

    void* p2 = (void*)(addr - (addr % alignment));

    *((void**)p2 - 1) = p1;

    return p2;
}

inline void free_buffer(void* buff) {
    free((void*)(*((void**)buff - 1)));
}

}

#else // LINUX

#include <cstdlib>

namespace sonia {

inline void* allocate_buffer(size_t alignment, size_t size) {
    return std::aligned_alloc(alignment, size);
}

inline void free_buffer(void* buff) {
    free(buff);
}

}

#endif
