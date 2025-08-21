/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <span>
#include <tuple>

#include "arithmetic.hpp"
#include "ct.hpp"

#include "sonia/utility/scope_exit.hpp"
#include "sonia/utility/small_array.hpp"

#ifndef DC_DIV_QR_THRESHOLD
#   define DC_DIV_QR_THRESHOLD 50
#endif

#ifndef SONIA_MP_INPLACE_LIMB_RESERVE_COUNT
#   define SONIA_MP_INPLACE_LIMB_RESERVE_COUNT 8
#endif

namespace sonia::mp::arithmetic {

using sonia::container::small_array;

template <std::unsigned_integral LimbT>
inline void ushift_right(std::span<const LimbT> u, unsigned int shift, LimbT* r) noexcept
{
    assert(!u.empty());
    assert(shift < std::numeric_limits<LimbT>::digits);

    size_t lshift = std::numeric_limits<LimbT>::digits - shift;

    LimbT const* ub = u.data(), * ue = &u.back();
    LimbT* re = r + u.size() - 1;

    LimbT result = *ue << lshift;
    *re = (*ue) >> shift;
    while (ub != ue) {
        *re-- |= (*--ue) << lshift;
        *re = (*ue) >> shift;
    }
    return result;
}

// inplace
template <std::unsigned_integral LimbT>
inline LimbT ushift_right(std::span<LimbT> u, unsigned int shift) noexcept
{
    return ushift_right<LimbT>(u, shift, u.data());
}

template <std::unsigned_integral LimbT>
inline LimbT ushift_right(LimbT& uh, std::span<const LimbT> u, unsigned int shift, LimbT* rb) noexcept
{
    assert(shift < std::numeric_limits<LimbT>::digits);
    size_t lshift = std::numeric_limits<LimbT>::digits - shift;
    LimbT result;
    
    if (u.empty()) {
        result = uh << lshift;
    } else {
        LimbT const* ub = u.data(), * ue = &u.back();

        result = *ub << lshift;
        *rb = (*ub) >> shift;
        while (ub != ue) {
            *rb++ |= (*++ub) << lshift;
            *rb = (*ub) >> shift;
        }
        *rb |= uh << lshift;
    }
    uh >>= shift;

    return result;
}

//// r = [U / 2 ^ shift]
//template <std::unsigned_integral LimbT>
//LimbT ushift_right(LimbT& uh, std::span<const LimbT> ul, unsigned int shift, LimbT* rl)
//{
//    assert(shift < std::numeric_limits<LimbT>::digits);
//    size_t lshift = std::numeric_limits<LimbT>::digits - shift;
//    LimbT rll = uh << lshift;
//    uh >>= shift;
//    if (!ul.empty()) {
//        LimbT const* ub = ul.data(), * ue = ub + ul.size() - 1;
//        LimbT* re = rl + ul.size() - 1;
//        for (;; --re, --ue) {
//            LimbT tmp = rll | ((*ue) >> shift);
//            rll = (*ue) << lshift;
//            *re = tmp;
//            if (ub == ue) [[unlikely]] break;
//        }
//    }
//    return rll;
//}

template <std::unsigned_integral LimbT>
inline LimbT ushift_right(LimbT& uh, std::span<LimbT> u, unsigned int shift) noexcept
{
    return ushift_right<LimbT>(uh, u, shift, u.data());
}

// r = U * 2 ^ shift
template <std::unsigned_integral LimbT>
LimbT ushift_left(LimbT& uh, std::span<const LimbT> ul, unsigned int shift, LimbT* rl) noexcept
{
    assert(shift < std::numeric_limits<LimbT>::digits);
    size_t rshift = std::numeric_limits<LimbT>::digits - shift;
    LimbT uhh = uh >> rshift;
    uh <<= shift;
    if (!ul.empty()) {
        uh |= ul.back() >> rshift;
        LimbT const* ub = ul.data(), * ue = ub + ul.size() - 1;
        LimbT* re = rl + ul.size() - 1;
        for (;; --re) {
            *re = (*ue) << shift;
            if (ub == ue) [[unlikely]] break;
            --ue;
            (*re) |= (*ue) >> rshift;
        }
    }
    return uhh;
}

template <std::unsigned_integral LimbT>
inline LimbT ushift_left(std::span<LimbT> u, unsigned int shift) noexcept
{
    return ushift_left<LimbT>(u.back(), u.first(u.size() - 1), shift, u.data());
}

template <std::unsigned_integral LimbT>
inline void uor(std::span<const LimbT> u, std::span<const LimbT> v, std::span<LimbT> r) noexcept
{
    assert(r.size() >= (std::max)(u.size(), v.size()));
    LimbT const* ub = u.data(), * ue = ub + u.size();
    LimbT const* vb = v.data(), * ve = vb + v.size();
    LimbT* rb = r.data(), * re = rb + r.size();

    for (;; ++ub, ++vb, ++rb) {
        if (ub != ue) {
            if (vb != ve) {
                *rb = *ub | *vb;
                continue;
            } else {
                do {
                    *rb = *ub;
                    ++ub; ++rb;
                } while (ub != ue);
            }
        } else {
            for (; vb != ve; ++vb, ++rb) {
                *rb = *vb;
            }
        }
        break;
    }
}

// prereq: size(r) >= max(size(u), size(v))
template <std::unsigned_integral LimbT, typename OpT>
inline void perlimb_op(LimbT uh, std::span<const LimbT> u, LimbT vh, std::span<const LimbT> v, LimbT * rb, OpT && op) noexcept
{
    LimbT const* ub = u.data(), * ue = ub + u.size();
    LimbT const* vb = v.data(), * ve = vb + v.size();

    for (;; ++ub, ++vb, ++rb) {
        if (ub != ue) {
            if (vb != ve) {
                *rb = std::forward<OpT>(op)(*ub, *vb);
                continue;
            } else {
                *rb++ = std::forward<OpT>(op)(*ub++, vh);
                while (ub != ue) {
                    *rb++ = *ub++;
                }
                *rb = uh;
            }
        } else if (vb != ve) {
            *rb++ = std::forward<OpT>(op)(*vb++, uh);
            while (vb != ve) {
                *rb++ = *vb++;
            }
            *rb = vh;
        } else {
            *rb = std::forward<OpT>(op)(uh, vh);
        }
        break;
    }
}

template <std::unsigned_integral LimbT>
inline void uor(LimbT uh, std::span<const LimbT> u, LimbT vh, std::span<const LimbT> v, LimbT* r) noexcept
{
    perlimb_op<LimbT>(uh, u, vh, v, r, [](LimbT l, LimbT r)->LimbT { return l | r; });
}

template <std::unsigned_integral LimbT>
inline void uxor(LimbT uh, std::span<const LimbT> u, LimbT vh, std::span<const LimbT> v, LimbT* r) noexcept
{
    perlimb_op<LimbT>(uh, u, vh, v, r, [](LimbT l, LimbT r)->LimbT { return l ^ r; });
}

template <std::unsigned_integral LimbT>
inline void uxor(std::span<const LimbT> u, std::span<const LimbT> v, std::span<LimbT> r)
{
    assert(r.size() >= (std::max)(u.size(), v.size()));
    LimbT const* ub = u.data(), * ue = ub + u.size();
    LimbT const* vb = v.data(), * ve = vb + v.size();
    LimbT* rb = r.data(), * re = rb + r.size();

    for (;; ++ub, ++vb, ++rb) {
        if (ub != ue) {
            if (vb != ve) {
                *rb = *ub ^ *vb;
                continue;
            } else {
                do {
                    *rb = *ub;
                    ++ub; ++rb;
                } while (ub != ue);
            }
        } else {
            for (; vb != ve; ++vb, ++rb) {
                *rb = *vb;
            }
        }
        break;
    }
}

// u size must be >= v size
template <std::unsigned_integral LimbT>
inline LimbT uadd_partial_unchecked(LimbT const*& ub, LimbT const* vb, LimbT const* ve, LimbT *& rb)
{
    LimbT c = 0;
    for (; vb != ve; ++ub, ++vb, ++rb) {
        std::tie(c, *rb) = sonia::arithmetic::uadd1c(*ub, *vb, c);
    }
    return c;
}

// inplace version
template <std::unsigned_integral LimbT>
inline LimbT uadd_partial_unchecked(LimbT *& ub, LimbT const* vb, LimbT const* ve)
{
    LimbT c = 0;
    for (; vb != ve; ++ub, ++vb) {
        std::tie(c, *ub) = sonia::arithmetic::uadd1c(*ub, *vb, c);
    }
    return c;
}

// u size must be >= v size
template <std::unsigned_integral LimbT>
inline LimbT uadd_unchecked(LimbT const* ub, LimbT const* ue, LimbT const* vb, LimbT const* ve, LimbT* rb)
{
    LimbT c = uadd_partial_unchecked(ub, vb, ve, rb);
    if (c) {
        if (ub == ue) return c;
        do {
            std::tie(c, *rb++) = sonia::arithmetic::uadd1(*ub++, c);
            if (ub == ue) return c;
        } while (c);
    }
    std::copy(ub, ue, rb);
    return c;
}

// u size must be >= v size
// inplace version
template <std::unsigned_integral LimbT>
inline LimbT uadd_unchecked(LimbT * ub, LimbT const* ue, LimbT const* vb, LimbT const* ve)
{
    LimbT c = uadd_partial_unchecked(ub, vb, ve);
    if (c && ub != ue) {
        do {
            std::tie(c, *ub) = sonia::arithmetic::uadd1(*ub, c);
            if (!c) return c;
            ++ub;
        } while (ub != ue);
    }
    return c;
}

// u size must be >= v size
template <std::unsigned_integral LimbT>
inline LimbT uadd_unchecked(LimbT uh, LimbT const* ub, LimbT const* ue, LimbT vh, LimbT const* vb, LimbT const* ve, LimbT*& rb)
{
    LimbT c = uadd_partial_unchecked(ub, vb, ve, rb);
    if (ub != ue) {
        std::tie(c, *rb++) = sonia::arithmetic::uadd1c(*ub++, vh, c);
        for (; ub != ue; ++ub, ++rb) {
            std::tie(c, *rb) = sonia::arithmetic::uadd1(*ub, c);
        }
        std::tie(c, *rb++) = sonia::arithmetic::uadd1(uh, c);
    } else {
        std::tie(c, *rb++) = sonia::arithmetic::uadd1c(uh, vh, c);
    }
    return c;
}

template <std::unsigned_integral LimbT>
inline LimbT uadd(std::span<const LimbT> u, std::span<const LimbT> v, std::span<LimbT> r) noexcept
{
    assert(r.size() >= (std::max)(u.size(), v.size()));
    LimbT const* ub = u.data(), * ue = ub + u.size();
    LimbT const* vb = v.data(), * ve = vb + v.size();
    LimbT* rb = r.data();

    if (u.size() >= v.size()) {
        return uadd_unchecked(ub, ue, vb, ve, rb);
    } else {
        return uadd_unchecked(vb, ve, ub, ue, rb);
    }
}

// inplace +=
template <std::unsigned_integral LimbT>
inline LimbT uadd(std::span<LimbT> u, std::span<const LimbT> v) noexcept
{
    return uadd<LimbT>(u, v, u);
}

// prereqs: ur.size() + 1 >= v.size()
//template <std::unsigned_integral LimbT>
//LimbT uadd(LimbT& urh, std::span<LimbT> ur, std::span<const LimbT> v)
//{
//    auto vlsp = v.subspan((std::max)(ur.size(), v.size()));
//    LimbT c = uadd<LimbT>(ur, vlsp, ur);
//    if (vlsp.size() < v.size()) {
//        assert(vlsp.size() + 1 == v.size());
//        std::tie(c, urh) = sonia::arithmetic::uadd1c(urh, v.front(), c);
//    } else {
//        std::tie(c, urh) = sonia::arithmetic::uadd1(urh, c);
//    }
//    return c;
//}

// u size must be >= v size
template <std::unsigned_integral LimbT>
inline LimbT usub_partial_unchecked(LimbT const*& ub, LimbT const* vb, LimbT const* ve, LimbT*& rb)
{
    LimbT c = 0;
    for (; vb != ve; ++ub, ++vb, ++rb) {
        std::tie(c, *rb) = sonia::arithmetic::usub1c(*ub, *vb, c);
    }
    return c;
}

// u size must be >= v size
template <std::unsigned_integral LimbT>
inline LimbT usub_unchecked(LimbT const* ub, LimbT const* ue, LimbT last_u, LimbT const* vb, LimbT const* ve, LimbT last_v, LimbT*& rb)
{
    LimbT c = usub_partial_unchecked(ub, vb, ve, rb);
    if (ub != ue) {
        std::tie(c, *rb++) = sonia::arithmetic::usub1c(*ub++, last_v, c);
        for (; ub != ue; ++ub, ++rb) {
            std::tie(c, *rb) = sonia::arithmetic::usub1(*ub, c);
        }
        std::tie(c, *rb) = sonia::arithmetic::usub1(last_u, c);
    } else {
        std::tie(c, *rb) = sonia::arithmetic::usub1c(last_u, last_v, c);
    }
    return c;
}

template <std::unsigned_integral LimbT>
LimbT usub(std::span<const LimbT> u, std::span<const LimbT> v, std::span<LimbT> r) noexcept
{
    assert(r.size() >= (std::max)(u.size(), v.size()));
    LimbT const* ub = u.data(), * ue = ub + u.size();
    LimbT const* vb = v.data(), * ve = vb + v.size();
    LimbT* rb = r.data(); // , * re = rb + r.size();
    LimbT c = 0;
    for (;; ++ub, ++vb, ++rb) {
        if (ub != ue) {
            if (vb != ve) {
                std::tie(c , *rb) = sonia::arithmetic::usub1c(*ub, *vb, c);
                continue;
            } else if (c) {
                do {
                    std::tie(c, *rb) = sonia::arithmetic::usub1(*ub, c);
                    if (!c) break;
                    ++ub; ++rb;
                } while (ub != ue);
            }
        } else {
            for (; vb != ve; ++vb, ++rb) {
                std::tie(c, *rb) = sonia::arithmetic::usub1c(LimbT{0}, *vb, c);
            }
        }
        return c;
    }
}

// prereqs: ur.size() + 1 = v.size()
//template <std::unsigned_integral LimbT>
//LimbT usub(LimbT& uh, std::span<LimbT> ul, std::span<const LimbT> v) noexcept
//{
//    assert(ul.size() + 1 == v.size());
//    auto vlsp = v.subspan((std::max)(ul.size(), v.size()));
//    LimbT c = usub<LimbT>(ul, vlsp, ul);
//    if (vlsp.size() < v.size()) {
//        assert(vlsp.size() + 1 == v.size());
//        std::tie(c, uh) = sonia::arithmetic::usub1c(uh, v.back(), c);
//    } else {
//        std::tie(c, uh) = sonia::arithmetic::usub1(uh, c);
//    }
//    return c;
//}


//template <std::unsigned_integral LimbT>
//inline LimbT usub(LimbT & uhh, LimbT & uh, std::span<LimbT> ul, LimbT vh, std::span<const LimbT> vl) noexcept
//{
//    assert(ul.size() + 1 == vl.size());
//    LimbT c = usub<LimbT>(ul, vl, ul);
//    std::tie(c, uh) = sonia::arithmetic::usub1c(uh, vh, c);
//    std::tie(c, uhh) = sonia::arithmetic::usub1(uhh, c);
//    return c;
//}

// (c, [u]) <- [u] * v + cl; returns c
template <std::unsigned_integral LimbT>
inline LimbT umul1(std::span<LimbT> u, LimbT v, LimbT cl = 0) noexcept
{
    for (LimbT & e : u) {
        auto [h, l] = sonia::arithmetic::umul1(e, v);
        l += cl;
        cl = (l < cl) + h;
        e = l;
    }
    return cl;
}


// (c, p[u.size()]) <- [u] * v + cl; returns c
template <std::unsigned_integral LimbT>
inline LimbT umul1(std::span<const LimbT> u, LimbT v, LimbT* p, LimbT cl = 0) noexcept
{
    for (LimbT const& e : u) {
        auto [h, l] = sonia::arithmetic::umul1(e, v);
        l += cl;
        cl = (l < cl) + h;
        *p++ = l;
    }
    return cl;
}

// (c, p[u.size()]) <- [u] * v + p[u.size()]; returns c
template <std::unsigned_integral LimbT>
inline LimbT umul1_add(std::span<const LimbT> u, LimbT v, LimbT* p, LimbT cl = 0) noexcept
{
    for (LimbT const& e : u) {
        auto [h, l] = sonia::arithmetic::umul1(e, v);
        auto [ph, pl] = sonia::arithmetic::uadd1c(l, *p, cl);
        cl = h + ph;
        *p++ = pl;
    }
    return cl;
}

// (uh, [ul]) * v + cl -> (pcl, ph, p[ul.size()]); returns {pcl, ph}
template <std::unsigned_integral LimbT>
inline std::pair<LimbT, LimbT> umul1(LimbT uh, std::span<const LimbT> ul, LimbT v, LimbT* p, LimbT cl = 0) noexcept
{
    LimbT pcl = umul1<LimbT>(ul, v, p, cl);
    auto [h, ph] = sonia::arithmetic::umul1(uh, v);
    ph += pcl;
    pcl = (ph < pcl) + h;
    return { pcl, ph };
}

// (c, ph, p[ul.size()]) <- (uhh, uh, [ul]) * v + cl; returns (c, ph)
template <std::unsigned_integral LimbT>
inline std::tuple<LimbT, LimbT, LimbT> umul1(LimbT uhh, LimbT uh, std::span<const LimbT> ul, LimbT v, LimbT* p, LimbT cl = 0) noexcept
{
    LimbT pcl = umul1<LimbT>(ul, v, p, cl);
    auto [h, ph] = sonia::arithmetic::umul1<LimbT>(uh, v);
    ph += pcl;
    pcl = (ph < pcl) + h;
    auto [hh, phh] = sonia::arithmetic::umul1<LimbT>(uhh, v);
    phh += pcl;
    pcl = (phh < pcl) + hh;
    return { pcl, phh, ph };
}

// (c, p[size(ul) + 2]) <- (uhh, uh, [ul]) * v + p[size(ul)] + cl; returns c
template <std::unsigned_integral LimbT>
inline LimbT umul1_add(LimbT uhh, LimbT uh, std::span<const LimbT> ul, LimbT v, LimbT* p, LimbT cl = 0) noexcept
{
    LimbT pcl = umul1_sum<LimbT>(ul, v, p, cl);
    p += ul.size();
    auto [h, ph] = sonia::arithmetic::umul1<LimbT>(uh, v);
    auto [rc, rh] = sonia::arithmetic::uadd1c(ph, *p, pcl);
    *p++ = rh;
    auto [hh, phh] = sonia::arithmetic::umul1<LimbT>(uhh, v);
    auto [rch, rhh] = sonia::arithmetic::uadd1c(phh, *p, rc);
    *p++ = rh;
    return rch;
}

//template <std::unsigned_integral LimbT>
//inline void umul(std::span<const LimbT> u, std::span<const LimbT> v, std::span<LimbT> r) noexcept
//{
//    if (!u.size() || !v.size()) [[unlikely]] return;
//
//    LimbT const* ub = u.data(), * ue = ub + u.size();
//    LimbT const* vb = v.data(), * ve = vb + v.size();
//    LimbT* rb = r.data(), * re = rb + r.size();
//    assert(r.size() >= u.size() + v.size());
//
//    // first line
//    LimbT cl = 0;
//    do {
//        auto [h, l] = sonia::arithmetic::umul1(*ub, *vb);
//        l += cl;
//        cl = (l < cl) + h;
//        *rb = l;
//        ++ub; ++rb;
//    } while (ub != ue);
//    if (cl) { *rb++ = cl; }
//    while (rb != re) { *rb++ = 0; }
//    rb = r.data() + u.size();
//
//    // next lines
//    for (++vb; vb != ve; ++vb) {
//        cl = 0;
//        ub = u.data();
//        rb -= u.size() - 1;
//        do {
//            auto [h, l] = sonia::arithmetic::umul1(*ub, *vb);
//            auto [h1, l1] = sonia::arithmetic::uadd1c(l, *rb, cl);
//            cl = h + h1;
//            *rb = l1;
//            ++ub; ++rb;
//        } while (ub != ue);
//        if (cl) {
//            *rb = sonia::arithmetic::uadd1(*rb, cl).second;
//        }
//    }
//}

// base case mul: {u} * {v} -> {rb, re}
// returns re
template <std::unsigned_integral LimbT>
inline LimbT* umul(std::span<const LimbT> u, std::span<const LimbT> v, LimbT* r) noexcept
{
    if (u.empty() || v.empty()) [[unlikely]] return r;

    LimbT const* ub = u.data(), * ue = ub + u.size();
    LimbT const* vb = v.data(), * ve = vb + v.size();
    LimbT* rb = r;

    // first line
    LimbT cl = 0;
    do {
        auto [h, l] = sonia::arithmetic::umul1(*ub, *vb);
        l += cl;
        cl = (l < cl) + h;
        *rb = l;
        ++ub; ++rb;
    } while (ub != ue);
    *rb = cl;

    // next lines
    for (++vb; vb != ve; ++vb) {
        cl = 0;
        ub = u.data();
        rb -= u.size() - 1;
        do {
            auto [h, l] = sonia::arithmetic::umul1(*ub, *vb);
            auto [h1, l1] = sonia::arithmetic::uadd1c(l, *rb, cl);
            cl = h + h1;
            *rb = l1;
            ++ub; ++rb;
        } while (ub != ue);
        *rb = cl;
    }
    return rb + 1;
}

template <std::unsigned_integral LimbT>
inline void umul(std::span<const LimbT> u, std::span<const LimbT> v, std::span<LimbT> r) noexcept
{
    assert(r.size() >= u.size() + v.size());
    LimbT* re = umul(u, v, r.data());
    std::fill(re, r.data() + r.size(), 0);
}

// returns residual
template <std::unsigned_integral LimbT>
auto udivby1(std::span<LimbT> ls, LimbT d, LimbT invd, int l) -> LimbT
{
    using sonia::arithmetic::udiv2by1;

    int shift = std::numeric_limits<LimbT>::digits - l;

    LimbT ahigh = ls.back();
    LimbT* qp = &ls.back();

    LimbT r;

    if (!shift) {
        LimbT qhigh = (ahigh >= d);
        r = (qhigh ? ahigh - d : ahigh);

        *qp = qhigh;
        if (qp != ls.data()) {
            --qp;
            for (;;) {
                LimbT n0 = *qp;
                udiv2by1<LimbT>(*qp, r, r, n0, d, invd);
                if (qp == ls.data()) break;
                --qp;
            }
        }
    } else {
        r = 0;
        for (;;) {
            if (ahigh < d) {
                r = ahigh << shift;
                *qp = 0;
                if (qp == ls.data()) break;
                --qp;
            }
            
            LimbT n1 = *qp;
            r |= n1 >> l;
            LimbT dnorm = d << shift;
            while (qp != ls.data()) {
                --qp;
                LimbT n0 = *qp;
                udiv2by1<LimbT>(*(qp + 1), r, r, (n1 << shift) | (n0 >> l), dnorm, invd);
                n1 = n0;
            }
            udiv2by1<LimbT>(*qp, r, r, n1 << shift, dnorm, invd);
            break;
        }
    }
    return r >> shift;
}

// returns residual
template <std::unsigned_integral LimbT>
auto udivby1(std::span<const LimbT> ls, LimbT d, std::span<LimbT> r) -> LimbT
{
    assert(r.size() >= ls.size());
    std::copy(ls.begin(), ls.end(), r.begin());
    int shift = sonia::arithmetic::count_leading_zeros(d);
    int l = std::numeric_limits<LimbT>::digits - shift;
    LimbT u1 = (shift ? (LimbT{ 1 } << l) : 0) - d;
    auto [invd, dummy] = sonia::arithmetic::udiv2by1<LimbT>(u1, 0, d);

    return udivby1(r, d, invd, l);
}

template <std::unsigned_integral LimbT, LimbT d, bool ProcR = true>
auto udivby1(std::span<LimbT> ls) -> std::pair<LimbT, LimbT>
{
    using sonia::arithmetic::udiv2by1;
    using mpct::W;

    constexpr uint32_t limb_bit_count = std::numeric_limits<LimbT>::digits;
    constexpr int l = 1 + sonia::arithmetic::consteval_log2<LimbT, limb_bit_count>(d);
    constexpr auto d_c = W<d>;
    using m_ct = decltype(upow(W<2>, W<limb_bit_count>) * (upow(W<2>, W<l>) - d_c) / d_c);
    constexpr auto invd = mpct::front<m_ct>;
    constexpr uint32_t shift = limb_bit_count - l;

    LimbT ahigh = ls.back();
    LimbT* qp = &ls.back();

    LimbT r;
    LimbT dnorm = d << shift;

    if constexpr(!shift) {
        LimbT qhigh = (ahigh >= d);
        r = (qhigh ? ahigh - d : ahigh);

        *qp = qhigh;
        if (qp != ls.data()) {
            --qp;
            for (;;) {
                LimbT n0 = *qp;
                udiv2by1<LimbT>(*qp, r, r, n0, d, invd);
                if (qp == ls.data()) break;
                --qp;
            }
        }
    } else {
        r = 0;
        for (;;) {
            if (ahigh < d) {
                r = ahigh << shift;
                *qp = 0;
                if (qp == ls.data()) break;
                --qp;
            }

            LimbT n1 = *qp;
            r |= n1 >> l;
            
            while (qp != ls.data()) {
                --qp;
                LimbT n0 = *qp;
                udiv2by1<LimbT>(*(qp + 1), r, r, (n1 << shift) | (n0 >> l), dnorm, invd);
                n1 = n0;
            }
            udiv2by1<LimbT>(*qp, r, r, n1 << shift, dnorm, invd);
            break;
        }
    }

    if constexpr (ProcR) {
        LimbT frac;
        udiv2by1<LimbT>(frac, r, r, 0, dnorm, invd);
        return std::pair{ frac, r >> shift };
    } else {
        return r >> shift;
    }
}

template <std::unsigned_integral LimbT, typename QOutputIteratorT>
inline LimbT udivby1(LimbT& uh, std::span<LimbT> ul, LimbT d, QOutputIteratorT q)
{
    throw std::runtime_error("not implemented");
#if 0
    if (!uh) {
        ul.front() = udivby1((std::span<const LimbT>)ul, d, q);
        std::fill(ul.begin() + 1, ul.end(), 0);
        return uh;
    }
    throw std::runtime_error("not implemented");
#endif
}




// base case u / d
// prereqs: u < d * B^m, d normilized, where m = size(u) - size(d) in limbs, B = 2^bitsize(limb)
// returns {rhh, rh}; [rl] -> u
template <std::unsigned_integral LimbT, typename QOutputIteratorT, typename AllocatorT>
std::pair<LimbT, LimbT> udiv_bc_unorm(LimbT* puhh, LimbT* puh, std::span<LimbT>& ul, LimbT dh, std::span<const LimbT> dl, QOutputIteratorT qit, AllocatorT& alloc)
{
    assert(*puhh <= dh);

    size_t m = ul.size() - dl.size() + 1;
    
    if (m) {
        small_array<LimbT, SONIA_MP_INPLACE_LIMB_RESERVE_COUNT, AllocatorT> daux{ dl.size(), alloc };

#if defined(SONIA_ARITHMETIC_USE_INVINT_DIV)
        auto [dinv, _] = sonia::arithmetic::udiv2by1<LimbT>(~dh + 1, 0, dh);
#endif
        do {
            LimbT dummy;
            LimbT qj;
            for (;;)
            {
                if (*puhh < dh) {
#if defined(SONIA_ARITHMETIC_USE_INVINT_DIV)
                    sonia::arithmetic::udiv2by1<LimbT>(qj, dummy, *puhh, *puh, dh, dinv);
#else
                    std::tie(qj, dummy) = sonia::arithmetic::udiv2by1norm<LimbT>(*puhh, *puh, dh);
#endif
                    if (!qj) break;
                } else {
                    qj = (std::numeric_limits<LimbT>::max)();
                }
            
                auto [mdhh, mdh] = umul1<LimbT>(dh, dl, qj, daux.data());
                
                // u - dh * B^j
                auto usp = ul.last(dl.size());
                LimbT uc = usub<LimbT>(usp, daux, usp);
                std::tie(uc, *puh) = sonia::arithmetic::usub1c(*puh, mdh, uc);
                std::tie(uc, *puhh) = sonia::arithmetic::usub1c(*puhh, mdhh, uc);

                if (uc) {
                    do {
                        --qj;
                        uc = uadd<LimbT>(usp, dl, usp);
                        std::tie(uc, *puh) = sonia::arithmetic::uadd1c(*puh, dh, uc);
                        std::tie(uc, *puhh) = sonia::arithmetic::uadd1(*puhh, uc);
                    } while (!uc);
                }
                break;
            }
            assert(!*puhh);
            puhh = puh;
            puh = &ul.back();
            ul = ul.first(ul.size() - 1);
            *qit = qj; --qit;
        } while (--m);
    }
    return { *puhh, *puh };
}

template <std::unsigned_integral LimbT>
LimbT do_udiv_unorm(LimbT* puhh, LimbT* puh, std::span<LimbT>& ul, LimbT dh, std::span<const LimbT> dl)
{
    LimbT uc, tmphh, tmph;
    std::tie(uc, tmphh) = sonia::arithmetic::usub1(*puhh, dh);
    if (!uc) { // dh <= *puhh
        if (dl.empty()) {
            *puhh = tmphh;
        } else {
            //if (dl.size() == 1) {
            //    std::tie(uc, tmph) = sonia::arithmetic::usub1(*puh, dl.front());
            //    std::tie(uc, tmphh) = sonia::arithmetic::usub1(tmphh, uc);
            //    if (!uc) {
            //        *puhh = tmphh;
            //        *puh = tmph;
            //    }
            //}

            auto dl1 = dl.first(dl.size() - 1);
            auto ul1 = ul.last(dl1.size());

            uc = usub<LimbT>(ul1, dl1, ul1);
            std::tie(uc, tmph) = sonia::arithmetic::usub1c(*puh, dl.back(), uc);
            std::tie(uc, tmphh) = sonia::arithmetic::usub1(tmphh, uc);
            if (!uc) {
                *puhh = tmphh;
                *puh = tmph;
            } else {
                uadd<LimbT>(ul1, dl1, ul1);
            }
        }
    }
    return 1 - uc;
}

// base case u / d
// prereqs: u < d * B^m, d normilized, where m = size(u) - size(d) in limbs, B = 2^bitsize(limb)
// returns {rhh, rh}; [rl] -> u
template <std::unsigned_integral LimbT, typename QOutputIteratorT, typename AllocatorT>
std::pair<LimbT, LimbT> udiv_bc(LimbT* puhh, LimbT* puh, std::span<LimbT>& ul, LimbT dh, std::span<const LimbT> dl, QOutputIteratorT qit, AllocatorT& alloc)
{
    *qit-- = do_udiv_unorm(puhh, puh, ul, dh, dl);
    return udiv_bc_unorm<LimbT>(puhh, puh, ul, dh, dl, std::move(qit), alloc);
}

//
template <std::unsigned_integral LimbT, typename QOutputIteratorT, typename AllocatorT>
std::pair<LimbT, LimbT> udiv_svoboda(LimbT* puhh, LimbT* puh, std::span<LimbT>& ul, LimbT dh, std::span<const LimbT> dl, QOutputIteratorT qit, AllocatorT& alloc)
{
    //using allocator_type = std::remove_cvref_t<AllocatorT>;
    //using alloc_traits_t = std::allocator_traits<allocator_type>;

    *qit-- = do_udiv_unorm(puhh, puh, ul, dh, dl);

    size_t m = ul.size() + 1 - dl.size();
    if (!m) return { *puhh, *puh };

    small_array<LimbT, SONIA_MP_INPLACE_LIMB_RESERVE_COUNT, AllocatorT> auxbuff(dl.size() + 1 /*dh*/ + 2, alloc);
    std::fill(auxbuff.begin(), auxbuff.end() - 1, 0);
    auxbuff.back() = 1;
    
    LimbT k[3];
    auto tmpsp = auxbuff.span().first(dl.size() + 1);
    udiv_bc(&auxbuff.back(), &auxbuff.back() - 1, tmpsp, dh, dl, k + 2, alloc);
    // ceiling: if reminder != 0 => k = k+1
    for (LimbT const* pr = tmpsp.data() + dl.size();;) {
        if (*pr) {
            LimbT uc;
            std::tie(uc, k[0]) = sonia::arithmetic::uadd1<LimbT>(k[0], 1);
            if (uc) ++k[1];
            break;
        }
        if (pr == tmpsp.data()) break;
        --pr;
    }

    //d1 = k*d, reuse auxbuff for d1
    auto d1 = auxbuff.span();
    
    //(dh*B^(dl.size()) + dl) * k = dh * k + dl * k
    LimbT kdh[3];
    umul<LimbT>(dl, { k, 2 }, d1); // dl * k
    kdh[2] = umul1<LimbT>({ k, 2 }, dh, kdh);
    uadd<LimbT>({ d1.data() + dl.size(), d1.size() - dl.size() }, { kdh, 3 });
    assert(d1[dl.size() + 1] == 0);
    assert(d1[dl.size() + 2] == 1);
    auto d1l = std::span{ d1.data(), d1.size() - 2 };
    //LimbT d1h = d1.back();

    size_t m1 = m - 1;
    
    small_array<LimbT, SONIA_MP_INPLACE_LIMB_RESERVE_COUNT, AllocatorT> q1{ m1, alloc };
    if (m1) {
        LimbT* pq1 = &q1.back();
        auto q1sp = q1.span();

        // daux stores qj * d1
        small_array<LimbT, SONIA_MP_INPLACE_LIMB_RESERVE_COUNT, AllocatorT> daux{ d1.size(), alloc };
        LimbT& dauxhh = daux.back();
        LimbT& dauxh = *(&dauxhh - 1);
        auto dauxl = daux.first(d1.size() - 2);

        for (;;) {
            // q(j) = a(n+j)
            LimbT qj = *puhh;
        
            // A <- A - qj*d1*B^(j-1)
            LimbT uc = umul1<LimbT>(d1, qj, dauxl.data());
            assert(!uc);
        
            auto usp = ul.last(dauxl.size());
        
            uc = usub<LimbT>(usp, dauxl, usp);
            std::tie(uc, *puh) = sonia::arithmetic::usub1c(*puh, dauxh, uc);
            std::tie(uc, *puhh) = sonia::arithmetic::usub1c(*puhh, dauxhh, uc);
            if (*puhh) { // <=> if uc != 0 then gj <- gj - 1, A <- A + d1*B^(j-1)
                --qj;
                uc = uadd<LimbT>(usp, d1l, usp);
                std::tie(uc, *puh) = sonia::arithmetic::uadd1<LimbT>(*puh, uc); // d1h = 0
                //std::tie(uc, *puhh) = sonia::arithmetic::uadd1c<LimbT>(*puhh, 1, uc); // d1hh = 1
                //assert(uc);
                *puhh = 0;
            }
            puhh = puh;
            puh = &ul.back();
            ul = ul.first(ul.size() - 1);
            *pq1 = qj;
            if (pq1 == q1.data()) break;
            --pq1;
        }
    }
    // q0 = r1 div d
    LimbT q0arr[2];
    auto qr = udiv_bc<LimbT>(puhh, puh, ul, dh, dl, q0arr + 1, alloc);
    
    // q = k * q1, we need aux buffer for q to reorder q limbs
    small_array<LimbT, SONIA_MP_INPLACE_LIMB_RESERVE_COUNT, AllocatorT> q(m1 + 2, alloc);
    LimbT* qb = q.data();
    LimbT *qe = umul<LimbT>(q1, { k, 2 }, qb) - 1;
    auto [q0, qbval] = sonia::arithmetic::uadd1(*qb, q0arr[0]);
    *qb = qbval; q0 += q0arr[1];
    if (q0) {
        LimbT* qit = qb;
        do {
            ++qit;
            std::tie(q0, *qit) = sonia::arithmetic::uadd1(*qit, q0);
        } while (q0);
    }

    while (qe != qb) {
        *qit-- = *--qe;
    }
    return qr;
}


// divide and conquer
// prereqs: u >= d, d normilized
template <std::unsigned_integral LimbT, typename QOutputIteratorT, typename AllocatorT>
inline void udiv_dv(LimbT* puhh, LimbT* puh, std::span<LimbT>& ul, std::span<LimbT> d, QOutputIteratorT qit, AllocatorT& alloc)
{
    // B (base) = 2^std::numetic_limits<LimbT>::digits;

    using allocator_type = std::remove_cvref_t<AllocatorT>;
    using alloc_traits_t = std::allocator_traits<allocator_type>;

    size_t m = ul.size() - d.size() + 2;
    if (m < DC_DIV_QR_THRESHOLD || d.size() < 2)
        return udiv_bc(puh, puhh, ul, d, std::move(qit));

    size_t k = m / 2;
    size_t thr = 2 * k;;
    assert(ul.size() >= thr);
    auto ul1 = ul.subspan(thr);
    auto d1 = d.subspan(k);
    auto d0 = d.first(k);

    size_t q1sz = 2 + ul1.size() - d1.size();
    LimbT* q1 = alloc_traits_t::allocate(alloc, q1sz);
    SCOPE_EXIT([&alloc, q1, q1sz] { alloc_traits_t::deallocate(alloc, q1, q1sz); });

    LimbT r1h = udiv_dv<LimbT>(puhh, puh, ul1, d1, q1 + q1sz - 1, alloc);
    size_t realq1sz = q1sz;
    if (!q1[q1sz - 1]) --realq1sz;
    // here u1 = r1
    ul1 = ul.first(thr + ul1.size());
    // now u1 = r1*B^2k + (u mod B^2k)
    
    // u1 - q1 * d0 * B^k
    size_t q1d0sz = realq1sz * d0.size();
    LimbT* q1d0 = alloc_traits_t::allocate(alloc, q1d0sz);
    SCOPE_EXIT([&alloc, q1d0, q1d0sz] { alloc_traits_t::deallocate(alloc, q1d0, q1d0sz); });
    umul<LimbT>({ q1, realq1sz }, d0, { q1d0, q1d0sz });
    
    auto ul2 = ul1.subspan(k); // ul2 = u1 div B^k
    LimbT c = usub<LimbT>(r1h, ul2, { q1d0, q1d0sz });
}

template <std::unsigned_integral LimbT, typename QOutputIteratorT, typename AllocatorT>
LimbT udiv(LimbT uh, std::span<LimbT>& ul, LimbT dh, std::span<const LimbT> dl, QOutputIteratorT qit, AllocatorT && alloc)
{
    //using allocator_type = std::remove_cvref_t<AllocatorT>;
    //using alloc_traits_t = std::allocator_traits<allocator_type>;

    LimbT* puh;
    if (!uh) {
        puh = &ul.back();
        ul = ul.first(ul.size() - 1);
    } else {
        puh = &uh;
    }

    // normalization
    small_array<LimbT, SONIA_MP_INPLACE_LIMB_RESERVE_COUNT, AllocatorT> optdnorm(0, alloc);
    const LimbT* dlnorm;
    LimbT uhhstore = 0;
    int shift = sonia::arithmetic::count_leading_zeros(dh);
    if (shift) {
        optdnorm.reset(dl.size());
        ushift_left<LimbT>(dh, dl, shift, optdnorm.data()); // returns 0
        uhhstore = ushift_left<LimbT>(*puh, ul, shift, ul.data());
        dlnorm = optdnorm.data();
    } else {
        dlnorm = dl.data();
    }
    
    LimbT* puhh = &uhhstore;
    if (!uhhstore) {
        puhh = puh;
        puh = &ul.back();
        ul = ul.first(ul.size() - 1);
    }
    assert(*puhh);
    //auto [rhh, rh] = udiv_dv(puhh, puh, ul, dh, { dlnorm, dl.size() }, std::move(qit), alloc);
    auto [rhh, rh] = udiv_svoboda(puhh, puh, ul, dh, { dlnorm, dl.size() }, std::move(qit), alloc);
    //auto [rhh, rh] = udiv_bc(puhh, puh, ul, dh, { dlnorm, dl.size() }, std::move(qit), alloc);
    if (shift) {
        ushift_right<LimbT>(rh, ul, shift); // returns 0
        if (rhh) {
            rh |= (rhh << (std::numeric_limits<LimbT>::digits - shift));
        }
    } else if (rhh) {
        ul = { ul.data(), ul.size() + 1 };
        assert(ul.back() == rh);
        rh = rhh;
    }
    
    return rh;
}



// prereqs: u >= d, d.back() > 0
// {uh, ul} / d -> q(from high to low); rl -> ul, returns rh
// uh can be 0, daux.size() >= d.size()
template <std::unsigned_integral LimbT, typename QOutputIteratorT>
LimbT udiv2(LimbT& uh, std::span<LimbT> & ul, std::span<LimbT> d, std::span<LimbT> daux, QOutputIteratorT qit)
{
    assert(d.back());
    assert(daux.size() >= d.size());

    if (d.size() == 1) {
        return udivby1(uh, ul, d.back(), qit);
    }

    LimbT* puh;
    if (!uh) {
        puh = &ul.back();
        ul = ul.first(ul.size() - 1);
    } else {
        puh = &uh;
    }

    // normalization
    LimbT uhhstore = 0;
    int shift = sonia::arithmetic::count_leading_zeros(d.back());
    if (shift) {
        uhhstore = ushift_left<LimbT>(*puh, ul, shift, ul.data());
        ushift_left<LimbT>(d, shift); // returns 0
    }
    LimbT* puhh = &uhhstore;
    if (!uhhstore) {
        puhh = puh;
        puh = &ul.back();
        ul = ul.first(ul.size() - 1);
    }
    assert(*puhh);

    // ul.size() + 2 = n + m, n = d.size(); => m = ul.size() + 2 - d.size() = ul.size() - d.size() + 2
    // compare U and d * B^m
    size_t m = ul.size() - d.size() + 2;
    
    LimbT const* db = d.data(), * de = db + d.size() - 1;
    LimbT const* ub = ul.data(), * ue = ub + ul.size() - 1;
    bool u_gt_d = *puhh > *de;
    if (!u_gt_d && *puhh == *de) {
        --de;
        u_gt_d = *puh > *de;
        if (!u_gt_d && *puh == *de) {
            u_gt_d = true;
            while (db != de) {
                --de;
                if (*ue != *de) { u_gt_d = *ue > *de; break; }
                --ue;
            }
        }
    }

    if (u_gt_d) {
        //  u <- u - B^m, q(m) <- 1
        auto dsp = d.first(d.size() - 2);
        auto usp = ul.last(dsp.size());
        LimbT uc;
        if (!dsp.empty()) {
            uc = usub<LimbT>(usp, dsp, usp);
            std::tie(uc, *puh) = sonia::arithmetic::usub1c(*puh, d[d.size() - 2], uc);
        } else {
            std::tie(uc, *puh) = sonia::arithmetic::usub1(*puh, d[d.size() - 2]);
        }
        std::tie(uc, *puhh) = sonia::arithmetic::usub1c(*puhh, d.back(), uc);
        *qit = 1;
    } else {
        *qit = 0;
    }
    --qit;

    if (m) {
        auto dsp = d.first(d.size() - 1);
        auto dauxsp = daux.first(daux.size() - 1);

#if defined(SONIA_ARITHMETIC_USE_INVINT_DIV)
        auto [dinv, _] = sonia::arithmetic::udiv2by1<LimbT>(~d.back() + 1, 0, d.back());
#endif
        do {
            LimbT dummy;
            LimbT qj;
            for (;;)
            {
                if (*puhh < d.back()) {
#if defined(SONIA_ARITHMETIC_USE_INVINT_DIV)
                    sonia::arithmetic::udiv2by1<LimbT>(qj, dummy, *puhh, *puh, d.back(), dinv);
#else
                    std::tie(qj, dummy) = sonia::arithmetic::udiv2by1norm<LimbT>(*puhh, *puh, d.back());
#endif
                    if (!qj) break;
                } else {
                    qj = (std::numeric_limits<LimbT>::max)();
                }
            
                LimbT dh = umul1<LimbT>(d, qj, daux.data());
            
                // u - dh * B^j
                auto usp = ul.last(dauxsp.size());
                LimbT uc = usub<LimbT>(usp, dauxsp, usp);
                std::tie(uc, *puh) = sonia::arithmetic::usub1c(*puh, daux.back(), uc);
                std::tie(uc, *puhh) = sonia::arithmetic::usub1c(*puhh, dh, uc);

                if (uc) {
                    do {
                        --qj;
                        uc = uadd<LimbT>(usp, dsp, usp);
                        std::tie(uc, *puh) = sonia::arithmetic::uadd1c(*puh, d.back(), uc);
                        std::tie(uc, *puhh) = sonia::arithmetic::uadd1(*puhh, uc);
                    } while (!uc);
                }
                break;
            }
            assert(!*puhh);
            puhh = puh;
            puh = &ul.back();
            ul = ul.first(ul.size() - 1);
            *qit = qj; --qit;
        } while (--m);
    }
    if (*puhh) {
        puh = puhh;
        ul = { ul.data(), ul.size() + 1 };
    }
    if (shift) {
        ushift_right<LimbT>(*puh, ul, shift, ul.data());
    }
    return *puh;
}

template <std::unsigned_integral LimbT, typename QOutputIteratorT, typename AllocatorT>
LimbT udiv2(LimbT& uh, std::span<LimbT>& ul, LimbT dh, std::span<const LimbT> dl, QOutputIteratorT qit, AllocatorT&& alloc)
{
    using allocator_type = std::remove_cvref_t<AllocatorT>;
    using alloc_traits_t = std::allocator_traits<allocator_type>;

    std::span<LimbT> d{ alloc_traits_t::allocate(alloc, dl.size() + 1), dl.size() + 1 };
    std::span<LimbT> daux{ alloc_traits_t::allocate(alloc, dl.size() + 1), dl.size() + 1 };
    std::copy(dl.begin(), dl.end(), d.data()); d.back() = dh;
    SCOPE_EXIT([&alloc, d, daux] {
        alloc_traits_t::deallocate(alloc, d.data(), d.size());
        alloc_traits_t::deallocate(alloc, daux.data(), daux.size());
        });

    return udiv2<LimbT>(uh, ul, d, daux, std::move(qit));
}

template <std::unsigned_integral LimbT>
inline void udiv(std::span<const LimbT> u, std::span<const LimbT> v, std::span<LimbT> q, std::span<LimbT> r)
{
    LimbT const* vb = v.data(), * ve = vb + v.size();
    for (;; --ve) {
        if (vb == ve) [[unlikely]] {
            throw std::runtime_error("division by zero");
        }
        if (*(ve - 1)) break;
    }
    if (1 == ve - vb) {
        r.front() = udivby1(u, *vb, q);
        std::fill(r.begin() + 1, r.end(), 0);
        return;
    }
    throw std::runtime_error("not implemented");
}

template <std::unsigned_integral LimbT>
inline void ushift_left(std::span<const LimbT> u, size_t shift, std::span<LimbT> r)
{
    assert(r.size() > u.size());
    if (u.empty()) [[unlikely]] {
        return;
    }

    LimbT const* ub = u.data(), * ue = ub + u.size();
    r.front() = (*ub) << shift;
    
    LimbT * rb = r.data(), * re = rb + u.size();
    for (LimbT const* nub = ub + 1;;++ub, ++nub) {
        ++rb;
        if (nub == ue) [[unlikely]] {
            *rb = *ub >> (sizeof(LimbT) * CHAR_BIT - shift);
            return;
        }
        *rb = (*nub << shift) | (*ub >> (sizeof(LimbT) * CHAR_BIT - shift));
    }
}



template <std::unsigned_integral LimbT>
auto sqrt_rem(std::span<const LimbT> m, std::span<LimbT> s) noexcept
{
    size_t l = (m.size() - 1) / 4;
    if (!l) {
        s[0] = sonia::arithmetic::sqrt<LimbT>(m[0]);
    }
}

}
