// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

#include "agnostic/stdext/allocate_new.hpp"
#include "agnostic/std/utility/in_place.hpp"

namespace agnostic {

template <typename RefCountT>
class optimized_base
{
public:
    optimized_base() = default;
    optimized_base(optimized_base const& rhs) = delete;
    optimized_base(optimized_base&& rhs) = delete;

    virtual ~optimized_base() = default;

    optimized_base& operator= (optimized_base const&) = delete;
    optimized_base& operator= (optimized_base&&) = delete;

    inline void add_ref() noexcept
    {
        ++refs_;
    }

    inline void release() noexcept
    {
        if (--refs_ == 0) {
            dispose();
        }
    }

    bool unique() const noexcept { return 1 == refs_; }

    virtual optimized_base* clone() const = 0;
    virtual void dispose() noexcept = 0;

    //uint32_t service_cookie;

private:
    RefCountT refs_{ 1 };
};

template <>
class optimized_base<void>
{
public:
    optimized_base() = default;
    optimized_base(optimized_base const& rhs) = default;
    optimized_base(optimized_base&& rhs) = default;

    virtual ~optimized_base() = default;

    optimized_base& operator= (optimized_base const&) = default;
    optimized_base& operator= (optimized_base&&) = default;

    constexpr bool unique() const noexcept { return true; }

    virtual optimized_base* clone() const = 0;
    virtual void dispose() noexcept = 0;

    //uint32_t service_cookie;
};

template <typename WrappedT, typename RefCountT, typename AllocatorT>
class optimized_wrapper 
    : public optimized_base<RefCountT>
    , std::allocator_traits<AllocatorT>::template rebind_alloc<
        optimized_wrapper<WrappedT, RefCountT, AllocatorT>
    >
{
    using allocator_t = typename std::allocator_traits<AllocatorT>::template rebind_alloc<optimized_wrapper>;

public:
    template <typename ArgT>
    explicit optimized_wrapper(std::in_place_t, ArgT&& arg)
        : val_(std::forward<ArgT>(arg))
    {}

    inline WrappedT const& get() const noexcept { return val_; }

    template <typename T>
    void set(T&& arg)
    {
        val_ = std::forward<T>(arg);
    }

    void dispose() noexcept override
    {
        allocator_t alloc(static_cast<allocator_t&>(*this));
        std::destroy_at(this);
        std::allocator_traits<allocator_t>::destroy(alloc, this);
    }

    optimized_wrapper * clone() const override
    {
        return stdext::allocate_new<optimized_wrapper>(static_cast<allocator_t const&>(*this), *this);
    }

private:
    WrappedT val_;
};

template <size_t HolderBytesV, size_t ServiceCookieBitsV>
struct ipo_holder_base
{
    static constexpr size_t used_bits = ServiceCookieBitsV + 1;

    static_assert (HolderBytesV >= sizeof(uintptr_t));
    static_assert (sizeof(uintptr_t)* CHAR_BIT - 1 >= ServiceCookieBitsV);

    //static constexpr size_t begin_offs = 1 + ServiceCookieBitsV / CHAR_BIT;

    static constexpr size_t max_cookie_val = (((size_t)1) << ServiceCookieBitsV) - 1;

    static constexpr int first_byte_bits = ServiceCookieBitsV < (CHAR_BIT - 1) ? ServiceCookieBitsV : (CHAR_BIT - 1);
    static constexpr size_t first_byte_mask = (((size_t)1) << first_byte_bits) - 1;

    // to be able to store ponters directly in first holder bytes
    alignas(void*) char holder_[HolderBytesV];

    inline void* as_ptr() const noexcept { return *std::launder(reinterpret_cast<void* const*>(holder_)); }
    inline uintptr_t * as_uintptr() noexcept { return std::launder(reinterpret_cast<uintptr_t*>(holder_)); }
    inline uintptr_t const* as_uintptr() const noexcept { return std::launder(reinterpret_cast<uintptr_t const*>(holder_)); }
    inline char const* data() const noexcept { return std::launder(reinterpret_cast<char const*>(holder_)); }
    inline char* data() noexcept { return std::launder(reinterpret_cast<char*>(holder_)); }

    inline void init_not_ptr() noexcept { new (as_uintptr()) uintptr_t{1}; }
    inline void set_not_ptr() noexcept { *as_uintptr() |= 1; }
    inline bool is_ptr() const noexcept { return !(1 & *as_uintptr()); }
    inline void set_ptr(void* p) noexcept { assert(!(((uintptr_t)p)&1)); *reinterpret_cast<void**>(holder_) = p; }
    

    unsigned long long get_service_cookie() const noexcept
    {
        if constexpr (ServiceCookieBitsV <= first_byte_bits) {
            return first_byte_mask & ((*data()) >> 1);
        } else {
            return get_service_cookie_adv();
        }
    }

    unsigned long long get_service_cookie_adv() const noexcept
    {
        auto const* src = data();
        uint64_t res = first_byte_mask & ((*src) >> 1);
        size_t sbits = ServiceCookieBitsV - first_byte_bits;
        do {
            size_t next_byte_bits = sbits < CHAR_BIT ? sbits : CHAR_BIT;
            uint8_t next_byte_mask = (((uint8_t)1) << next_byte_bits) - 1;
            ++src;
            res <<= next_byte_bits;
            res |= next_byte_mask & *src;
            sbits -= next_byte_bits;
        } while (sbits > 0);
        return res;
    }

    void set_service_cookie(unsigned long long val) noexcept
    {
        if constexpr (ServiceCookieBitsV <= first_byte_bits) {
            auto* dest = data();
            *dest = (*dest & ~(first_byte_mask << 1)) | (unsigned char)(val << 1);
        } else {
            set_service_cookie_adv(val);
        }
    }

    void set_service_cookie_adv(size_t val) noexcept
    {
        uint8_t* dest = data();
        *dest = (uint8_t)(1 | (val >> (ServiceCookieBitsV - CHAR_BIT))); // 1 is 'not a pointer' flag
        size_t sbits = ServiceCookieBitsV - first_byte_bits;
        do {
            size_t next_byte_bits = sbits < CHAR_BIT ? sbits : CHAR_BIT;
            uint8_t next_byte_mask = (((uint8_t)1) << next_byte_bits) - 1;
            sbits -= next_byte_bits;
            ++dest;
            *dest = (*dest & ~next_byte_mask) | (uint8_t)(next_byte_mask & (val >> sbits));
        } while (sbits > 0);
    }
};

template <size_t HolderBytesV, size_t ServiceCookieBitsV>
struct ipo_holder : ipo_holder_base<HolderBytesV, ServiceCookieBitsV>
{

};

}
