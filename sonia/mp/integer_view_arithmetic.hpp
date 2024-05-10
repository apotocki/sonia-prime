/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <vector>
//#include <iterator>

#include "integer_view.hpp"
#include "limb_arithmetic.hpp"

// to do: look at https://github.com/google/double-conversion

namespace sonia::mp {

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
        result = { alloc_traits_t::allocate(alloc, 1), 1, 1, 1 };
        *std::get<0>(result) = 1;
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
            if (ralloc) { alloc_traits_t::deallocate(alloc, const_cast<LimbT*>(rview.limbs().data()), ralloc); }
            rview = basic_integer_view<LimbT>{ std::span{std::get<0>(result), std::get<1>(result)}, std::get<3>(result) }; ralloc = std::get<2>(result);
        }
        n >>= 1;
        if (!n) break;
        auto [limbs, sz, rsz, sign] = mul(base, base, alloc);
        if (balloc) { alloc_traits_t::deallocate(alloc, const_cast<LimbT*>(base.limbs().data()), balloc); }
        base = basic_integer_view<LimbT>{ std::span{limbs, sz}, sign }; balloc = rsz;
    }
    if (balloc) { alloc_traits_t::deallocate(alloc, const_cast<LimbT*>(base.limbs().data()), balloc); }
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
    size_t totalbits = limbs.size() * std::numeric_limits<LimbT>::digits - lzcnt;
									
	if (0 == (base & (base - 1))) { // if base is a power of 2
        LimbT lb_base = mp_bases[base].big_base;
	    return (totalbits + lb_base - 1) / lb_base;
    } else {
        auto [ph, _dummy] = umulpp(mp_bases[base].logb2 + 1, totalbits);
        return ph + 1;
    }
}
#endif





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
