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
auto copy(basic_integer_view<LimbT> arg, AllocatorT&& alloc)
{
    std::tuple<LimbT*, size_t, size_t, int> result;
    if (!arg.size())[[unlikely]] {
        result = { nullptr, 0, 0, 1 };
    } else {
        using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
        result = {
            alloc_traits_t::allocate(alloc, arg.size()),
            arg.size(),
            arg.size(),
            arg.sgn()
        };
        auto r = std::copy(arg.data(), arg.data() + arg.size(), std::get<0>(result));
        if (arg.most_significant_skipping_bits()) {
            *(r - 1) &= arg.last_limb_mask();
        }
    }
    return result;
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] auto add(basic_integer_view<LimbT> l, basic_integer_view<LimbT> r, AllocatorT&& alloc)
{
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    
    std::tuple<LimbT*, size_t, size_t, int> result;
    if (!l.size()) return copy(r, std::forward<AllocatorT>(alloc));
    if (!r.size()) return copy(l, std::forward<AllocatorT>(alloc));
        
    size_t margsz = l.size();

    std::get<3>(result) = l.sgn();

    LimbT const* lb = l.data(), * le = l.data() + l.size() - 1;
    LimbT const* rb = r.data(), * re = r.data() + r.size() - 1;
    
    LimbT last_l = l.most_significant_skipping_bits() ? *le & l.last_limb_mask() : *le;
    LimbT last_r = r.most_significant_skipping_bits() ? *re & r.last_limb_mask() : *re;
        
    if (l.size() < r.size()) {
        std::swap(lb, rb);
        std::swap(le, re);
        std::swap(last_l, last_r);
        std::get<3>(result) = r.sgn();
        margsz = r.size();
    }

    if (l.sgn() - r.sgn() == 0) { // the same signs
        //if (lb == le) {} // one limb optimization
        auto [ctmp, _] = sonia::arithmetic::uadd1c(last_l, last_r, LimbT{1}); // if ctmp == 0 => no need additional limb result
        std::get<2>(result) = ctmp + margsz;
        std::get<0>(result) = alloc_traits_t::allocate(alloc, std::get<2>(result));
        LimbT* res = std::get<0>(result);
        LimbT c = arithmetic::uadd_unchecked(last_l, lb, le, last_r, rb, re, res);
        if (c) {
            *res = c;
            std::get<1>(result) = margsz + 1;
        } else {
            std::get<1>(result) = margsz;
        }
    } else {
        if (l.size() == r.size()) {
            while (last_l == last_r) {
                if (lb == le) {
                    result = { nullptr, 0, 0, 1 };
                    return result;
                }
                last_l = *--le;
                last_r = *--re;
            }
            if (last_l < last_r) {
                std::swap(lb, rb);
                std::swap(le, re);
                std::swap(last_l, last_r);
                std::get<3>(result) = r.sgn();
                margsz = r.size();
            }
        }
        std::get<0>(result) = alloc_traits_t::allocate(alloc, margsz);
        std::get<1>(result) = std::get<2>(result) = margsz;
        LimbT* res = std::get<0>(result);
        LimbT c = arithmetic::usub_unchecked(lb, le, last_l, rb, re, last_r, res);
        assert(!c);
        for (; !*res; --res) { // need not to check res >= std::get<0>(result)
            --std::get<1>(result);
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

    return l.with_limbs([r, &alloc](std::span<const LimbT> llimbs, int lsign) {
        return r.with_limbs([llimbs, lsign, &alloc](std::span<const LimbT> rlimbs, int rsign) {
            std::tuple<limb_t*, size_t, size_t, int> result;
    
            size_t margsz = llimbs.size() + rlimbs.size();
            std::get<1>(result) = std::get<2>(result) = margsz;
            std::get<0>(result) = alloc_traits_t::allocate(alloc, std::get<2>(result));
            std::get<3>(result) = !(lsign + rsign) ? -1 : 1;
            if (basic_integer_view{ llimbs } >= basic_integer_view{ rlimbs }) {
                arithmetic::umul<limb_t>(llimbs, rlimbs, std::span{ std::get<0>(result), std::get<2>(result) });
            } else {
                arithmetic::umul<limb_t>(rlimbs, llimbs, std::span{ std::get<0>(result), std::get<2>(result) });
            }
            while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
                --std::get<1>(result);
            }
            return result;
        });
    });
}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] std::tuple<LimbT*, size_t, size_t, int> div(basic_integer_view<LimbT> l, basic_integer_view<LimbT> r, AllocatorT&& alloc)
{
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;

    return l.with_limbs([r, &alloc](std::span<const LimbT> llimbs, int lsign) {
        return r.with_limbs([llimbs, lsign, &alloc](std::span<const LimbT> rlimbs, int rsign) {
            std::tuple<LimbT*, size_t, size_t, int> result;

            size_t margsz = llimbs.size() + rlimbs.size();
            std::get<2>(result) = margsz;
            LimbT* pls = alloc_traits_t::allocate(alloc, std::get<2>(result));
            std::get<3>(result) = !(lsign + rsign) ? -1 : 1;
            std::span q{ pls, llimbs.size() };
            std::span res{ pls + llimbs.size(), rlimbs.size() };
            arithmetic::udiv<LimbT>(llimbs, rlimbs, q, res);
            std::get<0>(result) = q.data();
            std::get<1>(result) = q.size();
            assert(q.size() <= margsz);
            while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
                --std::get<1>(result);
            }
            return result;
        });
    });
}

//// s and d must be normilized (no heading zeros in sl and dl)
//template <std::unsigned_integral LimbT, typename AllocatorT>
//requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
//[[nodiscard]] std::tuple<LimbT*, size_t, size_t, int> div_qr(LimbT& sh, std::span<LimbT>& sl, int ssign, basic_integer_view<LimbT> d, AllocatorT&& alloc)
//{
//    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
//
//    std::tuple<LimbT*, size_t, size_t, int> result;
//    size_t ssz = sl.size() + (sh ? 1 : 0);
//    if (!ssz) [[unlikely]] { result = { nullptr, 0, 0, 1 }; return result; }
//    auto [dh, dl] = d.limbs();
//    unsigned int dh_cnt = dh ? 1 : 0;
//    size_t dsz = dl.size() + dh_cnt;
//    if (dsz > ssz) { result = { nullptr, 0, 0, 1 }; return result; }
//    size_t qsz = ssz - dsz + 1;
//    get<2>(result) = qsz;
//    LimbT* qlimbs = alloc_traits_t::allocate(alloc, std::get<2>(result));
//    get<3>(result) = !(ssign + d.sgn()) ? -1 : 1;
//    SCOPE_EXCEPTIONAL_EXIT([&alloc, &result] { alloc_traits_t::deallocate(alloc, get<0>(result), get<2>(result)); });
//
//    size_t dauxspace = (dl.size() + dh_cnt);
//    LimbT* daux = alloc_traits_t::allocate(alloc, 2 * dauxspace);
//    LimbT* daux_tmp = std::copy(dl.begin(), dl.end(), daux);
//    if (dh_cnt) *daux_tmp++ = dh;
//    SCOPE_EXIT([&alloc, daux, dauxspace] { alloc_traits_t::deallocate(alloc, daux, 2 * dauxspace); });
//
//    std::span<LimbT> q{ qlimbs, qsz };
//    sh = arithmetic::udiv<LimbT>(sl, &sh, { daux, dauxspace }, { daux_tmp, dauxspace }, q);
//    assert(q.data() == qlimbs);
//    std::get<0>(result) = qlimbs;
//    std::get<1>(result) = q.size();
//    return result;
//}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
[[nodiscard]] std::tuple<std::remove_cv_t<LimbT>*, size_t, size_t, int> mod(basic_integer_view<LimbT> l, basic_integer_view<LimbT> r, AllocatorT&& alloc)
{
    using limb_t = std::remove_cv_t<LimbT>;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    return l.with_limbs([r, &alloc](std::span<const LimbT> llimbs, int lsign) {
        return r.with_limbs([llimbs, lsign, &alloc](std::span<const LimbT> rlimbs, int rsign) {
            std::tuple<limb_t*, size_t, size_t, int> result;

            size_t margsz = llimbs.size() + rlimbs.size();
            std::get<2>(result) = margsz;
            limb_t * pls = alloc_traits_t::allocate(alloc, std::get<2>(result));
            std::get<3>(result) = !(lsign + rsign) ? -1 : 1;
            std::span q{ pls + rlimbs.size(), llimbs.size() };
            std::span res{ pls, rlimbs.size() };
            arithmetic::udiv<limb_t>(llimbs, rlimbs, q, res);
            std::get<0>(result) = res.data();
            std::get<1>(result) = res.size();
            while (std::get<1>(result) && !*(std::get<0>(result) + std::get<1>(result) - 1)) {
                --std::get<1>(result);
            }
            return result;
        });
    });
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
            if (ralloc) { alloc_traits_t::deallocate(alloc, const_cast<LimbT*>(rview.data()), ralloc); }
            rview = basic_integer_view<LimbT>{ std::span{std::get<0>(result), std::get<1>(result)}, std::get<3>(result) }; ralloc = std::get<2>(result);
        }
        n >>= 1;
        if (!n) break;
        auto [limbs, sz, rsz, sign] = mul(base, base, alloc);
        if (balloc) { alloc_traits_t::deallocate(alloc, const_cast<LimbT*>(base.data()), balloc); }
        base = basic_integer_view<LimbT>{ std::span{limbs, sz}, sign }; balloc = rsz;
    }
    if (balloc) { alloc_traits_t::deallocate(alloc, const_cast<LimbT*>(base.data()), balloc); }
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
    std::get<3>(result) = l.sgn() * r.sgn();
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
    std::get<3>(result) = l.sgn() * r.sgn();
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
    std::get<3>(result) = l.sgn() * r.sgn();
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
    std::get<3>(result) = -l.sgn();
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
        result = { nullptr, 0, 0, l.sgn() };
        return result;
    }
    std::get<3>(result) = l.sgn();

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
        result = { nullptr, 0, 0, l.sgn() };
        return result;
    }
    std::get<3>(result) = l.sgn();
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

}
