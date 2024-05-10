/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <type_traits>
#include <limits>
#include <concepts>
#include <cassert>
#include <iterator>
#include <vector>
#include <span>
#include <compare>
#include <iosfwd>

#include "arithmetic.hpp"

#include "limbs_from_string.hpp"
#include "limbs_from_integral.hpp"
#include "limbs_to_string.hpp"
#include "ct.hpp"

namespace sonia::mp::detail {

template <std::unsigned_integral StorageT> struct integer_view_ctl;

template <std::unsigned_integral StorageT>
requires(sizeof(StorageT) == 8)
struct integer_view_ctl<StorageT>
{
    StorageT size : 56;
    StorageT skip_bits : 7; // most significant bit count to skip
    StorageT sign_bit : 1;

    inline integer_view_ctl() noexcept
        : size{ 0 }, skip_bits{ 0 }, sign_bit{ 0 }
    {}

    inline integer_view_ctl(size_t sz, int sign, size_t sb = 0)
        : size{static_cast<StorageT>(sz)}
        , skip_bits{ static_cast<StorageT>(sb) }
        , sign_bit{ sign < 0 ? 1u : 0 }
    {
        assert(!((sz >> 56) & 0xff));
        assert(sb < 128);
    }

    inline int sign() const noexcept { return sign_bit ? -1 : 1; }

    inline integer_view_ctl negated() const noexcept
    {
        integer_view_ctl result{ *this };
        result.sign_bit = ~result.sign_bit;
        return result;
    }
};

template <std::unsigned_integral StorageT>
requires(sizeof(StorageT) == 4)
struct integer_view_ctl<StorageT>
{
    StorageT size : 24;
    StorageT skip_bits : 7;
    StorageT sign_bit : 1;

    inline integer_view_ctl() noexcept
        : size{ 0 }, skip_bits{ 0 }, sign_bit{ 0 }
    {}

    inline integer_view_ctl(size_t sz, int sign, size_t sb = 0) noexcept
        : size{ static_cast<StorageT>(sz) }
        , skip_bits{ static_cast<StorageT>(sb) }
        , sign_bit{ sign < 0 ? 1u : 0 }
    {
        assert(!((sz >> 24) & 0xff));
        assert(sb < 128);
    }

    inline int sign() const noexcept { return sign_bit ? -1 : 1; }

    inline integer_view_ctl negated() const noexcept
    {
        integer_view_ctl result{ *this };
        result.sign_bit = ~result.sign_bit;
        return result;
    }
};

} // sonia::mp::detail

namespace sonia::mp {
template <std::unsigned_integral LimbT>
class basic_integer_view
{
    template <std::unsigned_integral> friend class basic_integer_view;

    using const_limb_t = std::add_const_t<LimbT>;
    using ctl_type = detail::integer_view_ctl<uintptr_t>; // to ensure the same size as a pointer size

    const_limb_t* limbs_;
    ctl_type ctl_;

    inline basic_integer_view(LimbT const * limbs, ctl_type ctl) noexcept
        : limbs_{ limbs }, ctl_{ ctl }
    {}

public:
    basic_integer_view() noexcept
        : limbs_{nullptr}
    {}

    explicit basic_integer_view(std::span<const LimbT> limbs, int sign = 1, size_t skip_bits = 0) noexcept
        : limbs_{ limbs.data() }, ctl_{ limbs.size(), sign, skip_bits }
    {
        assert(std::numeric_limits<LimbT>::digits > skip_bits); // nonsense in skipping all limb bits
    }

    template <LimbT ... ls>
    basic_integer_view(mpct::limbs<LimbT, ls...>, int sign = 1) noexcept
        : limbs_{ mpct::limbs<LimbT, ls...>::data }, ctl_{ sizeof...(ls), sign }
    {}

    template <typename LT>
    requires(std::is_same_v<std::remove_cv_t<LimbT>, LT>)
    basic_integer_view(basic_integer_view<LT> rhs) noexcept
        : limbs_{ rhs.limbs_ }, ctl_{ rhs.ctl_ }
    {}

    inline const_limb_t* data() const noexcept { return limbs_; };
    
    inline size_t size() const noexcept { return ctl_.size; }

    inline int sign() const noexcept { return ctl_.sign(); }

    inline std::span<const_limb_t> limbs() const noexcept { return { limbs_, size() }; }

    inline operator std::span<const_limb_t>() const noexcept { return limbs(); }
    
    inline size_t most_significant_skipping_bits() const noexcept { return ctl_.skip_bits; }

    inline LimbT last_limb_mask() const noexcept
    {
        return (LimbT{ 1 } << (std::numeric_limits<LimbT>::digits - ctl_.skip_bits)) - 1;
    }

    inline LimbT at(size_t index) const noexcept
    {
        if (index >= size()) [[unlikely]] return LimbT{ 0 };
        if (ctl_.skip_bits && index + 1 == size())[[unlikely]] {
            return limbs_[index] & last_limb_mask();
        }
        return limbs_[index];
    }

    template <std::integral T>
    explicit operator T() const
    {
        if (!size()) [[unlikely]] return 0;
        using u_t = std::make_unsigned_t<std::remove_cv_t<T>>;
        u_t result = static_cast<u_t>(at(0));
        if constexpr (sizeof(T) > sizeof(LimbT)) {
            constexpr intptr_t max_limbs_to_process = (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT);
            intptr_t limbs_to_process = (std::min)(size(), max_limbs_to_process);
            for (intptr_t i = 1; i < limbs_to_process; ++i) {
                result |= static_cast<u_t>(at(i)) << (i * std::numeric_limits<LimbT>::digits);
            }
        }
        // for signed types in case of overflow the behaviour is similar to c cast's one
        if (sign() < 0) {
           result = ~result + 1;
        }
        return static_cast<T>(result);
    }

    template <std::floating_point T>
    explicit operator T() const
    {
        if (!size()) [[unlikely]] return 0;
        using r_t = std::remove_cv_t<T>;
        r_t result = static_cast<r_t>(at(0));
        r_t mval = 1;
        intptr_t limbs_to_process = size();
        for (intptr_t i = 1; i < limbs_to_process; ++i) {
            mval *= r_t(LimbT(1) << (std::numeric_limits<LimbT>::digits - 1)) * 2;
            result += static_cast<r_t>(at(i)) * mval;
        }
        
        if (sign() < 0) {
            result = -result;
        }
        return result;
    }

    explicit operator bool() const
    {
        size_t sz = size();
        if (!sz) return false;
        --sz;
        if (ctl_.skip_bits) {
            if (limbs_[sz] & last_limb_mask()) return true;
        } else {
            if (!limbs_[sz]) return true;
        }
        while (sz != 0) {
            --sz;
            if (!limbs_[sz]) return true;
        }
        return false;
    }

    inline basic_integer_view operator- () const noexcept
    {
        return basic_integer_view { limbs_, ctl_.negated() };
    }

    friend bool operator ==(basic_integer_view lhs, basic_integer_view rhs)
    {
        if ((lhs.sign() < 0 && rhs.sign() > 0) || (lhs.sign() > 0 && rhs.sign() < 0)) return false;
        LimbT const* lb = lhs.limbs_, * le = lb + lhs.size();
        LimbT const* rb = rhs.limbs_, * re = rb + rhs.size();
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
            if ((lhs.sign() < 0 && rhs > 0) || (lhs.sign() > 0 && rhs < 0)) return false;
        } else {
            if (rhs && lhs.sign() < 0) return false;
        }

        T rabsval;
        if constexpr (std::is_signed_v<T>) {
            rabsval = std::abs(rhs);
        } else {
            rabsval = rhs;
        }

        if (lhs.at(0) != static_cast<LimbT>(rabsval)) return false;
        constexpr auto limbs_to_compare = (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT);
        size_t lsz = lhs.size();
        if constexpr (limbs_to_compare > 1) {
            for (int i = 1; i < limbs_to_compare; ++i) {
                LimbT l = static_cast<LimbT>(rabsval >> (i * std::numeric_limits<LimbT>::digits));
                if (l != (lsz > i ? lhs.at(i) : LimbT{ 0 })) return false;
            }
        }
        for (size_t i = lsz - 1; i >= limbs_to_compare; --i) {
            if (lhs.at(i)) return false;
        }
        return true;
    }

    friend std::strong_ordering operator <=> (basic_integer_view lhs, basic_integer_view rhs)
    {
        if (lhs.sign() < 0 && rhs.sign() > 0) return std::strong_ordering::less;
        if (lhs.sign() > 0 && rhs.sign() < 0) return std::strong_ordering::greater;
        size_t lsz = lhs.size();
        size_t rsz = rhs.size();
        
        LimbT const* re = rhs.limbs_ + rsz;
        LimbT const* le = lhs.limbs_ + lsz;
        if (lsz < rsz) {
            do {
                --re;
                if (*re) return rhs.sign() > 0 ? std::strong_ordering::less : std::strong_ordering::greater;
                --rsz;
            } while (lsz < rsz);
        } else if (rsz < lsz) {
            do {
                --le;
                if (*le) return lhs.sign() > 0 ? std::strong_ordering::greater : std::strong_ordering::less;
                --lsz;
            } while (rsz < lsz);
        }
        for (; lhs.limbs_ != le;) {
            --le; --re;
            auto r = *le <=> *re;
            if (r != std::strong_ordering::equal) {
                return (lhs.sign() > 0 ? r : (0 <=> r));
            }
        }
        return std::strong_ordering::equal;
    }

    template <std::integral T>
    friend std::strong_ordering operator <=> (basic_integer_view lhs, T rhs)
    {
        /*
        if constexpr (std::is_signed_v<T>) {
            if (lhs.sign() < 0 && rhs > 0) return std::strong_ordering::less;
            if (lhs.sign() > 0 && rhs < 0) return std::strong_ordering::greater;
        } else {
            if (lhs.sign() < 0) return std::strong_ordering::less;
        }
        */
        std::remove_cv_t<LimbT> buf[(sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT)];
        auto [sz, sign] = to_limbs(rhs, std::span{buf});
        return lhs <=> basic_integer_view{ std::span{buf, sz}, sign };
    }
};

template <std::unsigned_integral LimbT>
basic_integer_view(std::span<LimbT>, int sign) -> basic_integer_view<LimbT>;

template <typename Elem, typename Traits, std::unsigned_integral LimbT>
std::basic_ostream<Elem, Traits>& operator <<(std::basic_ostream<Elem, Traits>& os, basic_integer_view<LimbT> iv)
{
    using namespace std::string_view_literals;

    int base;
    int flags = os.flags();
    if (flags & std::ios_base::hex) {
        base = 16;
    } else if (flags & std::ios_base::oct) {
        base = 8;
    } else {
        base = 10;
    }

    std::vector<Elem> result;
    bool reversed;
    to_string(iv.limbs(), std::back_inserter(result), reversed);
    if (iv.sign() < 0) os << '-';
    if (flags & std::ios_base::showbase) {
        switch (base) {
        case 8: os << '0'; break;
        case 16: os << "0x"sv; break;
        }
    }
    if (reversed) {
        std::copy(result.rbegin(), result.rend(), std::ostreambuf_iterator(os));
    } else {
        std::copy(result.begin(), result.end(), std::ostreambuf_iterator(os));
    }
    return os;
}

}
