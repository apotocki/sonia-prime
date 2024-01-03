/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "arithmetic.hpp"
#include "limb_arithmetic.hpp"
#include "limbs_from_string.hpp"
#include "ct.hpp"
#include <vector>

namespace sonia::mp {

template <std::unsigned_integral LimbT, std::integral T, size_t N>
requires(N >= (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT))
std::tuple<size_t, int> to_limbs(T value, std::span<LimbT, N> sp)
{
    T absval;
    int sign;
    if constexpr (std::is_signed_v<T>) {
        absval = std::abs(value);
        sign = value < 0 ? -1 : 1;
    } else {
        absval = value;
        sign = 1;
    }

    constexpr size_t limbs_cnt = (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT);

    sp.front() = static_cast<LimbT>(absval);
    size_t cnt = 1;
    if constexpr (limbs_cnt > 1) {
        absval >>= sizeof(LimbT) * CHAR_BIT;
        for (; !!absval && cnt < limbs_cnt; ++cnt) {
            sp[cnt] = static_cast<LimbT>(absval);
            absval >>= sizeof(LimbT) * CHAR_BIT;
        }
    }
    return { cnt, sign };
}

template <std::unsigned_integral LimbT>
class basic_integer_view
{
    template <std::unsigned_integral> friend class basic_integer_view;

    inline basic_integer_view(LimbT * limbs, intptr_t size) noexcept
        : limbs_{ limbs }, size_{ size }
    {}

public:
    explicit basic_integer_view(std::span<LimbT> limbs, int sign = 1) noexcept
        : limbs_{ limbs.data() }, size_ { static_cast<intptr_t>(limbs.size()) }
    {
        if (sign < 0) size_ = -size_;
    }

    template <LimbT ... ls>
    basic_integer_view(mpct::limbs<LimbT, ls...>, int sign = 1) noexcept
        : limbs_{ mpct::limbs<LimbT, ls...>::data }, size_{ sizeof...(ls) }
    {
        if (sign < 0) size_ = -size_;
    }

    template <typename LT>
    requires(std::is_const_v<LimbT> && std::is_same_v<std::remove_cv_t<LimbT>, LT>)
    basic_integer_view(basic_integer_view<LT> rhs) noexcept
        : limbs_{ rhs.limbs_ }, size_{ rhs.size_ }
    {}

    inline int sign() const noexcept { return size_ < 0 ? -1 : 1; }
    inline operator std::span<LimbT>() noexcept { return { limbs_, static_cast<size_t>(size_ < 0 ? -size_ : size_) }; }
    inline operator std::span<std::add_const_t<LimbT>>() const noexcept { return { limbs_, static_cast<size_t>(size_ < 0 ? -size_ : size_) }; }
    
    inline std::span<std::add_const_t<LimbT>> limbs() const noexcept
    {
        return { limbs_, static_cast<size_t>(size_ < 0 ? -size_ : size_) };
    }

    inline std::span<LimbT> limbs() noexcept
    {
        return { limbs_, static_cast<size_t>(size_ < 0 ? -size_ : size_) };
    }

    inline LimbT at(size_t index) const noexcept
    {
        return size_t(std::abs(size_)) > index ? limbs_[index] : LimbT{0};
    }

    template <std::integral T>
    explicit operator T() const
    {
        if (!size_) [[unlikely]] return 0;
        using u_t = std::make_unsigned_t<std::remove_cv_t<T>>;
        u_t result = static_cast<u_t>(*limbs_);
        if constexpr (sizeof(T) > sizeof(LimbT)) {
            constexpr intptr_t max_limbs_to_process = (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT);
            intptr_t limbs_to_process = (std::min)(std::abs(size_), max_limbs_to_process);
            for (intptr_t i = 1; i < limbs_to_process; ++i) {
                result |= static_cast<u_t>(limbs_[i]) << (i * sizeof(LimbT) * CHAR_BIT);
            }
        }

        if (sign() < 0) {
           result = ~result + 1;
        }
        return static_cast<T>(result);
    }

    inline basic_integer_view operator- () const noexcept
    {
        return basic_integer_view { limbs_, -size_ };
    }

    friend bool operator ==(basic_integer_view lhs, basic_integer_view rhs)
    {
        if ((lhs.size_ < 0 && rhs.size_ > 0) || (lhs.size_ > 0 && rhs.size_ < 0)) return false;
        LimbT* lb = lhs.limbs_, * le = lb + std::abs(lhs.size_);
        LimbT* rb = rhs.limbs_, * re = rb + std::abs(rhs.size_);
        for (;; ++rb, ++lb) {
            if (lb != le) {
                if (rb != re) {
                    if (*lb != *rb) return false;
                    continue;
                } else {
                    do {
                        if (*lb != 0) return false;
                        ++lb;
                    } while (lb != le);
                }
            } else {
                for (; rb != re; ++rb) {
                    if (*rb != 0) return false;
                }
            }
            return true;
        }
    }

    template <std::integral T>
    friend bool operator ==(basic_integer_view lhs, T rhs)
    {
        if constexpr (std::is_signed_v<T>) {
            if ((lhs.size_ < 0 && rhs > 0) || (lhs.size_ > 0 && rhs < 0)) return false;
        } else {
            if (rhs && lhs.size_ < 0) return false;
        }

        T rabsval;
        if constexpr (std::is_signed_v<T>) {
            rabsval = std::abs(rhs);
        } else {
            rabsval = rhs;
        }

        if (lhs.at(0) != static_cast<LimbT>(rabsval)) return false;
        constexpr auto limbs_to_compare = (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT);
        intptr_t lsz = std::abs(lhs.size_);
        if constexpr (limbs_to_compare > 1) {
            for (int i = 1; i < limbs_to_compare; ++i) {
                LimbT l = static_cast<LimbT>(rabsval >> (i * sizeof(LimbT) * CHAR_BIT));
                if (l != (lsz > i ? lhs.limbs_[i] : LimbT{ 0 })) return false;
            }
        }
        for (intptr_t i = lsz - 1; i >= limbs_to_compare; --i) {
            if (lhs.limbs_[i]) return false;
        }
        return true;
    }

    friend std::strong_ordering operator <=> (basic_integer_view lhs, basic_integer_view rhs)
    {
        if (lhs.size_ < 0 && rhs.size_ > 0) return std::strong_ordering::less;
        if (lhs.size_ > 0 && rhs.size_ < 0) return std::strong_ordering::greater;
        intptr_t lsz = std::abs(lhs.size_);
        intptr_t rsz = std::abs(rhs.size_);
        
        LimbT* re = rhs.limbs_ + rsz;
        LimbT* le = lhs.limbs_ + lsz;
        if (lsz < rsz) {
            do {
                --re;
                if (*re) return rhs.size_ > 0 ? std::strong_ordering::less : std::strong_ordering::greater;
                --rsz;
            } while (lsz < rsz);
        } else if (rsz < lsz) {
            do {
                --le;
                if (*le) return lhs.size_ > 0 ? std::strong_ordering::greater : std::strong_ordering::less;
                --lsz;
            } while (rsz < lsz);
        }
        for (; lhs.limbs_ != le;) {
            --le; --re;
            auto r = *le <=> *re;
            if (r != std::strong_ordering::equal) {
                return (lhs.size_ > 0 ? r : (0 <=> r));
            }
        }
        return std::strong_ordering::equal;
    }

    template <std::integral T>
    friend std::strong_ordering operator <=> (basic_integer_view lhs, T rhs)
    {
        /*
        if constexpr (std::is_signed_v<T>) {
            if (lhs.size_ < 0 && rhs > 0) return std::strong_ordering::less;
            if (lhs.size_ > 0 && rhs < 0) return std::strong_ordering::greater;
        } else {
            if (lhs.size_ < 0) return std::strong_ordering::less;
        }
        */
        std::remove_cv_t<LimbT> buf[(sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT)];
        auto [sz, sign] = to_limbs(rhs, std::span{buf});
        return lhs <=> basic_integer_view{ std::span{buf, sz}, sign };
    }

private:
    LimbT* limbs_;
    intptr_t size_;
};

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] auto add(basic_integer_view<LimbT> l, basic_integer_view<LimbT> r, AllocatorT&& alloc)
{
    using limb_t = std::remove_cv_t<LimbT>;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    std::tuple<limb_t*, size_t, size_t, int> result;
    size_t margsz = (std::max)(l.limbs().size(), r.limbs().size());
    
    if (!!(l.sign() + r.sign())) {
        std::get<2>(result) = 1 + margsz;
        std::get<0>(result) = alloc_traits_t::allocate(alloc, std::get<2>(result));
        std::get<3>(result) = l.sign();
        limb_t c = arithmetic::uadd<limb_t>(l.limbs(), r.limbs(), std::span{ std::get<0>(result), std::get<2>(result) });
        if (c) {
            *(std::get<0>(result) + std::get<2>(result) - 1) = c;
            std::get<1>(result) = margsz + 1;
        } else {
            std::get<1>(result) = margsz;
        }
    } else {
        std::strong_ordering cmpval = basic_integer_view{ l.limbs() } <=> basic_integer_view{ r.limbs() };
        if (cmpval == std::strong_ordering::equal) [[unlikely]] {
            std::get<0>(result) = nullptr;
            std::get<1>(result) = std::get<2>(result) = 0;
            std::get<3>(result) = 0;
        } else {
            std::get<1>(result) = std::get<2>(result) = margsz;
            std::get<0>(result) = alloc_traits_t::allocate(alloc, std::get<2>(result));
            if (cmpval == std::strong_ordering::less) {
                arithmetic::usub<limb_t>(r.limbs(), l.limbs(), std::span{ std::get<0>(result), std::get<2>(result) });
                std::get<3>(result) = r.sign();
            } else {
                assert(cmpval == std::strong_ordering::greater);
                arithmetic::usub<limb_t>(l.limbs(), r.limbs(), std::span{ std::get<0>(result), std::get<2>(result) });
                std::get<3>(result) = l.sign();
            }
            while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
                --std::get<1>(result);
            }
        }
    }
    return result;
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] auto sub(basic_integer_view<LimbT> l, basic_integer_view<LimbT> r, AllocatorT&& alloc)
{
    return add(l, -r, alloc);
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] std::tuple<std::remove_cv_t<LimbT>*, size_t, size_t, int> mul(basic_integer_view<LimbT> l, basic_integer_view<LimbT> r, AllocatorT&& alloc)
{
    using limb_t = std::remove_cv_t<LimbT>;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    std::tuple<limb_t*, size_t, size_t, int> result;
    
    size_t margsz = l.limbs().size() + r.limbs().size();
    std::get<1>(result) = std::get<2>(result) = margsz;
    std::get<0>(result) = alloc_traits_t::allocate(alloc, std::get<2>(result));
    std::get<3>(result) = !(l.sign() + r.sign()) ? -1 : 1;
    if (basic_integer_view{ l.limbs() } >= basic_integer_view{ r.limbs() }) {
        arithmetic::umul<limb_t>(l.limbs(), r.limbs(), std::span{ std::get<0>(result), std::get<2>(result) });
    } else {
        arithmetic::umul<limb_t>(r.limbs(), l.limbs(), std::span{ std::get<0>(result), std::get<2>(result) });
    }
    while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
        --std::get<1>(result);
    }
    return result;
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] std::tuple<std::remove_cv_t<LimbT>*, size_t, size_t, int> div(basic_integer_view<LimbT> l, basic_integer_view<LimbT> r, AllocatorT&& alloc)
{
    using limb_t = std::remove_cv_t<LimbT>;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    std::tuple<limb_t*, size_t, size_t, int> result;

    size_t margsz = l.limbs().size() + r.limbs().size();
    std::get<2>(result) = margsz;
    limb_t* pls = alloc_traits_t::allocate(alloc, std::get<2>(result));
    std::get<3>(result) = !(l.sign() + r.sign()) ? -1 : 1;
    std::span q{ pls, l.limbs().size() };
    std::span res{ pls + l.limbs().size(), r.limbs().size() };
    arithmetic::udiv<limb_t>(l.limbs(), r.limbs(), q, res);
    std::get<0>(result) = q.data();
    std::get<1>(result) = q.size();
    while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
        --std::get<1>(result);
    }
    return result;
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] std::tuple<std::remove_cv_t<LimbT>*, size_t, size_t, int> mod(basic_integer_view<LimbT> l, basic_integer_view<LimbT> r, AllocatorT&& alloc)
{
    using limb_t = std::remove_cv_t<LimbT>;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    std::tuple<limb_t*, size_t, size_t, int> result;

    size_t margsz = l.limbs().size() + r.limbs().size();
    std::get<2>(result) = margsz;
    limb_t * pls = alloc_traits_t::allocate(alloc, std::get<2>(result));
    std::get<3>(result) = !(l.sign() + r.sign()) ? -1 : 1;
    std::span q{ pls + r.limbs().size(), l.limbs().size() };
    std::span res{ pls, r.limbs().size() };
    arithmetic::udiv<limb_t>(l.limbs(), r.limbs(), q, res);
    std::get<0>(result) = res.data();
    std::get<1>(result) = res.size();
    while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
        --std::get<1>(result);
    }
    return result;
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] auto pow(basic_integer_view<LimbT> l, unsigned int n, AllocatorT&& alloc)
{
    using limb_t = std::remove_cv_t<LimbT>;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    std::tuple<limb_t*, size_t, size_t, int> result;

    if (!n) [[unlikely]] {
        std::get<0>(result) = nullptr;
        return result;
    }

    LimbT ibuf[1] = { 1 };
    /*std::get<0>(result) = ibuf;
    std::get<1>(result) = 1;
    std::get<2>(result) = 0;
    std::get<3>(result) = l.sign();
    basic_integer_view<LimbT> rview { std::span{ std::get<0>(result), std::get<1>(result) }, std::get<3>(result) };*/
    basic_integer_view<LimbT> rview{ std::span{ibuf} };
    basic_integer_view<LimbT> base{ l };
    size_t ralloc = 0, balloc = 0;
    for (;;) {
        if (n & 1) {
            result = mul(base, rview, alloc);
            if (ralloc) { alloc_traits_t::deallocate(alloc, rview.limbs().data(), ralloc); }
            rview = basic_integer_view<LimbT>{ std::span{std::get<0>(result), std::get<1>(result)}, std::get<3>(result) }; ralloc = std::get<2>(result);
        }
        n >>= 1;
        if (!n) break;
        auto [limbs, sz, rsz, sign] = mul(base, base, alloc);
        if (balloc) { alloc_traits_t::deallocate(alloc, base.limbs().data(), balloc); }
        base = basic_integer_view<LimbT>{ std::span{limbs, sz}, sign }; balloc = rsz;
    }
    if (balloc) { alloc_traits_t::deallocate(alloc, base.limbs().data(), balloc); }
    while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
        --std::get<1>(result);
    }
    //std::get<0>(result)
    //std::get<3>(result) = l.sign() > 0 ? 1 : ((n & 1) ? -1 : 1);
    //std::get<1>(result) = std::get<2>(result) = margsz;
    return result;
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] auto binand(basic_integer_view<LimbT> l, basic_integer_view<LimbT> r, AllocatorT&& alloc)
{
    using limb_t = std::remove_cv_t<LimbT>;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    std::tuple<limb_t*, size_t, size_t, int> result;
    size_t margsz = (std::min)(l.limbs().size(), r.limbs().size());
    std::get<3>(result) = l.sign() * r.sign();
    std::get<1>(result) = std::get<2>(result) = margsz;
    if (!margsz) [[unlikely]] {
        std::get<0>(result) = nullptr;
        return result;
    }
    std::get<0>(result) = alloc_traits_t::allocate(alloc, margsz);
    
    LimbT const* ub = l.limbs().data(), * ue = ub + margsz;
    LimbT const* vb = r.limbs().data();
    LimbT* rb = std::get<0>(result);
    for (; ub != ue; ++ub, ++vb, ++rb) {
        *rb = *ub & *vb;
    }
    while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
        --std::get<1>(result);
    }
    return result;
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] auto binor(basic_integer_view<LimbT> l, basic_integer_view<LimbT> r, AllocatorT&& alloc)
{
    using limb_t = std::remove_cv_t<LimbT>;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    std::tuple<limb_t*, size_t, size_t, int> result;
    size_t margsz = (std::max)(l.limbs().size(), r.limbs().size());
    std::get<3>(result) = l.sign() * r.sign();
    std::get<1>(result) = std::get<2>(result) = margsz;
    if (!margsz) [[unlikely]] {
        std::get<0>(result) = nullptr;
        return result;
    }
    std::get<0>(result) = alloc_traits_t::allocate(alloc, margsz);
    arithmetic::uor<limb_t>(l.limbs(), r.limbs(), std::span{ std::get<0>(result), std::get<2>(result) });
    
    return result;
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] auto binxor(basic_integer_view<LimbT> l, basic_integer_view<LimbT> r, AllocatorT&& alloc)
{
    using limb_t = std::remove_cv_t<LimbT>;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    std::tuple<limb_t*, size_t, size_t, int> result;
    size_t margsz = (std::max)(l.limbs().size(), r.limbs().size());
    std::get<3>(result) = l.sign() * r.sign();
    std::get<1>(result) = std::get<2>(result) = margsz;
    if (!margsz) [[unlikely]] {
        std::get<0>(result) = nullptr;
        return result;
    }
    std::get<0>(result) = alloc_traits_t::allocate(alloc, margsz);
    arithmetic::uxor<limb_t>(l.limbs(), r.limbs(), std::span{ std::get<0>(result), std::get<2>(result) });
    while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
        --std::get<1>(result);
    }
    return result;
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] auto binnot(basic_integer_view<LimbT> l, AllocatorT&& alloc)
{
    using limb_t = std::remove_cv_t<LimbT>;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    std::tuple<limb_t*, size_t, size_t, int> result;
    size_t margsz = l.limbs().size();
    std::get<3>(result) = -l.sign();
    std::get<1>(result) = std::get<2>(result) = margsz;
    if (!margsz) [[unlikely]] {
        std::get<0>(result) = nullptr;
        return result;
    }
    std::get<0>(result) = alloc_traits_t::allocate(alloc, margsz);
    LimbT const* lb = l.limbs().data(), * le = lb + margsz;
    LimbT* rb = std::get<0>(result);
    for (;lb != le; ++lb, ++rb) {
        *rb = ~*lb;
    }
    while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
        --std::get<1>(result);
    }
    return result;
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] auto shift_left(basic_integer_view<LimbT> l, unsigned int n, AllocatorT&& alloc)
{
    using limb_t = std::remove_cv_t<LimbT>;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    std::tuple<limb_t*, size_t, size_t, int> result;

    if (!l.limbs().size()) [[unlikely]] {
        result = { nullptr, 0, 0, l.sign() };
        return result;
    }
    std::get<3>(result) = l.sign();

    size_t shift = n & (std::numeric_limits<LimbT>::digits - 1);
    size_t rsz = (n >> sonia::arithmetic::consteval_log2(size_t(std::numeric_limits<LimbT>::digits)));
    std::get<1>(result) = std::get<2>(result) = rsz + !!shift + l.limbs().size();
    limb_t* pls = std::get<0>(result) = alloc_traits_t::allocate(alloc, std::get<2>(result));
    std::fill(pls, pls + rsz, 0);
    if (shift) {
        arithmetic::ushift_left<limb_t>(l.limbs(), shift, std::span{ pls + rsz, l.limbs().size() + 1 });
    } else {
        std::copy(l.limbs().begin(), l.limbs().end(), pls + rsz);
    }
    while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
        --std::get<1>(result);
    }
    return result;
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] auto shift_right(basic_integer_view<LimbT> l, unsigned int n, AllocatorT&& alloc)
{
    using limb_t = std::remove_cv_t<LimbT>;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    std::tuple<limb_t*, size_t, size_t, int> result;

    size_t rsz = (n >> sonia::arithmetic::consteval_log2(size_t(std::numeric_limits<LimbT>::digits)));

    if (rsz >= l.limbs().size()) {
        result = { nullptr, 0, 0, l.sign() };
        return result;
    }
    std::get<3>(result) = l.sign();
    size_t shift = n & (std::numeric_limits<LimbT>::digits - 1);
    std::get<1>(result) = std::get<2>(result) = l.limbs().size() - rsz;
    limb_t* pls = std::get<0>(result) = alloc_traits_t::allocate(alloc, std::get<2>(result));
    if (shift) {
        arithmetic::ushift_right<limb_t>(l.limbs().subspan(rsz), shift, std::span{ pls, std::get<2>(result) });
    } else {
        std::copy_backward(l.limbs().begin() + rsz, l.limbs().end(), pls + std::get<2>(result));
    }
    while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
        --std::get<1>(result);
    }
    return result;
}

template <std::unsigned_integral LimbT, typename AllocatorT>
class basic_integer : private std::allocator_traits<AllocatorT>::template rebind_alloc<LimbT>
{
    using allocator_type = std::allocator_traits<AllocatorT>::template rebind_alloc<LimbT>;

public:
    basic_integer() : size_{0}, reserved_size_{0} {}
        
    basic_integer(AllocatorT const& alloc) : allocator_type{ alloc }, size_{ 0 }, reserved_size_{ 0 } {}

    basic_integer(AllocatorT const& alloc, std::span<LimbT> limbs, int32_t sgnsz)
        : allocator_type{alloc}
        , limbs_{ limbs.data() }
        , reserved_size_{ static_cast<uint32_t>(limbs.size()) }
        , size_ { sgnsz }
    {}

    ~basic_integer()
    {
        if (reserved_size_) {
            std::allocator_traits<allocator_type>::deallocate(*this, limbs_, (size_t)reserved_size_);
            //this->deallocate(limbs_);
        }
    }

    inline int32_t sign() const noexcept { return size_ < 0 ? -1 : 1; }
    //inline intptr_t limbs_size() const noexcept { return size_; };
private:
    LimbT* limbs_;
    int32_t size_;
    uint32_t reserved_size_;
};

#if 0
template <std::integral LimbT>
requires(std::is_unsigned_v<LimbT>)
LimbT size_in_base(std::span<const LimbT> limbs, int base)
{
    int __lb_base;
									
    assert (base >= 2);
    
    if (limbs.empty()) return 1;
    
	/* Calculate the total number of significant bits of X.  */
    LimbT lzcnt = count_leading_zeros(limbs.back());
    size_t totalbits = limbs.size() * sizeof(LimbT) * CHAR_BIT - lzcnt;
									
	if (0 == (base & (base - 1))) { // if base is a power of 2
        LimbT lb_base = mp_bases[base].big_base;
	    return (totalbits + lb_base - 1) / lb_base;
    } else {
        auto [ph, _dummy] = umulpp(mp_bases[base].logb2 + 1, totalbits);
        return ph + 1;
    }
}
#endif

// limbs are desructed during the string conversion
template <std::unsigned_integral LimbT, typename OutputIteratorT>
requires(!std::is_const_v<LimbT>)
OutputIteratorT bc_get_str(std::span<LimbT> limbs, int base, std::string_view alphabet, OutputIteratorT oi)
{
    using namespace sonia::arithmetic;
    namespace mpa = sonia::mp::arithmetic;
    using mpct::W;

    assert(!limbs.empty());
    assert(alphabet.size() >= base);
    constexpr uint32_t limb_bit_count = sizeof(LimbT) * CHAR_BIT;
    using limb_traits_t = uint_t<limb_bit_count>;
    
    static_assert(sizeof(LimbT) <= 8);
    char tempbuff[64]; // not more than 64 chars per limb

    if (base == 10) { // questionable choice
        while (limbs.size() > 1) {
            constexpr auto max_dec_base_c = upow(W<10>, W<std::numeric_limits<LimbT>::digits10>);
            constexpr uint64_t max_dec_base = mpct::front<decltype(max_dec_base_c)>;
            //constexpr uint64_t l = 1 + consteval_log2<LimbT, limb_bit_count>(max_dec_base);
            //constexpr uint32_t shift = limb_bit_count - l;

            auto [frac, r] = mpa::udivby1<LimbT, max_dec_base, true>(limbs);
            frac += 1;

            limbs = std::span{ limbs.data(), limbs.size() - (limbs.back() == 0) };
            
            constexpr size_t chars_per_limb = std::numeric_limits<LimbT>::digits10;
            char* s = tempbuff + chars_per_limb - 1;

            if constexpr (sizeof(LimbT) <= 4) {
                for (;;) {
                    auto [d, f] = umul1<LimbT>(frac, 10);
                    frac = f;
                    *s = alphabet[static_cast<uint8_t>(d)];
                    if (s == tempbuff) break;
                    --s;
                }
            } else {
                //static_assert(!shift);
                auto [d0, f0] = umul1<LimbT>(frac, 10);
                frac = f0;
                *s-- = alphabet[static_cast<uint8_t>(d0)];

                auto [d1, f1] = umul1<LimbT>(frac, 10);
                frac = f1;
                *s-- = alphabet[static_cast<uint8_t>(d1)];

                auto [d2, f2] = umul1<LimbT>(frac, 10);
                frac = f2;
                *s-- = alphabet[static_cast<uint8_t>(d2)];

                auto [d4, f4] = umul1<LimbT>(frac, 10);
                frac = f4;
                *s-- = alphabet[static_cast<uint8_t>(d4)];

                frac = (frac + 15) >> 4;
                for (;;) {
                    frac *= 10;
                    LimbT digit = frac >> (sizeof(LimbT) * CHAR_BIT - 4);
                    *s = alphabet[static_cast<uint8_t>(digit)];
                    if (s == tempbuff) break;
                    --s;
                    frac &= (~(LimbT)0) >> 4;
                }
            }
            oi = std::copy(tempbuff, tempbuff + chars_per_limb, std::move(oi));
        }
    } else {
        size_t chars_per_limb = size_t(std::floor(double(limb_bit_count) / std::log2(base)));
        LimbT big_base = ipow<LimbT>(base, chars_per_limb);

        int shift = count_leading_zeros(big_base);
        int l = limb_bit_count - shift;
        LimbT u1 = (shift ? (LimbT{ 1 } << l) : 0) - big_base;
        auto [big_base_inverted, dummy] = udiv2by1<LimbT>(u1, 0, big_base);

        while (limbs.size() > 1) {
            LimbT r = mpa::udivby1<LimbT>(limbs, big_base, big_base_inverted, l);
            LimbT frac;
            udiv2by1<LimbT>(frac, r, r << shift, 0, big_base << shift, big_base_inverted);
            frac += 1;
            r >>= shift;
            limbs = std::span{ limbs.data(), limbs.size() - (limbs.back() == 0) };

            char* s = tempbuff + chars_per_limb - 1;

            for (;;) {
                auto [d, f] = umul1<LimbT>(frac, base);
                frac = f;
                *s = alphabet[static_cast<uint8_t>(d)];
                if (s == tempbuff) break;
                --s;
            }
            oi = std::copy(tempbuff, tempbuff + chars_per_limb, std::move(oi));
        }
    }

    for (LimbT ul = limbs.front(); ul; ++oi)
    {
        auto [q, r] = udiv1<LimbT>(ul, base);
        ul = q;
        *oi = alphabet[static_cast<uint8_t>(r)];
    }
    
    return std::move(oi);
}

template <std::unsigned_integral LimbT, typename OutputIteratorT>
OutputIteratorT to_string_converter(std::span<LimbT> limbs, OutputIteratorT out, bool& reversed, int base = 10, std::string_view alphabet = {})
{
    using namespace std::string_view_literals;
    using limb_type = std::remove_cv_t<LimbT>;

    if (base < 2 || base > (alphabet.empty() ? 62 : alphabet.size())) {
        throw std::invalid_argument("wrong base");
    }
    if (limbs.empty()) {
        reversed = false;
        *out = alphabet.empty() ? '0' : alphabet[0];
        ++out;
        return std::move(out);
    }

    if (alphabet.empty()) {
        alphabet = base <= 36 ? detail::default_alphabet : detail::default_alphabet_big;
    }

    if (!(base & (base - 1))) {
        reversed = false;
        uint_least8_t mask = static_cast<uint_least8_t>(base - 1);
        uint_least8_t bits_per_digit = 0;
        for (int tmp = base - 1; tmp; ++bits_per_digit, tmp >>= 1);
        uint_least8_t offset_bits = (sizeof(LimbT) * limbs.size() * CHAR_BIT) % bits_per_digit;
        if (!offset_bits) offset_bits = bits_per_digit;
        int limb_pos = static_cast<int>(sizeof(LimbT) * CHAR_BIT) - offset_bits;
        const limb_type* plimb = limbs.data() + limbs.size() - 1;
        
        // skip leading zeros
        bool skip_zeroes = true;
        for (limb_type cur = *plimb, carry = 0;;) {
            limb_type nf = carry | (cur >> limb_pos);
            uint_least8_t n = static_cast<uint_least8_t>(nf) & mask;
            if (n || !skip_zeroes) {
                carry = 0;
                skip_zeroes = false;
                *out = alphabet[n];
                ++out;
            }
            limb_pos -= bits_per_digit;
            if (limb_pos >= 0) {
                continue;
            } else if (plimb != limbs.data()) {
                carry = static_cast<uint_least8_t>(cur) << (-limb_pos); // no need mask
                --plimb;
                cur = *plimb;
                limb_pos += static_cast<int>(sizeof(LimbT) * CHAR_BIT);
            } else {
                if (skip_zeroes) {
                    *out = alphabet[0];
                    ++out;
                }
                return std::move(out);
            }
        }
    }

    //if (un < 35)
    reversed = true;
    if constexpr (std::is_const_v<LimbT>) {
        std::vector<limb_type> mls( limbs.begin(), limbs.end() );
        return bc_get_str(std::span{mls}, base, alphabet, std::move(out));
    } else {
        return bc_get_str(limbs, base, alphabet, std::move(out));
    }

    /*
    powtab_mem = new mp_limb_t[2 + un];
    //powtab_mem = ((mp_limb_t*)__gmp_tmp_reentrant_alloc(&__tmp_marker, (un + 2 * 64) * sizeof(mp_limb_t)));
    size_t ndig;
    mp_size_t xn;
    
    ndig = umulpp(__gmpn_bases[base].logb2, sizeof(LimbT) * CHAR_BIT * limbs.size()).first;
    
    xn = 1 + ndig / __gmpn_bases[base].chars_per_limb;
    pi = 1 + __gmpn_compute_powtab(powtab, powtab_mem, xn, base);
    tmp = ((mp_limb_t*)__gmp_tmp_reentrant_alloc(&__tmp_marker, (((un)+64)) * sizeof(mp_limb_t)));
    out_len = mpn_dc_get_str(str, 0, up, un, powtab + (pi - 1), tmp) - str;

    delete powtab_mem[];
    //__gmp_tmp_reentrant_free(__tmp_marker);
    
    return out_len;


    if (base == 10) {
    
    }

    return std::move(out);
    */
}


/*
template <typename LimbT, typename AllocatorT>
basic_integer<LimbT, AllocatorT> mul(basic_integer_view<LimbT> const& u, basic_integer_view<LimbT> const& v, AllocatorT const& alloc = std::allocator<LimbT>{})
{
    std::span<LimbT> uls = u;
    std::span<LimbT> vls = v;
    if (uls.size() < vls.size()) {
        std::swap(uls, vls);
    }
    if (!vls.empty()) {
        return {};
    }
    size_t rsize = uls.size() + vls.size();
    auto rsign = u.sign() ^ v.sign();

    using allocator_type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<LimbT>;
    LimbT* rlimbs = allocator_type{ alloc }.allocate(rsize);
    auto cy_limb = mpn_mul(rlimbs, uls.data(), uls.size(), vls.data(), vls.size());
    rsize -= cy_limb;
    return { alloc, std::span{rlimbs, rsize}, rsign };
}
*/

/*
template <size_t MinBits, size_t MaxBits, typename AllocatorT>
class integer
{
    using limb_t = 
    using type = typename std::allocator_traits<AllocatorT>::template rebind_alloc<value_type>;
    union {
        
    };
public:
    template <std::integral T>
    static void create(T val)
};
*/

}
