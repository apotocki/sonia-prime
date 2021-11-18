//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <new>

namespace sonia {

#ifdef BOOST_WINDOWS

inline void* allocate_buffer(size_t alignment, size_t size) {
    return _aligned_malloc(size, alignment);
}

inline void free_buffer(void* buff) {
    _aligned_free(buff);
}

#elif (defined(__APPLE__))

inline void* allocate_buffer(size_t alignment, size_t size) {
    void* ptr;
    if (posix_memalign(&ptr, alignment, size)) {
        ptr = nullptr;
    }
    return ptr;
}

inline void free_buffer(void* buff) {
    free(buff);
}

#else // LINUX

inline void* allocate_buffer(size_t alignment, size_t size) {
    return aligned_alloc(alignment, size);
}

inline void free_buffer(void* buff) {
    free(buff);
}

#endif

}
