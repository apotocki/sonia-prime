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

namespace sonia::mp::arithmetic {

template <std::unsigned_integral LimbT>
inline void uor(std::span<const LimbT> u, std::span<const LimbT> v, std::span<LimbT> r)
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

// u size must be >= v size
template <std::unsigned_integral LimbT>
inline LimbT uadd_unchecked(LimbT const* ub, LimbT const* ue, LimbT const* vb, LimbT const* ve, LimbT* rb)
{
    LimbT c = uadd_partial_unchecked(ub, vb, ve, rb);
    for (; ub != ue; ++ub, ++rb) {
        std::tie(c, *rb) = sonia::arithmetic::uadd1(*ub, c);
    }
    return c;
}

// u size must be >= v size
template <std::unsigned_integral LimbT>
inline LimbT uadd_unchecked(LimbT const* ub, LimbT const* ue, LimbT last_u, LimbT const* vb, LimbT const* ve, LimbT last_v, LimbT*& rb)
{
    LimbT c = uadd_partial_unchecked(ub, vb, ve, rb);
    if (ub != ue) {
        std::tie(c, *rb++) = sonia::arithmetic::uadd1c(*ub++, last_v, c);
        for (; ub != ue; ++ub, ++rb) {
            std::tie(c, *rb) = sonia::arithmetic::uadd1(*ub, c);
        }
        std::tie(c, *rb++) = sonia::arithmetic::uadd1(last_u, c);
    } else {
        std::tie(c, *rb++) = sonia::arithmetic::uadd1c(last_u, last_v, c);
    }
    return c;
}

template <std::unsigned_integral LimbT>
inline LimbT uadd(std::span<const LimbT> u, std::span<const LimbT> v, std::span<LimbT> r)
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
    LimbT* rb = r.data(), * re = rb + r.size();
    LimbT c = 0;
    for (;; ++ub, ++vb, ++rb) {
        if (ub != ue) {
            if (vb != ve) {
                std::tie(c , *rb) = sonia::arithmetic::usub1c(*ub, *vb, c);
                continue;
            } else {
                do {
                    std::tie(c, *rb) = sonia::arithmetic::usub1(*ub, c);
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

//template <std::unsigned_integral LimbT>
//inline LimbT usub(LimbT & uhh, LimbT & uh, std::span<LimbT> ul, LimbT vh, std::span<const LimbT> vl) noexcept
//{
//    assert(ul.size() + 1 == vl.size());
//    LimbT c = usub<LimbT>(ul, vl, ul);
//    std::tie(c, uh) = sonia::arithmetic::usub1c(uh, vh, c);
//    std::tie(c, uhh) = sonia::arithmetic::usub1(uhh, c);
//    return c;
//}

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

// u * v + cl -> p; returns pcl
template <std::unsigned_integral LimbT>
inline LimbT umul1(std::span<const LimbT> u, LimbT v, std::span<LimbT> p, LimbT cl = 0) noexcept
{
    assert(p.size() >= u.size());
    LimbT * pb = p.data();
    for (LimbT const& e : u) {
        auto [h, l] = sonia::arithmetic::umul1(e, v);
        l += cl;
        cl = (l < cl) + h;
        *pb++ = l;
    }
    return cl;
}

template <std::unsigned_integral LimbT>
inline void umul(std::span<const LimbT> u, std::span<const LimbT> v, std::span<LimbT> r) noexcept
{
    if (!u.size() || !v.size()) [[unlikely]] return;

    LimbT const* ub = u.data(), * ue = ub + u.size();
    LimbT const* vb = v.data(), * ve = vb + v.size();
    LimbT* rb = r.data(), * re = rb + r.size();
    assert(r.size() >= u.size() + v.size());

    // first line
    LimbT cl = 0;
    do {
        auto [h, l] = sonia::arithmetic::umul1(*ub, *vb);
        l += cl;
        cl = (l < cl) + h;
        *rb = l;
        ++ub; ++rb;
    } while (ub != ue);
    if (cl) { *rb++ = cl; }
    while (rb != re) { *rb++ = 0; }
    rb = r.data() + u.size();

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
        if (cl) {
            *rb = sonia::arithmetic::uadd1(*rb, cl).second;
        }
    }
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

template <std::unsigned_integral LimbT>
inline LimbT udivby1(std::span<LimbT> ul, LimbT * uh, LimbT dh, std::span<LimbT> q)
{
    if (!*uh) {
        ul.front() = udivby1((std::span<const LimbT>)ul, dh, q);
        std::fill(ul.begin() + 1, ul.end(), 0);
        return *uh;
    }
    throw std::runtime_error("not implemented");
}

// r = U * 2 ^ shift
template <std::unsigned_integral LimbT>
LimbT ushift_left(std::span<const LimbT> ul, LimbT & uh, unsigned int shift, std::span<LimbT> rl) noexcept
{
    assert(shift < std::numeric_limits<LimbT>::digits);
    assert(rl.size() == ul.size());
    size_t rshift = std::numeric_limits<LimbT>::digits - shift;
    LimbT uhh = uh >> rshift;
    uh <<= shift;
    if (!ul.empty()) {
        uh |= ul.back() >> rshift;
        LimbT const* ub = ul.data(), * ue = ub + ul.size() - 1;
        LimbT* re = rl.data() + ul.size() - 1;
        for (;;--re) {
            *re = (*ue) << shift;
            if (ub == ue) [[unlikely]] break;
            --ue;
            (*re) |= (*ue) >> rshift;
        }
    }
    return uhh;
}

template <std::unsigned_integral LimbT>
inline LimbT ushift_left(std::span<LimbT> ul, unsigned int shift) noexcept
{
    auto sp = ul.subspan(0, ul.size() - 1);
    return ushift_left<LimbT>(ul.subspan(0, ul.size() - 1), ul.back(), shift, sp);
}

// r = [U / 2 ^ shift]
template <std::unsigned_integral LimbT>
LimbT ushift_right(std::span<const LimbT> ul, LimbT& uh, unsigned int shift, std::span<LimbT> rl)
{
    assert(shift < std::numeric_limits<LimbT>::digits);
    assert(rl.size() == ul.size());
    size_t lshift = std::numeric_limits<LimbT>::digits - shift;
    size_t lmask = ~((LimbT(1) << shift) - 1);
    LimbT rll = uh << lshift;
    uh >>= shift;
    if (!ul.empty()) {
        LimbT const* ub = ul.data(), * ue = ub + ul.size() - 1;
        LimbT* re = rl.data() + ul.size() - 1;
        for (;; --re, --ue) {
            LimbT tmp = rll | ((*ue) >> shift);
            rll = (*ue) << lshift;
            *re = tmp;
            if (ub == ue) [[unlikely]] break;
        }
    }
    return rll;
}

// prereqs: u >= d, d.back() > 0
// {uh, ul} / d -> q; rl -> ul, returns rh
// uh can be 0, daux.size() >= d.size()
template <std::unsigned_integral LimbT>
inline LimbT udiv(std::span<LimbT> & ul, LimbT * puh, std::span<LimbT> d, std::span<LimbT> daux, std::span<LimbT>& q)
{
    assert(d.back());
    assert(daux.size() >= d.size());
   // constexpr uint32_t limb_bit_count = std::numeric_limits<LimbT>::digits;

    if (d.size() == 1) {
        return udivby1(ul, puh, d.back(), q);
    }
    
    if (!*puh) {
        puh = &ul.back();
        ul = ul.subspan(0, ul.size() - 1);
    }

    // normalization
    LimbT uhhstore = 0;
    int shift = sonia::arithmetic::count_leading_zeros(d.back());
    if (shift) {
        uhhstore = ushift_left<LimbT>(ul, *puh, shift, ul);
        ushift_left<LimbT>(d, shift); // returns 0
    }
    LimbT* puhh = &uhhstore;
    if (!uhhstore) {
        puhh = puh;
        puh = &ul.back();
        ul = ul.subspan(0, ul.size() - 1);
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

    q = q.subspan(0, m + (u_gt_d ? 1 : 0));
    LimbT* pqb = q.data(), * pqj = pqb + q.size();

    if (u_gt_d) {
        //  u <- u - B^m, q(m) <- 1
        auto dsp = d.subspan(0, d.size() - 2);
        auto usp = ul.subspan(ul.size() - dsp.size(), dsp.size());
        LimbT uc;
        if (!dsp.empty()) {
            uc = usub<LimbT>(usp, dsp, usp);
            std::tie(uc, *puh) = sonia::arithmetic::usub1c(*puh, d[d.size() - 2], uc);
        } else {
            std::tie(uc, *puh) = sonia::arithmetic::usub1(*puh, d[d.size() - 2]);
        }
        std::tie(uc, *puhh) = sonia::arithmetic::usub1c(*puhh, d.back(), uc);
        *--pqj = 1;
    }
    
    if (pqb != pqj) {
        auto dsp = d.subspan(0, d.size() - 1);
        auto dauxsp = daux.subspan(0, daux.size() - 1);

#if defined(SONIA_ARITHMETIC_USE_INVINT_DIV)
        auto [dinv, _] = sonia::arithmetic::udiv2by1<LimbT>(~d.back() + 1, 0, d.back());
#endif
        do {
            LimbT dummy;
            --pqj;
        
            for (;;)
            {
                if (*puhh < d.back()) {
#if defined(SONIA_ARITHMETIC_USE_INVINT_DIV)
                    sonia::arithmetic::udiv2by1<LimbT>(*pqj, dummy, *puhh, *puh, d.back(), dinv);
#else
                    std::tie(*pqj, dummy) = sonia::arithmetic::udiv2by1norm<LimbT>(*puhh, *puh, d.back());
#endif
                    
                    if (!*pqj) break;
                } else {
                    *pqj = (std::numeric_limits<LimbT>::max)();
                }
            
                LimbT dh = umul1<LimbT>(d, *pqj, daux);
            
                // u - dh * B^j
                auto usp = ul.subspan(ul.size() - dauxsp.size(), dauxsp.size());
                LimbT uc = usub<LimbT>(usp, dauxsp, usp);
                std::tie(uc, *puh) = sonia::arithmetic::usub1c(*puh, daux.back(), uc);
                std::tie(uc, *puhh) = sonia::arithmetic::usub1c(*puhh, dh, uc);

                if (uc) {
                    do {
                        --*pqj;
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
            ul = ul.subspan(0, ul.size() - 1);
        } while (pqb != pqj);
    }
    if (*puhh) {
        puh = puhh;
        ul = { ul.data(), ul.size() + 1 };
    }
    if (shift) {
        ushift_right<LimbT>(ul, *puh, shift, ul);
    }
    return *puh;
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
inline void ushift_right(std::span<const LimbT> u, size_t shift, std::span<LimbT> r)
{
    assert(r.size() >= u.size());
    if (u.empty()) [[unlikely]] {
        return;
    }
    LimbT const* ub = u.data(), * ue = ub + u.size() - 1;
    LimbT* rb = r.data(), * re = rb + u.size() - 1;

    size_t lshift = std::numeric_limits<LimbT>::digits - shift;

    *re = (*ue) >> shift;
    for (;;) {
        if (ub == ue) [[unlikely]] return;
        LimbT const* nue = ue - 1;
        --re;
        *re = (*nue >> shift) | (*ue << lshift);
        --ue;
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
