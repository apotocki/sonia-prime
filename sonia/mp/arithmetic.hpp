/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <type_traits>
#include <concepts>
#include <limits>
#include <utility>
#include <cassert>

namespace sonia {

template <int Bits> struct uint_t
{
    using least = void;
    using fast = void;
};

template <> struct uint_t<8>
{
    using least = uint_least8_t;
    using fast = uint_fast8_t;
    /*
    static constexpr least max_dec_base = 100;
    static constexpr least max_dec_base_inv = 71; // [2^8 * (2^7 - dec_base) / dec_base]
    static constexpr least max_dec_log2 = 7; // 6.6..
    */
};

template <> struct uint_t<16>
{
    using least = uint_least16_t;
    using fast = uint_fast16_t;
    /*
    static constexpr least max_dec_base = 10000;
    static constexpr least max_dec_base_inv = 41838; // [2^16 * (2^14 - max_dec_base) / max_dec_base]
    static constexpr least max_dec_log2 = 14; // 13.2..
    */
};

template <> struct uint_t<32>
{
    using least = uint_least32_t;
    using fast = uint_fast32_t;
    /*
    static constexpr least max_dec_base = 1000000000;
    static constexpr least max_dec_base_inv = 316718722; // [2^32 * (2^30 - max_dec_base) / max_dec_base]
    static constexpr least max_dec_log2 = 30; // 29.8...
    */
};

template <> struct uint_t<64>
{
    using least = uint_least64_t;
    using fast = uint_fast64_t;
    /*
    static constexpr least max_dec_base = 0x8ac7230489e80000ULL;
    static constexpr least max_dec_base_inv = 0xd83c94fb6d2ac34aULL; // [2^64 * (2^64 - max_dec_base) / max_dec_base]
    static constexpr least max_dec_log2 = 64; // 63.1..
    */
};

#if defined(__SIZEOF_INT128__) && !defined(__CUDACC__) && !defined(_MSC_VER)
#  define PLATFORM_HAS_INT128
template <> struct uint_t<128>
{
    using least = uint_least128_t;
    using fast = uint_fast128_t;
};
#endif

}

namespace sonia::arithmetic {

inline static const unsigned char clz_tab[129] =
{
    1,2,3,3,4,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    9
};

template <std::unsigned_integral T>
inline constexpr unsigned int generic_count_leading_zeros(T x) noexcept
{
    T shift;
									
    if constexpr(std::numeric_limits<T>::digits == 32) {
        shift = x < ((T)1 << 16)
	        ? (x < ((T)1 << 8) ? 1 : 9) : (x < ((T)1 << 24) ? 17
	        : 25);
    } else {
	    for (shift = std::numeric_limits<T>::digits - 8; shift > 0; shift -= 8)
	        if (((x >> shift) & 0xff) != 0)
	            break;
	        ++shift;
    }
    return (unsigned int)(std::numeric_limits<T>::digits + 1 - shift - clz_tab[x >> shift]);
}

template <std::unsigned_integral T, unsigned int bits = std::numeric_limits<T>::digits>
inline constexpr unsigned int consteval_count_leading_zeros(T x) noexcept
{
    if constexpr (bits == 1) {
        return x ? 0 : 1;
    } else {
        constexpr T half = ((T)1) << bits / 2;
        if (x < half) {
            return bits / 2 + consteval_count_leading_zeros<T, bits / 2>(x);
        } else {
            return consteval_count_leading_zeros<T, bits / 2>(x >> bits / 2);
        }
    }
}

template <std::unsigned_integral T, unsigned int bits = std::numeric_limits<T>::digits>
inline constexpr unsigned int consteval_log2(T x) noexcept
{
    return bits - consteval_count_leading_zeros<T, bits>(x) - 1;
}

template <std::unsigned_integral T>
inline constexpr unsigned int count_leading_zeros(T x) noexcept
{
    if (std::is_constant_evaluated()) {
        return consteval_count_leading_zeros(x);
    } else {
#ifdef _MSC_VER
        if constexpr (sizeof(T) == 2) {
            return __lzcnt16(x);
        } else if constexpr (sizeof(T) == 4) {
            return __lzcnt(x);
        } else if (sizeof(T) == 8) {
            return (unsigned int)__lzcnt64(x);
        } else {
            return generic_count_leading_zeros(x);
        }
#else
        return generic_count_leading_zeros(x);
#endif
    }
}

template <typename T>
inline constexpr T ipow(T base, size_t exp) noexcept
{
    T result = 1;
    for (;;) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            return result;
        base *= base;
    }
}

template <std::unsigned_integral T>
inline constexpr int ucmp1(T a, T b) noexcept
{
    return a < b ? -1 : (a > b ? 1 : 0);
};

template <std::unsigned_integral T>
inline constexpr std::pair<T, T> uadd1(T a, T b) noexcept
{
    T nextc = 0;
    T r = a + b;
    if (r < a) ++nextc;
    return { nextc, r };
};

template <std::unsigned_integral T>
inline constexpr std::pair<T, T> uadd1c(T a, T b, T c) noexcept
{
    T nextc = 0;
    T r0 = a + b;
    if (r0 < a) ++nextc;
    T r = r0 + c;
    if (r < r0) ++nextc;
    return { nextc, r };
}

template <std::unsigned_integral T>
inline constexpr std::pair<T, T> usub1(T a, T b) noexcept
{
    T nextc = a < b;
    return { nextc, a - b };
}

template <std::unsigned_integral T>
inline constexpr std::pair<T, T> usub1c(T a, T b, T c) noexcept
{
    T nextc = a < b;
    T r = a - b;
    nextc += r < c;
    return { nextc, r - c };
};

template <std::unsigned_integral T>
inline constexpr void add2(T & sh, T & sl, T ah, T al, T bh, T bl) noexcept
{
    constexpr int bsz = std::numeric_limits<T>::digits;
    using h_t = typename uint_t<bsz * 2>::least;
    if constexpr (std::is_void_v<h_t>) {
        sl = al + bl;
        sh = ah + bh + (sl < al);
    } else {
        h_t result = ((((h_t)ah) << bsz) | al) + ((((h_t)bh) << bsz) | bl);
        sl = (T)(result & (std::numeric_limits<T>::max)());
        sh = (T)(result >> bsz);
    }
}

template <std::unsigned_integral T>
inline constexpr auto umul1(T u, T v) noexcept -> std::pair<T, T>
{
    constexpr int bsz = std::numeric_limits<T>::digits;
    if constexpr (std::is_void_v<typename uint_t<bsz * 2>::least>) {
        constexpr int hbsz = std::numeric_limits<T>::digits / 2;
        constexpr T lmask = ((T)1 << hbsz) - 1;

        using h_t = typename uint_t<hbsz>::least;
    
        h_t ul = static_cast<h_t>(u & lmask);
        h_t uh = static_cast<h_t>(u >> hbsz);
        h_t vl = static_cast<h_t>(v & lmask);
        h_t vh = static_cast<h_t>(v >> hbsz);

        T x0 = (T)ul * vl;
        T x1 = (T)ul * vh;
        T x2 = (T)uh * vl;
        T x3 = (T)uh * vh;

        x1 += (x0 >> hbsz);
        x1 += x2;
        if (x1 < x2) {
            x3 += (T)1 << hbsz;
        }
        return { x3 + ((T)x1 >> hbsz), (x1 << hbsz) + ((T)x0 & lmask) };
    } else {
        using h_t = typename uint_t<bsz * 2>::least;
        constexpr h_t lmask = ((h_t)1 << bsz) - 1;

        h_t r = ((h_t)u) * ((h_t)v);
        return { (T)(r >> bsz), (T)(r & lmask) };
    }
}

// Niels Möller and Torbjörn Granlund,
// “Improved division by invariant integers”,
//  IEEE Transactions on Computers, 11 June 2010.
//  https://gmplib.org/~tege/division-paper.pdf

template <std::unsigned_integral T>
void udiv2by1(T& q, T& r, T u1, T u0, T d, T v) noexcept
{
    assert(d >= (((T)1) << (std::numeric_limits<T>::digits - 1)));
    auto [q1, q0] = umul1(u1, v);
    add2<T>(q, q0, q1, q0, u1 + 1, u0);
    r = u0 - q * d;
    if (r > q0) {
        --q;
        r += d;
    }
    if (r >= d) {
        ++q;
        r -= d;
    }
}

template <std::integral T>
inline constexpr auto div1(T u, T d) noexcept -> std::pair<T, T>
{
#if 1
    return { u / d, u % d };
#else
    T q = u / d;
    return { q, u - q * d };
#endif
}

// {u1, u0} / d -> {q, r}
// 'norm' means that u1 high bit is set to 1
template <std::unsigned_integral T>
constexpr auto udiv2by1norm(T u1, T u0, T d) noexcept -> std::pair<T, T>
{
    assert(u1 < d);
    constexpr int bsz = std::numeric_limits<T>::digits;
    using h_t = typename uint_t<bsz * 2>::least;
    if constexpr (std::is_void_v<h_t>) {
        assert(d & (T(1) << (bsz - 1)));
        constexpr T lmask = (T(1) << (bsz / 2)) - 1;
        T d1 = d >> (bsz / 2);
        T d0 = d & lmask;
            
        auto [q1, r1] = div1(u1, d1);
        T m = q1 * d0;
        r1 = (r1 << (bsz / 2)) | (u0 >> (bsz / 2));

        if (r1 < m) {
            --q1;
            r1 += d;
            if (r1 >= d && r1 < m) {
                --q1;
                r1 += d;
            }
        }
        r1 -= m;
        auto [q0, r0] = div1(r1, d1);
        m = q0 * d0;
        r0 = (r0 << (bsz / 2)) | (u0 & lmask);
        if (r0 < m) {
            --q0;
            r0 += d;
            if (r0 >= d && r0 < m) {
                --q0;
                r0 += d;
            }
        }
        return { (q1 << (bsz / 2)) | q0, r0 - m };
    } else {
        h_t u = (((h_t)u1) << bsz) | u0;
        auto [q, r] = div1<h_t>(u, d);
        return { T(q), T(r) };
    }
}

// {u1, u0} / d -> {q, r}
template <std::unsigned_integral T>
constexpr auto udiv2by1(T u1, T u0, T d) noexcept -> std::pair<T, T>
{
    assert(u1 < d); // => d != 0
    constexpr int bsz = std::numeric_limits<T>::digits;
    using h_t = typename uint_t<bsz * 2>::least;
    if constexpr (std::is_void_v<h_t>) {
        constexpr T highbit = T(1) << (bsz - 1);
        if (!(d & highbit)) {
            int shift = count_leading_zeros(d);
            d <<= shift;
            u1 = (u1 << shift) | u0 >> (bsz - shift);
            u0 <<= shift;
            auto [q, r] = udiv2by1norm(u1, u0, d);
            return { q, r >> shift };
        }
        return udiv2by1norm(u1, u0, d);
    } else {
        h_t u = (((h_t)u1) << bsz) | u0;
        auto [q, r] = div1<h_t>(u, d);
        return { T(q), T(r) };
    }
}

template <std::unsigned_integral T>
inline constexpr T sqrt(T m) noexcept // returns floor(sqrt(m))
{
    if (!m) return m;
    T u = (T(1) << ((std::numeric_limits<T>::digits - count_leading_zeros(m) + 1) / 2)) - 1;
    //T u = m - 1;
    //T u2 = (T(1) << ((std::numeric_limits<T>::digits - count_leading_zeros(m) - 1) / 2 + 1));
    //if (u2 < u) {
    //    std::cout << "!m: " << (uint64_t)m << ",  u: " << (uint64_t)u << "\n";
    //} else if (u < u2) {
    //    std::cout << "m: " << (uint64_t)m << ",  u: " << (uint64_t)u << "\n";
    //}
    for (;;) {
        T s = u;
        T t = s + m / s;
        u = t / 2;
        if (u >= s) [[unlikely]] return s;
    }
}

template <std::unsigned_integral T>
inline constexpr T sqrt(T mh, T ml) noexcept // returns floor(sqrt(m))
{
    if (!mh && !ml) return ml;
    for (T u = mh ? (std::numeric_limits<T>::max)() : ml;;) {
        T s = u;
        auto [h, l] = udiv2by1(mh, ml, s);
        assert(h == 0);
        auto [c, tl] = uadd1(l, s);
        u = (tl >> 2) | (c << (std::numeric_limits<T>::digits - 1));
        if (u >= s) [[unlikely]] return s;
    }
}

}
