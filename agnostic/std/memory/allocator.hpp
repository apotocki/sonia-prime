// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

namespace std {

template <class T>
struct allocator
{
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    template <class U> struct rebind { typedef allocator<U> other; };

    pointer allocate(size_type n)
    {
        return reinterpret_cast<pointer>(new char[n * sizeof(T)]);
    }

    void deallocate(T* p, size_t)
    {
        delete[](char*)p;
    }

    template <class... Args>
    void construct(pointer p, Args&& ... args)
    {
        ::new(p) T(forward<Args>(args)...);
    }

    void destroy(pointer p)
    {
        p->T::~T();
    }
};

}
