/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <span>
#include <algorithm>
#include <type_traits>
#include <utility>

namespace sonia::container {

template <typename T, size_t InplaceSz, typename AllocatorT>
class small_array : AllocatorT
{
    union {
        mutable T small_data_[InplaceSz];
        T* pdata_;
    };
    size_t size_;

    using allocator_type = std::remove_cvref_t<AllocatorT>;
    using alloc_traits_t = std::allocator_traits<AllocatorT>;

public:
    template <typename AllocatorArgT>
    small_array(size_t sz, AllocatorArgT&& arg)
        : AllocatorT(std::forward<AllocatorArgT>(arg))
        , size_{ sz }
    {
        if (size_ > InplaceSz) {
            pdata_ = alloc_traits_t::allocate(static_cast<AllocatorT&>(*this), size_);
        }

        if constexpr (!std::is_trivially_constructible_v<T>) {
            for (T& obj : *this) {
                new (&obj) T;
            }
        }
    }

    explicit small_array(size_t sz)
        : small_array(sz, AllocatorT{})
    {}

    small_array(small_array const& rhs)
        : AllocatorT(static_cast<AllocatorT const&>(rhs)), size_{ rhs.size_ }
    {
        T* pdata;
        if (size_ > InplaceSz) {
            pdata = pdata_ = alloc_traits_t::allocate(static_cast<AllocatorT&>(*this), size_);
        } else {
            pdata = small_data_;
        }
        do_copy_init(rhs.begin(), rhs.end(), pdata);
    }

    template <typename ElementT = T>
    requires (!InplaceSz || std::is_nothrow_move_constructible_v<ElementT>)
    small_array(small_array && rhs) noexcept
        : AllocatorT(std::move(static_cast<AllocatorT&>(rhs))), size_{ rhs.size_ }
    {
        if (size_ > InplaceSz) {
            pdata_ = rhs.pdata_;
            rhs.size_ = 0;
        } else {
            do_move_init(rhs.small_data_, rhs.small_data_ + size_, small_data_);
        }
    }

    template <typename ElementT = T>
    requires (!InplaceSz || (std::is_nothrow_move_constructible_v<ElementT> && std::is_nothrow_swappable_v<ElementT>))
    inline void swap(small_array & rhs) noexcept
    {
        if constexpr (!InplaceSz) {
            std::swap(pdata_, rhs.pdata_);
            std::swap(size_, rhs.size_);
        } else {
            if (size_ > InplaceSz) {
                if (rhs.size_ > InplaceSz) {
                    std::swap(pdata_, rhs.pdata_);
                    std::swap(size_, rhs.size_);
                } else {
                    rhs.do_swap_allocated(*this);
                }
            } else if (rhs.size_ > InplaceSz) {
                do_swap_allocated(*this);
            } else {
                size_t swap_sz = (std::min)(size_, rhs.size_);
                T * bl = small_data_, * br = rhs.small_data_;
                for (T* el = small_data_ + swap_sz; bl != el; ++bl, ++br) {
                    using std::swap;
                    swap(*bl, br);
                }
                if (size_ > swap_sz) {
                    do_destructible_move_init(bl, small_data_ + size_, br);
                    std::swap(size_, rhs.size_);
                } else if (size_ < swap_sz) {
                    do_destructible_move_init(br, rhs.small_data_ + rhs.size_, bl);
                    std::swap(size_, rhs.size_);
                } // else lhs and rhs have the sam size => no moving
            }
        }
        using std::swap;
        swap(static_cast<AllocatorT&>(*this), static_cast<AllocatorT&>(rhs));
    }

    template <typename ElementT = T>
    requires(!InplaceSz || (std::is_nothrow_move_constructible_v<ElementT> && std::is_nothrow_swappable_v<ElementT>))
    inline friend void swap(small_array const& lhs, small_array const& rhs)
    {
        lhs.swap(rhs);
    }

    template <typename ElementT = T>
    requires(!InplaceSz || (std::is_nothrow_move_constructible_v<ElementT> && std::is_nothrow_swappable_v<ElementT>))
    small_array& operator=(small_array const& rhs)
    {
        small_array tmp(rhs);
        swap(tmp);
        return *this;
    }

    template <typename ElementT = T>
    requires(!InplaceSz || (std::is_nothrow_move_constructible_v<ElementT> && std::is_nothrow_swappable_v<ElementT>))
    small_array& operator=(small_array && rhs)
    {
        small_array tmp(std::move(rhs));
        swap(tmp);
        return *this;
    }

    ~small_array()
    {
        do_free();
    }

    inline T* data() const noexcept
    {
        if constexpr (!InplaceSz) { return pdata_; }
        else { return size_ > InplaceSz ? pdata_ : small_data_; }
    }

    inline size_t size() const noexcept { return size_; }

    inline T& front() const noexcept { return *data(); }
    inline T& back() const noexcept { assert(size_); return *(data() + size_ - 1); }

    inline T* begin() const noexcept { return data(); }
    inline T* end() const noexcept { return data() + size_; }

    inline operator std::span<T>() const noexcept { return std::span{ data(), size() }; }
    inline std::span<T> span() const noexcept { return std::span{ data(), size() }; }

    inline std::span<T> first(size_t sz) const noexcept { return { data(), sz }; }
    inline std::span<T> last(size_t sz) const noexcept { return { data() + size_ - sz, sz }; }
    inline std::span<T> subspan(size_t offset) const noexcept { return { data() + offset, size() - offset }; }
    inline std::span<T> subspan(size_t offset, size_t count) const noexcept { return { data() + offset, count }; }

    void reset(size_t newsz)
    {
        do_free();
        
        size_ = newsz;

        if (newsz > InplaceSz) {
            try {
                pdata_ = alloc_traits_t::allocate(static_cast<AllocatorT&>(*this), size_);
            } catch (...) {
                size_ = 0;
                throw;
            }
        }
        
        if constexpr (!std::is_trivially_constructible_v<T>) {
            for (T& obj : *this) {
                new (&obj) T;
            }
        }
    }

private:
    inline void do_copy_init(T* srcb, T* srce, T* dst) noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        if constexpr (std::is_trivial_v<T>) {
            std::copy(srcb, srce, dst);
        } else if constexpr (std::is_nothrow_copy_constructible_v<T>) {
            while (srcb != srce) new(dst++) T(*srcb++);
        } else {
            for (T * dstb = dst; srcb != srce; ++srcb, ++dst) {
                try {
                    new(dst) T(*srcb);
                } catch (...) {
                    if (dst != dstb) {
                        do {
                            --dst;
                            dst->~T();
                        } while (dst != dstb);
                    }
                    throw;
                }
            }
        }
    }

    template <typename ElementT>
    requires(std::is_nothrow_move_constructible_v<ElementT>)
    inline void do_move_init(ElementT* srcb, ElementT* srce, ElementT* dst) noexcept
    {
        if constexpr (std::is_trivial_v<T>) {
            std::copy(srcb, srce, dst);
        } else {
            while (srcb != srce) new(dst++) ElementT(std::move(*srcb++));
        }
    }

    template <typename ElementT>
    requires(std::is_nothrow_move_constructible_v<ElementT>)
    inline void do_destructible_move_init(ElementT* srcb, ElementT* srce, ElementT* dst) noexcept
    {
        if constexpr (std::is_trivial_v<T>) {
            std::copy(srcb, srce, dst);
        } else {
            while (srcb != srce) {
                new(dst++) ElementT(std::move(*srcb));
                srcb->~ElementT();
                ++srcb;
            }
        }
    }

    template <typename ElementT = T>
    requires(std::is_nothrow_move_constructible_v<ElementT>)
    inline void do_swap_allocated(small_array& rhs) noexcept
    {
        T* rhs_data = rhs.pdata_;
        do_move_init(small_data_, small_data_ + size_, rhs.small_data_);
        std::swap(size_, rhs.size_);
        pdata_ = rhs_data;
    }

    inline void do_free() noexcept
    {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            for (T& obj : *this) {
                obj.~T();
            }
        }
        if (size_ > InplaceSz) {
            alloc_traits_t::deallocate(static_cast<AllocatorT&>(*this), pdata_, size_);
        }
    }
};

}