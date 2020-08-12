// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_POOL_HPP
#define AGNOSTIC_POOL_HPP

#pragma once

namespace agnostic {

template <class AllocatorT>
class IN_PAGE_SEG pool
    : private AllocatorT::rebind<char>::other
{
    using allocator_t = typename AllocatorT::rebind<char>::other;

public:
    template <typename ... AllocatorArgsT>
    pool(size_t element_sz, size_t start_reserve_size, size_t next_reserve_size, AllocatorArgsT&& ... args)
        : AllocatorT(std::forward<AllocatorArgsT>(args) ...)
        , element_sz_{element_sz}
        , start_reserve_size_{start_reserve_size}
    {
        element_sz = ((element_sz + sizeof(void*) - 1) / sizeof(void*)) * sizeof(void*);
        next_reserve_size_ = next_reserve_size ? next_reserve_size : start_reserve_size_;
    }

    pool(pool const&) = delete;
    pool& operator= (pool const&) = delete;

    pool(pool && rhs)
        : allocator_t(std::move(static_cast<allocator_t&>(rhs)))
        , element_sz_{rhs.element_sz}
        , start_reserve_size_{rhs.start_reserve_size}
        , next_reserve_size_{rhs.next_reserve_size}
        , free_element_{rhs.free_element_}
        , current_chunk_{rhs.current_chunk_}
        , first_chunk_{rhs.first_chunk_}
    {
        rhs.free_element_ = rhs.current_chunk_ = rhs.first_chunk_ = nullptr;
    }

    pool& operator= (pool && rhs)
    {
        free_pool();
        allocator_t::operator=(std::move(static_cast<allocator_t&>(rhs)));
        element_sz_ = rhs.element_sz;
        start_reserve_size_ = rhs.start_reserve_size;
        next_reserve_size_ = rhs.next_reserve_size;
        free_element_ = rhs.free_element_;
        current_chunk_ = rhs.current_chunk_;
        first_chunk_ = rhs.first_chunk_;
        return *this;
    }

    ~pool()
    {
        free_pool();
    }

    void* allocate()
    {
        for (;;) {
            if (free_element_) {
                void* result = free_element_;
                free_element_ = *reinterpret_cast<void**>(free_element_);
                return result;
            }
            if (!current_chunk_) {
                first_chunk_ = current_chunk_ = prepare_chunk(start_reserve_size_);
            } else {
                void* next_chunk = prepare_chunk(max_reserve_size_);
                *reinterpret_cast<void**>(current_chunk_) = next_chunk;
                current_chunk_ = next_chunk;
            }
        }
    }

    void deallocate(void* p)
    {
        *reinterpret_cast<void**>(p) = free_element_;
        free_element_ = p;
    }

private:
    void free_pool()
    {
        if (first_chunk_) {
            void* next = *reinterpret_cast<void**>(first_chunk_);
            allocator_t::deallocate(first_chunk_, element_sz_ * start_reserve_size_ + sizeof(void*));
            while (next) {
                void* tmpnext = *reinterpret_cast<void**>(next);
                allocator_t::deallocate(next, element_sz_ * next_reserve_size_ + sizeof(void*));
                next = tmpnext;
            }
        }
    }

    void* prepare_chunk(size_t sz)
    {
        void* chunk = allocator_t::allocate(element_sz * sz + sizeof(void*));
        void** pnext_chunk = reinterpret_cast<void**>(chunk);
        *pnext_chunk = nullptr;
        ++pnext_chunk;

        size_t element_sz_in_ptr = element_sz / sizeof(void*);
        
        void** end = begin + element_sz_in_ptr * sz;
        for (; pnext_chunk != end; pnext_chunk+= element_sz_in_ptr) {
            *pnext_chunk = free_element_;
            free_element_ = pnext_chunk;
        }
        return chunk;
    }

private:
    size_t element_sz_, start_reserve_size_, max_reserve_size_;
    void * free_element_{nullptr};
    void * current_chunk_{nullptr};
    void * first_chunk_{nullptr};
};

template <
    typename T,
    class MutexT,
    class AllocatorT
>
class IN_PAGE_SEG object_pool
    : private pool<AllocatorT>
    , private MutexT
{
    object_pool(object_pool const&) = delete;
    object_pool& operator= (object_pool const&) = delete;

    using pool_t = pool<AllocatorT>;

public:
    template <typename ... AllocatorArgsT>
    explicit object_pool(size_t start_reserve_size, size_t next_reserve_size, AllocatorArgsT&& ... args)
        : pool_t(sizeof(T), start_reserve_size, next_reserve_size, std::forward<AllocatorArgsT>(args) ...)
    {

    }

    template <typename ... ArgsT>
    T* new_object(ArgsT&& ... args)
    {
        T* result;
        {
            lock_guard<MutexT> guard(*this);
            result = reinterpret_cast<T*>(do_malloc());
        }
        try {
            return new (result) T(std::forward<ArgsT>(args) ...);
        }
        catch (...) {
            lock_guard<MutexT> guard(*this);
            do_free(result);
            throw;
        }
    }

    void delete_object(T* obj) noexcept
    {
        lock_guard<MutexT> guard(*this);
        obj->~T();
        pool_t::free(obj);
    }

private:
    void* do_malloc() { return pool_t::malloc(); }
    void do_free(void* p) { pool_t::free(p); }

};

template <class T, class BaseAllocatorT>
struct pool_allocator : private pool<BaseAllocatorT>
{
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using pool_t = pool<BaseAllocatorT>;

    template <class U> struct rebind { typedef allocator<U> other; };

    template <typename ... BaseAllocatorArgsT>
    explicit pool_allocator(size_t start_reserve_size, size_t next_reserve_size, BaseAllocatorArgsT&& ... args)
        : pool_t(sizeof(T), start_reserve_size, next_reserve_size, std::forward<BaseAllocatorArgsT>(args) ...)
    {}

    pointer allocate()
    {
        return reinterpret_cast<pointer>(pool_t::allocate());
    }

    void deallocate(T* p)
    {
        pool_t::deallocate(p);
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

#endif // AGNOSTIC_POOL_HPP
