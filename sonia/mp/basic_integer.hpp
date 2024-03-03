/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <memory>
#include <concepts>
#include <algorithm>

#include "integer_view.hpp"
#include <iosfwd>

namespace sonia::mp {

template <std::unsigned_integral LimbT, size_t N, typename AllocatorT = std::allocator<LimbT>>
class basic_integer 
    : private std::allocator_traits<AllocatorT>::template rebind_alloc<LimbT>
{
    using view_type = basic_integer_view<LimbT>;

    using allocator_type = std::allocator_traits<AllocatorT>::template rebind_alloc<LimbT>;
    using alloc_traits_t = std::allocator_traits<allocator_type>;
    
    inline allocator_type & allocator() noexcept { return static_cast<allocator_type&>(*this); }
    inline allocator_type const& allocator() const noexcept { return static_cast<allocator_type const&>(*this); }

    // small size optimized allocator
    struct sso_allocator
    {
        using value_type = LimbT;

        LimbT* buffer_;
        allocator_type& allocator_;
        size_t used_size_ = 0;

        sso_allocator(LimbT* buff, allocator_type& a) : buffer_{ buff }, allocator_{ a } {}

        LimbT* allocate(size_t cnt)
        {
            if (!used_size_ && cnt <= N) {
                used_size_ += cnt;
                return buffer_;
            } else {
                return alloc_traits_t::allocate(allocator_, cnt);
            }
        }

        void deallocate(LimbT* ptr, size_t sz)
        {
            if (std::less<LimbT*>{}(ptr, buffer_) || std::less<LimbT*>{}(buffer_ + N, ptr)) {
                alloc_traits_t::deallocate(allocator_, ptr, sz);
            } else {
                used_size_ = 0;
            }
        }
    };

    inline void init(std::tuple<LimbT*, size_t, size_t, int> tpl)
    {
        size_ = static_cast<uint32_t>(std::get<1>(tpl));
        sign_ = std::get<3>(tpl) < 0;
        if (std::get<0>(tpl) == inplace_limbs_) {
            inplace_ = 1;
        }
        else {
            limbs_ = std::get<0>(tpl);
            inplace_ = 0;
            reserved_size_ = static_cast<uint32_t>(std::get<2>(tpl));
        }
    }

public:
    basic_integer() : size_{0}, inplace_{1} {}
        
    basic_integer(AllocatorT const& alloc) : allocator_type{ alloc }, size_{ 0 }, reserved_size_{ 0 } {}

    basic_integer(std::span<LimbT> limbs, int32_t sgnsz, AllocatorT const& alloc)
        : allocator_type{alloc}
        , limbs_{ limbs.data() }
        , reserved_size_{ static_cast<uint32_t>(limbs.size()) }
        , size_ { static_cast<uint32_t>(std::abs(sgnsz)) }
        , sign_ { sgnsz < 0 }
        , inplace_{ 0 }
    {}

    template <std::integral T>
    explicit basic_integer(T value, AllocatorT const& alloc = AllocatorT{})
        : allocator_type{ alloc }
    {
        sso_allocator a{ inplace_limbs_, allocator() };
        init(to_limbs<LimbT>(value, a));
    }

    explicit basic_integer(std::string_view str, int base = 0, AllocatorT const& alloc = AllocatorT{})
        : allocator_type{ alloc }
    {
        sso_allocator a{ inplace_limbs_, allocator() };
        init(to_limbs<LimbT>(str, base, a));
    }

    basic_integer(basic_integer const& rhs)
        : allocator_type{ rhs.allocator() }
        , size_{ rhs.size_ }
        , sign_{ rhs.sign_ }
    {
        if (size_ <= N) {
            std::memcpy(inplace_limbs_, rhs.inplace_ ? rhs.inplace_limbs_ : rhs.limbs_, size_ * sizeof(LimbT));
            inplace_ = 1;
        } else {
            assert(!rhs.inplace_);
            limbs_ = alloc_traits_t::allocate(allocator(), size_);
            std::memcpy(limbs_, rhs.limbs_, size_ * sizeof(LimbT));
            reserved_size_ = size_;
            inplace_ = 0;
        }
    }

    basic_integer(basic_integer && rhs)
        : allocator_type{ rhs.allocator() }
        , size_{ rhs.size_ }
        , sign_{ rhs.sign_ }
        , inplace_{ rhs.inplace_ }
    {
        if (inplace_) {
            std::memcpy(inplace_limbs_, rhs.inplace_limbs_, size_ * sizeof(LimbT));
        } else {
            limbs_ = rhs.limbs_;
            reserved_size_ = rhs.reserved_size_;
            rhs.reserved_size_ = 0;
        }
    }

    ~basic_integer()
    {
        if (!inplace_ && reserved_size_) {
            alloc_traits_t::deallocate(allocator(), limbs_, (size_t)reserved_size_);
        }
    }

    basic_integer& operator= (basic_integer const& rhs)
    {
        basic_integer temp{ rhs };
        swap(temp);
        return *this;
    }

    basic_integer& operator= (basic_integer && rhs)
    {
        basic_integer temp{ std::move(rhs) };
        swap(temp);
        return *this;
    }

    template <std::integral T>
    basic_integer& operator= (T rhs)
    {
        basic_integer temp{ rhs };
        swap(temp);
        return *this;
    }

    void swap(basic_integer& other)
    {
        if (inplace_) {
            if (other.inplace_) {
                std::swap_ranges(inplace_limbs_, inplace_limbs_ + N, other.inplace_limbs_);
            } else {
                other.do_swap_from_inplace(*this);
            }
        } else if (other.inplace_) {
            do_swap_from_inplace(other);
        } else {
            std::swap(limbs_, other.limbs_);
            std::swap(reserved_size_, other.reserved_size_);
        }
        uint32_t tmp = size_; size_ = other.size_; other.size_ = tmp;
        tmp = sign_; sign_ = other.sign_; other.sign_ = tmp;
        std::swap(static_cast<allocator_type&>(*this), static_cast<allocator_type&>(other));
    }

    operator view_type() const
    {
        if (inplace_) {
            return view_type{ std::span{inplace_limbs_, (size_t)size_}, sign_ ? -1 : 1 };
        } else {
            return view_type{ std::span{limbs_, (size_t)size_}, sign_ ? -1 : 1 };
        }
    }

    template <size_t LN, size_t RN, typename AllocatorLT, typename AllocatorRT>
    friend bool operator ==(basic_integer<LimbT, LN, AllocatorLT> const& lhs, basic_integer<LimbT, RN, AllocatorRT> const& rhs)
    {
        return (view_type)lhs == (view_type)rhs;
    }

    template <size_t LN, typename AllocatorLT, std::integral RT>
    friend bool operator == (basic_integer<LimbT, LN, AllocatorLT> const& lhs, RT rhs)
    {
        return (view_type)lhs == rhs;
    }

    template <size_t LN, size_t RN, typename AllocatorLT, typename AllocatorRT>
    friend std::strong_ordering operator <=> (basic_integer<LimbT, LN, AllocatorLT> const& lhs, basic_integer<LimbT, RN, AllocatorRT> const& rhs)
    {
        return (view_type)lhs <=> (view_type)rhs;
    }

    template <size_t LN, typename AllocatorLT, std::integral RT>
    friend std::strong_ordering operator <=> (basic_integer<LimbT, LN, AllocatorLT> const& lhs, RT rhs)
    {
        return (view_type)lhs <=> rhs;
    }

    explicit operator bool() const
    {
        if (!size_) return false;
        if (inplace_) return !!inplace_limbs_[size_ - 1];
        return !!limbs_[size_ - 1];
    }

    template <std::integral T>
    explicit operator T() const
    {
        return (T)(view_type)*this;
    }

    template <std::floating_point T>
    explicit operator T() const
    {
        return (T)(view_type)*this;
    }

    inline int32_t sign() const noexcept { return sign_ ? -1 : 1; }
    //inline intptr_t limbs_size() const noexcept { return size_; };

    basic_integer operator- () const
    {
        basic_integer result{ *this };
        result.sign_ = ~sign_;
        return result;
    }

    friend inline basic_integer operator+ (basic_integer const& l, basic_integer_view<LimbT> r)
    {
        basic_integer result{ l.allocator() };
        sso_allocator alloc{ result.inplace_limbs_, result.allocator() };
        result.init(add((basic_integer_view<LimbT>)l, r, alloc));
        return result;
    }

    friend inline basic_integer operator- (basic_integer const& l, basic_integer_view<LimbT> r)
    {
        basic_integer result{ l.allocator() };
        sso_allocator alloc{ result.inplace_limbs_, result.allocator() };
        result.init(sub((basic_integer_view<LimbT>)l, r, alloc));
        return result;
    }

    friend inline basic_integer operator* (basic_integer const& l, basic_integer_view<LimbT> r)
    {
        basic_integer result{ l.allocator() };
        sso_allocator alloc{ result.inplace_limbs_, result.allocator() };
        result.init(mul((basic_integer_view<LimbT>)l, r, alloc));
        return result;
    }

    friend inline basic_integer operator/ (basic_integer const& l, basic_integer_view<LimbT> r)
    {
        basic_integer result{ l.allocator() };
        sso_allocator alloc{ result.inplace_limbs_, result.allocator() };
        result.init(div((basic_integer_view<LimbT>)l, r, alloc));
        return result;
    }

    friend inline basic_integer operator% (basic_integer const& l, basic_integer_view<LimbT> r)
    {
        basic_integer result{ l.allocator() };
        sso_allocator alloc{ result.inplace_limbs_, result.allocator() };
        result.init(mod((basic_integer_view<LimbT>)l, r, alloc));
        return result;
    }

    friend inline basic_integer pow(basic_integer const& arg, unsigned int n)
    {
        basic_integer result{ arg.allocator() };
        sso_allocator alloc{ result.inplace_limbs_, result.allocator() };
        result.init(pow((basic_integer_view<LimbT>)arg, n, alloc));
        return result;
    }

    template <std::integral T>
    friend inline basic_integer operator+ (basic_integer const& l, T r)
    {
        return l + basic_integer{ r };
    }

    template <std::integral T>
    friend inline basic_integer operator- (basic_integer const& l, T r)
    {
        return l - basic_integer{ r };
    }

    template <std::integral T>
    friend inline basic_integer operator* (basic_integer const& l, T r)
    {
        return l * basic_integer{ r };
    }

    template <std::integral T>
    friend inline basic_integer operator/ (basic_integer const& l, T r)
    {
        return l / basic_integer{ r };
    }

    template <std::integral T>
    friend inline basic_integer operator% (basic_integer const& l, T r)
    {
        return l % basic_integer{ r };
    }

private:
    void do_swap_from_inplace(basic_integer& other) noexcept
    {
        LimbT* limbs = limbs_;
        size_t rsz = reserved_size_;
        std::copy(other.inplace_limbs_, other.inplace_limbs_ + other.size_, inplace_limbs_);
        other.limbs_ = limbs;
        other.reserved_size_ = static_cast<uint32_t>(rsz);
        other.inplace_ = 0;
        inplace_ = 1;
    }

    union {
        struct {
            LimbT* limbs_;
            uint32_t reserved_size_;
        };
        LimbT inplace_limbs_[N];
    };

    uint32_t size_ : 30;
    uint32_t sign_ : 1;
    uint32_t inplace_ : 1;
};

template <typename Elem, typename Traits, std::unsigned_integral LimbT, size_t N, typename AllocatorT>
std::basic_ostream<Elem, Traits>& operator <<(std::basic_ostream<Elem, Traits>& os, basic_integer<LimbT, N, AllocatorT> iv)
{
    return os << (basic_integer_view<LimbT>)iv;
}

}
