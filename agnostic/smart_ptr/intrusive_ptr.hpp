// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_INTRUSIVE_PTR_HPP
#define AGNOSTIC_INTRUSIVE_PTR_HPP

#pragma once

//  Relies on unqualified calls to
//  
//      void intrusive_ptr_add_ref(T * p);
//      void intrusive_ptr_release(T * p);

namespace agnostic {

template <class T>
class intrusive_ptr
{
    template <class> friend class intrusive_ptr;

public:
    using element_type = T;

    constexpr intrusive_ptr() noexcept : ptr_{nullptr} {}

    explicit intrusive_ptr(T* p, bool add_ref = true)
        : ptr_{p}
    {
        if (ptr_ && add_ref) intrusive_ptr_add_ref(ptr_);
    }

    template <class U>
    intrusive_ptr(intrusive_ptr<U> const& rhs)
        : ptr_{rhs.ptr_}
    {
        if (ptr_) intrusive_ptr_add_ref(ptr_);
    }

    template <class U>
    intrusive_ptr(intrusive_ptr<U> && rhs) noexcept
        : ptr_{ rhs.ptr_ }
    {
        rhs.ptr_ = nullptr;
    }

    intrusive_ptr(intrusive_ptr const& rhs)
        : ptr_(rhs.ptr_)
    {
        if (ptr_) intrusive_ptr_add_ref(ptr_);
    }

    intrusive_ptr(intrusive_ptr && rhs) noexcept
        : ptr_(rhs.ptr_)
    {
        rhs.ptr_ = nullptr;
    }

    ~intrusive_ptr()
    {
        if (ptr_) intrusive_ptr_release(ptr_);
    }

    template <class U>
    intrusive_ptr & operator=(intrusive_ptr<U> const& rhs)
    {
        if (ptr_) intrusive_ptr_release(ptr_);
        ptr_ = rhs.ptr_;
        if (ptr_) intrusive_ptr_add_ref(ptr_);
        return *this;
    }

    template <class U>
    intrusive_ptr& operator=(intrusive_ptr<U> && rhs) noexcept
    {
        if (ptr_) intrusive_ptr_release(ptr_);
        ptr_ = rhs.ptr_;
        return *this;
    }

    intrusive_ptr& operator=(intrusive_ptr const& rhs)
    {
        if (ptr_) intrusive_ptr_release(ptr_);
        ptr_ = rhs.ptr_;
        if (ptr_) intrusive_ptr_add_ref(ptr_);
        return *this;
    }

    intrusive_ptr& operator=(intrusive_ptr && rhs) noexcept
    {
        if (ptr_) intrusive_ptr_release(ptr_);
        ptr_ = rhs.ptr_;
        return *this;
    }

    void reset()
    {
        if (ptr_) intrusive_ptr_release(ptr_);
        ptr_ = nullptr;
    }

    void reset(T* rhs)
    {
        if (ptr_) intrusive_ptr_release(ptr_);
        ptr_ = rhs;
        if (ptr_) intrusive_ptr_add_ref(ptr_);
    }

    void reset(T* rhs, bool add_ref)
    {
        if (ptr_) intrusive_ptr_release(ptr_);
        ptr_ = rhs;
        if (ptr_ && add_ref) intrusive_ptr_add_ref(ptr_);
    }

    T* get() const noexcept { return ptr_; }
    T* operator-> () const noexcept { return ptr_; }
    T& operator* () const noexcept { return *ptr_; }

    T* detach() noexcept
    {
        T * result = ptr_;
        ptr_ = nullptr;
        return result;
    }

    explicit operator bool() const { return !!ptr_; }

    void swap(intrusive_ptr & rhs) noexcept
    {
        T * tmp = ptr_;
        ptr_ = rhs.ptr_;
        rhs.ptr_ = tmp;
    }

private:
    T* ptr_;
};

template <class T, class U>
inline bool operator==(intrusive_ptr<T> const& l, intrusive_ptr<U> const& r) noexcept
{
    return l.get() == r.get();
}

template <class T, class U>
inline bool operator!=(intrusive_ptr<T> const& l, intrusive_ptr<U> const& r) noexcept
{
    return l.get() == r.get();
}

template <class T, class U>
inline bool operator==(intrusive_ptr<T> const& l, U* r) noexcept
{
    return l.get() == r;
}

template <class T, class U>
inline bool operator!=(intrusive_ptr<T> const& l, U* r) noexcept
{
    return l.get() != r;
}

template <class T, class U>
inline bool operator==(T* l, intrusive_ptr<U> const& r) noexcept
{
    return l == r.get();
}

template <class T, class U>
inline bool operator!=(T* l, intrusive_ptr<U> const& r) noexcept
{
    return l != r.get();
}

template <class T>
inline bool operator<(intrusive_ptr<T> const& a, intrusive_ptr<T> const& b) noexcept
{
    return std::less<T*>()(a.get(), b.get());
}

}

#endif // AGNOSTIC_INTRUSIVE_PTR_HPP
