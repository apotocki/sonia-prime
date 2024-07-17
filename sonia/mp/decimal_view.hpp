/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <cstdint>
#include <utility>
#include <concepts>
#include <algorithm>
#include <iterator>
#include <string>
#include <iosfwd>

#include "integer_view.hpp"
#include "basic_integer.hpp"

namespace sonia::mp {

template <std::unsigned_integral LimbT>
class basic_decimal_view
{
public:
    using limb_type = LimbT;
    basic_integer_view<LimbT> significand_;
    basic_integer_view<LimbT> exponent_;

    template <typename ST, typename ET>
    inline basic_decimal_view(ST && s, ET && e) noexcept
        : significand_{ std::forward<ST>(s) }, exponent_{ std::forward<ET>(e) }
    {}

    inline bool is_negative() const noexcept { return significand_.is_negative(); }
    inline int sgn() const noexcept { return significand_.sgn(); }

    template <std::integral T>
    explicit operator T() const noexcept
    {
        if (!exponent_) return (T)significand_;
        constexpr size_t bisz = (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT);
        return (T)(basic_integer<LimbT, bisz>)*this;
    }

    template <size_t N, typename AllocatorT>
    explicit operator basic_integer<LimbT, N, AllocatorT>() const
    {
        if (!exponent_.template is_fit<int>()) {
            if (exponent_.sgn() > 0) {
                throw std::invalid_argument("exponent is too large");
            } else {
                return 0;
            }
        }
        basic_integer<LimbT, N, AllocatorT> val10{ 10 };
        auto expm = pow(val10, (unsigned int)std::abs((int)exponent_));

        basic_integer<LimbT, N, AllocatorT> result{ significand_ };
        if (exponent_.sgn() > 0) {
            result *= expm;
        } else {
            result /= expm;
        }

        return result;
    }

    inline basic_integer_view<LimbT> const& significand() const noexcept { return significand_; }
    inline basic_integer_view<LimbT> const& exponent() const noexcept { return exponent_; }
};

using decimal_view = basic_decimal_view<uint64_t>;

// expected normilized value
template <std::unsigned_integral LimbT>
bool operator== (basic_decimal_view<LimbT> const& lhs, basic_decimal_view<LimbT> const& rhs) noexcept
{
    return lhs.exponent() == rhs.exponent() && lhs.significand() == rhs.significand();
}

template <std::unsigned_integral LimbT, std::integral T>
bool operator ==(basic_decimal_view<LimbT> const& lhs, T rhs) noexcept
{
    if (!rhs) return !lhs;
    if (!lhs || lhs.exponent().is_negative()) return false;
    size_t exp = 0;
    for (;;) {
        auto [q, r] = sonia::arithmetic::div1(rhs, 10);
        if (r) break;
        rhs = q;
        ++exp;
    }
    return lhs.significand() == rhs && lhs.exponent() == exp;
}

template <std::unsigned_integral LimbT>
std::strong_ordering operator<=> (basic_decimal_view<LimbT> const& lhs, basic_decimal_view<LimbT> const& rhs)
{
    int lsgn = lhs.significand().sgn();
    if (lsgn < 0) {
        if (rhs.significand().sgn() >= 0) return std::strong_ordering::less;
    } else if (!lsgn) {
        int rsgn = rhs.significand().sgn();
        return !rsgn ? std::strong_ordering::equal : (rsgn < 0 ? std::strong_ordering::greater : std::strong_ordering::less);
    } else {
        if (rhs.significand().sgn() <= 0) return std::strong_ordering::greater;
    }

    auto r = basic_integer<LimbT, 1>{rhs.exponent() } - lhs.exponent(); // can throw bad_alloc
    constexpr size_t big_base_digits_per_limb = std::numeric_limits<LimbT>::digits10;
    constexpr LimbT big_base = sonia::arithmetic::ipow<LimbT>(10, big_base_digits_per_limb);
    if (!r) {
        return lhs.significand() <=> rhs.significand();
    }
    auto [lsa, rsa, less_res] = r.is_negative()
        ? std::tuple{ rhs.significand().abs(), lhs.significand().abs(), lsgn > 0 ? std::strong_ordering::greater : std::strong_ordering::less }
        : std::tuple{ lhs.significand().abs(), rhs.significand().abs(), lsgn < 0 ? std::strong_ordering::greater : std::strong_ordering::less };
    
    if (r.is_negative()) {
        r.negate();
    }

    basic_integer<LimbT, 2> operand; // for now div needs more space for result, so it's optimization for 1-limb values
    
    if (lsa < rsa) return less_res;
    operand = lsa;
    for (;;) {
        if (auto res = r <=> big_base_digits_per_limb; res == std::strong_ordering::less || res == std::strong_ordering::equal) {
            operand /= (res == std::strong_ordering::equal ? big_base : sonia::arithmetic::ipow<LimbT>(10, (size_t)r)); // can throw bad_alloc
            return operand >= rsa ? 0 <=> less_res : less_res;
        } else {
            operand /= big_base;
            r -= big_base_digits_per_limb;
            if (operand < rsa) return less_res;
        }
    }
}

template <std::unsigned_integral LimbT>
inline std::string to_string(basic_decimal_view<LimbT> const& val)
{
    std::string result;
    bool reversed;

    int sgn = val.sgn();
    if (!sgn) {
        result = std::string("0"sv);
    }
    if (sgn < 0) result.push_back('-');
    size_t offset = result.size();

    val.significand().with_limbs([&result, &reversed](std::span<const LimbT> sp, int) { to_string(sp, std::back_inserter(result), reversed); });

    if (reversed) {
        std::reverse(result.begin() + offset, result.end());
    }
    int64_t e = (int64_t)val.exponent();
    if (e >= 0) {
        result.resize(result.size() + e, '0');
    } else {
        int pos = sgn < 0 ? 1 : 0;
        int64_t zpadcount = -e - (int64_t)result.size() + pos + 1;
        if (zpadcount > 0) {
            result.insert(result.begin() + pos, zpadcount, '0');
        }
        result.insert(result.begin() + result.size() + e, '.');
    }
    return result;
}

template <typename Elem, typename Traits, std::unsigned_integral LimbT>
inline std::basic_ostream<Elem, Traits>& operator <<(std::basic_ostream<Elem, Traits>& os, basic_decimal_view<LimbT> const& dv)
{
    return os << to_string(dv);
}

using decimal_view = basic_decimal_view<uint64_t>;

}

