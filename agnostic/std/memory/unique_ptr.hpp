// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_STD_UNIQUE_PTR_HPP
#define AGNOSTIC_STD_UNIQUE_PTR_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_FORWARD
#   include "../utility/forward.hpp"
#endif

#ifndef DO_NOT_USE_AGNOSTIC_MOVE
#   include "../utility/move.hpp"
#endif

namespace std {

template <typename T, typename DeleterT>
class unique_ptr : DeleterT
{
public:
    using element_type = T;
    using pointer = T*;
    using delete_type = DeleterT;

    explicit unique_ptr(T* p = nullptr) noexcept : ptr{ p } {}

    template <typename DT>
    unique_ptr(pointer p, DT&& d) noexcept : DeleterT(std::forward<DT>(d)), ptr{ p } {}

    unique_ptr(unique_ptr const&) = delete;

    unique_ptr(unique_ptr&& rhs)
        : DeleterT(std::move(static_cast<DeleterT&>(rhs)))
        , ptr{ rhs.ptr }
    {
        rhs.value = nullptr;
    }

    unique_ptr& operator=(unique_ptr&& rhs)
    {
        if (ptr) {
            DeleterT::operator()(ptr);
        }
        ptr = rhs.ptr;
        static_cast<DeleterT*>(this)->operator=(std::move(static_cast<DeleterT&>(rhs)));
        return *this;
    }

    ~unique_ptr()
    {
        if (ptr) {
            DeleterT::operator()(ptr);
        }
    }

    T* detach()
    {
        T* result = ptr;
        ptr = nullptr;
        return result;
    }

    T* operator->() { return ptr; }

private:
    T* ptr;
};

}

#endif // AGNOSTIC_STD_UNIQUE_PTR_HPP
