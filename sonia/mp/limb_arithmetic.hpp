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

template <std::unsigned_integral LimbT>
inline LimbT uadd(std::span<const LimbT> u, std::span<const LimbT> v, std::span<LimbT> r)
{
    assert(r.size() >= (std::max)(u.size(), v.size()));
    LimbT const* ub = u.data(), * ue = ub + u.size();
    LimbT const* vb = v.data(), * ve = vb + v.size();
    LimbT* rb = r.data(), * re = rb + r.size();
    LimbT c = 0;
    for (;; ++ub, ++vb, ++rb) {
        if (ub != ue) {
            if (vb != ve) {
                std::tie(c, *rb) = sonia::arithmetic::uadd1c(*ub, *vb, c);
                continue;
            } else {
                do {
                    std::tie(c, *rb) = sonia::arithmetic::uadd1(*ub, c);
                    ++ub; ++rb;
                } while (ub != ue);
            }
        } else {
            for (; vb != ve; ++vb, ++rb) {
                std::tie(c, *rb) = sonia::arithmetic::uadd1(*vb, c);
            }
        }
        return c;
    }
}

template <std::unsigned_integral LimbT>
inline LimbT usub(std::span<const LimbT> u, std::span<const LimbT> v, std::span<LimbT> r)
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

    // next lines
    for (++vb; vb != ve; ++vb) {
        cl = 0;
        ub = u.data();
        rb -= u.size();
        do {
            auto [h, l] = sonia::arithmetic::umul1(*ub, *vb);
            auto [h1, l1] = sonia::arithmetic::uadd1c(l, *rb, cl);
            cl = h + h1;
            *rb = l1;
            ++ub; ++rb;
        } while (ub != ue);
        if (cl) {
            *rb++ = sonia::arithmetic::uadd1(*rb, cl).second;
        }
    }
}

// returns residual
template <std::unsigned_integral LimbT>
auto udivby1(std::span<LimbT> ls, LimbT d, LimbT invd, int l) -> LimbT
{
    using sonia::arithmetic::udiv2by1;

    int shift = sizeof(LimbT) * CHAR_BIT - l;

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
    int l = sizeof(LimbT) * CHAR_BIT - shift;
    LimbT u1 = (shift ? (LimbT{ 1 } << l) : 0) - d;
    auto [invd, dummy] = sonia::arithmetic::udiv2by1<LimbT>(u1, 0, d);

    return udivby1(r, d, invd, l);
}

template <std::unsigned_integral LimbT, LimbT d, bool ProcR = true>
auto udivby1(std::span<LimbT> ls) -> std::pair<LimbT, LimbT>
{
    using sonia::arithmetic::udiv2by1;
    using mpct::W;

    constexpr uint32_t limb_bit_count = sizeof(LimbT) * CHAR_BIT;
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

    *re = (*ue) >> shift;
    for (;;) {
        if (ub == ue) [[unlikely]] return;
        LimbT const* nue = ue - 1;
        --re;
        *re = (*nue >> shift) | (*ue << (sizeof(LimbT) * CHAR_BIT - shift));
        --ue;
    }
}

}
