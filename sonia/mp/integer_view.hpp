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

#include "sonia/utility/scope_exit.hpp"
#include "arithmetic.hpp"

#include "limbs_from_string.hpp"
#include "limbs_from_integral.hpp"
#include "limbs_to_string.hpp"
#include "ct.hpp"

#ifndef basic_integer_view_auto_buffer_size
#   define basic_integer_view_auto_buffer_size 16
#endif

namespace sonia::mp::detail {

template <std::unsigned_integral StorageT> struct integer_view_ctl;

template <std::unsigned_integral StorageT>
requires(sizeof(StorageT) == 8)
struct integer_view_ctl<StorageT>
{
    StorageT size : 56; // if size == 0 then used inplace value
    StorageT skip_bits : 6; // most significant bit count to skip
    StorageT inplace_bit : 1;
    StorageT sign_bit : 1;

    inline integer_view_ctl() noexcept
        : size{ 0 }, skip_bits{ 0 }, inplace_bit{ 0 }, sign_bit{ 0 }
    {}

    inline integer_view_ctl(size_t sz, int sign, size_t sb = 0) noexcept
        : size{static_cast<StorageT>(sz)}
        , skip_bits{ static_cast<StorageT>(sb) }
        , inplace_bit{ 0 }
        , sign_bit{ sign < 0 ? 1u : 0 }
    {
        assert(!((sz >> 56) & 0xff));
        assert(sb < 64);
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
    StorageT skip_bits : 6; // most significant bit count to skip
    StorageT inplace_bit : 1;
    StorageT sign_bit : 1;

    inline integer_view_ctl() noexcept
        : size{ 0 }, skip_bits{ 0 }, inplace_bit{ 0 }, sign_bit{ 0 }
    {}

    inline integer_view_ctl(size_t sz, int sign, size_t sb = 0) noexcept
        : size{ static_cast<StorageT>(sz) }
        , skip_bits{ static_cast<StorageT>(sb) }
        , inplace_bit{ 0 }
        , sign_bit{ sign < 0 ? 1u : 0 }
    {
        assert(!((sz >> 24) & 0xff));
        assert(sb < 64);
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
    static_assert(sizeof(LimbT) <= 8); // because we reserve only 6 bits for 'skip_bits' in ctl_type
    static constexpr size_t inplace_max_size = (std::max)(sizeof(LimbT), sizeof(const_limb_t*)) / sizeof(LimbT);

    union {
        const_limb_t* limbs_;
        LimbT inplace_value_[inplace_max_size];
    };
    ctl_type ctl_;

    inline basic_integer_view(const_limb_t* limbs, ctl_type ctl) noexcept
        : limbs_{ limbs }, ctl_{ std::move(ctl) }
    {}

    inline basic_integer_view(std::in_place_t, const_limb_t* limbs, ctl_type ctl) noexcept
        : ctl_{ std::move(ctl) }
    {
        std::copy(limbs, limbs + inplace_max_size, inplace_value_);
    }

public:
    using limb_type = LimbT;

    inline basic_integer_view() noexcept
        : limbs_{nullptr}
    {}

    template <std::integral T>
    requires (sizeof(T) <= inplace_max_size * sizeof(LimbT))
    basic_integer_view(T value, int optsignmodifier = 1) noexcept
    {
        auto [sz, sign] = to_limbs(value, std::span<LimbT, inplace_max_size>{ inplace_value_ });
        ctl_.size = sz;
        ctl_.inplace_bit = 1;
        ctl_.sign_bit = (sign * optsignmodifier) < 0 ? 1 : 0;
    }

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

    inline const_limb_t* data() const noexcept { return is_inplace() ? inplace_value_ : limbs_; }
    
    inline size_t size() const noexcept { return ctl_.size; }

    explicit operator bool() const noexcept
    {
        size_t sz = size();
        if (!sz) return false;
        --sz;
        const_limb_t* blimbs = data();
        const_limb_t* limbs = blimbs + sz;
        if (ctl_.skip_bits) {
            if (*limbs & last_limb_mask()) return true;
        } else {
            if (*limbs) return true;
        }
        while (limbs != blimbs) {
            if (*--limbs) return true;
        }
        return false;
    }

    inline int sgn() const noexcept { return (!*this) ? 0 : ctl_.sign(); }

    inline bool is_negative() const noexcept { return ctl_.sign() < 0; }
    inline bool is_inplace() const noexcept { return ctl_.inplace_bit; }

    inline std::span<const_limb_t> limbs() const noexcept
    {
        return is_inplace()
            ? std::span<const_limb_t>{ inplace_value_, size() }
            : std::span<const_limb_t>{ limbs_, size() };
    }

    inline operator std::span<const_limb_t>() const noexcept { return limbs(); }
    
    inline size_t most_significant_skipping_bits() const noexcept { return ctl_.skip_bits; }

    inline LimbT last_limb_mask() const noexcept
    {
        return ctl_.skip_bits
            ? (LimbT{ 1 } << (std::numeric_limits<LimbT>::digits - ctl_.skip_bits)) - 1
            : (std::numeric_limits<LimbT>::max)();
    }

    inline LimbT at(size_t index) const noexcept
    {
        if (index >= size()) [[unlikely]] return LimbT{ 0 };
        else if (is_inplace()) { return inplace_value_[index]; }
        else if (ctl_.skip_bits && index + 1 == size()) {
            return limbs_[index] & last_limb_mask();
        }
        return limbs_[index];
    }

    template <typename OutputIteratorT>
    inline OutputIteratorT copy_to(OutputIteratorT oi) noexcept
    {
        if (!ctl_.skip_bits) {
            return std::copy(data(), data() + size(), std::move(oi));
        } else {
            assert(!ctl_.inplace_bit);
            const_limb_t* last_limb_ptr = limbs_ + size() - 1;
            OutputIteratorT r = std::copy(limbs_, last_limb_ptr, std::move(oi));
            *r = *last_limb_ptr & last_limb_mask();
            return std::move(++r);
        }
    }

    template <typename FunctorT, typename AllocatorT = std::allocator<LimbT>>
    inline decltype(auto) with_limbs(FunctorT const& ftor, AllocatorT && alloc = AllocatorT{}) const
    {
        if (!ctl_.skip_bits) {
            return ftor(limbs(), sgn());
        } else if (size() << basic_integer_view_auto_buffer_size) {
            LimbT buff[basic_integer_view_auto_buffer_size];
            std::copy(limbs_, limbs_ + size(), buff);
            buff[size() - 1] &= last_limb_mask();
            return ftor(std::span<const_limb_t>{ buff, size() }, sgn());
        } else {
            using alloc_traits_t = std::allocator_traits<AllocatorT>;
            LimbT * buff = alloc_traits_t::allocate(alloc, size());
            SCOPE_EXIT([this, &alloc, buff] { alloc_traits_t::deallocate(alloc, buff, size()); });
            std::copy(limbs_, limbs_ + size(), buff);
            buff[size() - 1] &= last_limb_mask();
            return ftor(std::span<const_limb_t>{ buff, size() }, sgn());
        }
    }

    template <std::integral T>
    [[nodiscard]] bool is_fit() const noexcept
    {
        if constexpr (inplace_max_size == 1) {
            if (is_inplace()) {
                if constexpr (sizeof(T) > sizeof(LimbT)) return true;
                else if constexpr (std::is_unsigned_v<T>) {
                    return (std::numeric_limits<T>::max)() >= *inplace_value_;
                } else if (sgn() >= 0) {
                    return static_cast<LimbT>((std::numeric_limits<T>::max)()) >= *inplace_value_;
                } else {
                    return (~static_cast<LimbT>((std::numeric_limits<T>::min)()) + 1) >= *inplace_value_;
                }
            }
        }
        int s = sgn();
        if (!s) {
            return true;
        } else if (s > 0) {
            return (std::numeric_limits<T>::max)() >= *this;
        } else {
            return (std::numeric_limits<T>::min)() <= *this;
        }
    }

    template <std::integral T>
    [[nodiscard]] explicit operator T() const noexcept
    {
        using u_t = std::make_unsigned_t<std::remove_cv_t<T>>;
        u_t result = static_cast<u_t>(at(0));
        if constexpr (sizeof(T) > sizeof(LimbT)) {
            constexpr size_t max_limbs_to_process = (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT);
            intptr_t limbs_to_process = (std::min)(size(), max_limbs_to_process);
            for (intptr_t i = 1; i < limbs_to_process; ++i) {
                result |= static_cast<u_t>(at(i)) << (i * std::numeric_limits<LimbT>::digits);
            }
        }
        // for signed types in case of overflow the behaviour is similar to C cast's one
        if (is_negative()) {
           result = ~result + 1;
        }
        return static_cast<T>(result);
    }

    template <std::floating_point T>
    [[nodiscard]] explicit operator T() const
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
        
        if (is_negative()) {
            result = -result;
        }
        return result;
    }

    [[nodiscard]] inline basic_integer_view operator- () const noexcept
    {
        return is_inplace()
            ? basic_integer_view { std::in_place, inplace_value_, ctl_.negated() }
            : basic_integer_view{ limbs_, ctl_.negated() };
    }

    [[nodiscard]] inline basic_integer_view abs() const noexcept
    {
        return sgn() < 0 ? -*this : *this;
    }

    [[nodiscard]] friend bool operator ==(basic_integer_view lhs, basic_integer_view rhs) noexcept
    {
        int lsgn = lhs.sgn(), rsgn = rhs.sgn();
        if (!lsgn) return !rsgn;
        if (!rsgn) return false;
        if ((lsgn < 0 && rsgn > 0) || (lsgn > 0 && rsgn < 0)) return false;

        size_t lsz = lhs.size();
        size_t rsz = rhs.size();
        LimbT const* lb = lhs.data(), * le = lb + lsz;
        LimbT const* rb = rhs.data(), * re = rb + rsz;

        if (lsz < rsz) {
            --re;
            if ( *re & rhs.last_limb_mask() ) return false;
            for (--rsz; lsz < rsz; --rsz) {
                if (*--re) return false;
            }
        } else if (rsz < lsz) {
            --le;
            if ( *le & lhs.last_limb_mask() ) return false;
            for (--lsz; rsz < lsz; --lsz) {
                if (*--le) return false;
            };
        }
        if (lb == le) [[unlikely]] return true;

        LimbT llimb = ((le == lb + lsz) && lhs.most_significant_skipping_bits()) ? *--le & lhs.last_limb_mask() : *--le;
        LimbT rlimb = ((re == rb + rsz) && rhs.most_significant_skipping_bits()) ? *--re & rhs.last_limb_mask() : *--re;
        if (llimb != rlimb) return false;
        
        for (; lb != le;) {
            if (*--le != *--re) return false;
        }
        return true;
    }

    template <std::integral T>
    [[nodiscard]] friend bool operator ==(basic_integer_view lhs, T rhs)
    {
        if constexpr (std::is_signed_v<T>) {
            int s = lhs.sgn();
            if (s < 0) {
                if (rhs >= 0) return false;
            } else if (!s) {
                if (rhs != 0) return false;
            } else {
                if (rhs <= 0) return false;
            }
        } else if (lhs.sgn() < 0) return false;

        std::make_unsigned_t<T> rabsval = rhs;
        if constexpr (std::is_signed_v<T>) {
            if (rhs < 0) rabsval = ~rabsval + 1;
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

    [[nodiscard]] friend std::strong_ordering operator <=> (basic_integer_view lhs, basic_integer_view rhs) noexcept
    {
        if (lhs.is_negative() && !rhs.is_negative()) return std::strong_ordering::less;
        if (!lhs.is_negative() && rhs.is_negative()) return std::strong_ordering::greater;
        size_t lsz = lhs.size();
        size_t rsz = rhs.size();
        
        LimbT const* lb = lhs.data();
        LimbT const* le = lb + lsz;

        LimbT const* rb = rhs.data();
        LimbT const* re = rb + rsz;
        
        if (lsz < rsz) {
            --re;
            if ((rhs.most_significant_skipping_bits() && (*re & rhs.last_limb_mask())) || (!rhs.most_significant_skipping_bits() && *re)) return !rhs.is_negative() ? std::strong_ordering::less : std::strong_ordering::greater;
            --rsz;
            while (lsz < rsz) {
                --re;
                if (*re) return !rhs.is_negative() ? std::strong_ordering::less : std::strong_ordering::greater;
                --rsz;
            }
        } else if (rsz < lsz) {
            --le;
            if ((lhs.most_significant_skipping_bits() && (*le & lhs.last_limb_mask())) || (!lhs.most_significant_skipping_bits() && *le)) return !lhs.is_negative() ? std::strong_ordering::greater : std::strong_ordering::less;
            --lsz;
            while (rsz < lsz) {
                --le;
                if (*le) return !lhs.is_negative() ? std::strong_ordering::greater : std::strong_ordering::less;
                --lsz;
            };
        }
        if (lb == le) [[unlikely]] return std::strong_ordering::equal;

        LimbT llimb = ((le == lb + lsz) && lhs.most_significant_skipping_bits()) ? *--le & lhs.last_limb_mask() : *--le;
        LimbT rlimb = ((re == rb + rsz) && rhs.most_significant_skipping_bits()) ? *--re & rhs.last_limb_mask() : *--re;
        if (auto r = llimb <=> rlimb; r != std::strong_ordering::equal) {
            return (!lhs.is_negative() ? r : (0 <=> r));
        }
        for (; lb != le;) {
            --le; --re;
            auto r = *le <=> *re;
            if (r != std::strong_ordering::equal) {
                return (!lhs.is_negative() ? r : (0 <=> r));
            }
        }
        return std::strong_ordering::equal;
    }

    template <std::integral T>
    [[nodiscard]] friend std::strong_ordering operator <=> (basic_integer_view lhs, T rhs)
    {
        /*
        if constexpr (std::is_signed_v<T>) {
            if (lhs.sign() < 0 && rhs > 0) return std::strong_ordering::less;
            if (lhs.sign() > 0 && rhs < 0) return std::strong_ordering::greater;
        } else {
            if (lhs.sign() < 0) return std::strong_ordering::less;
        }
        */
        return lhs <=> basic_integer_view{ rhs };
    }
};

template <std::unsigned_integral LimbT>
basic_integer_view(std::span<LimbT>, int sign) -> basic_integer_view<LimbT>;

using integer_view = basic_integer_view<uint64_t>;

template <typename T> struct is_basic_integer_view : std::false_type {};
template <typename LimbT> struct is_basic_integer_view<basic_integer_view<LimbT>> : std::true_type {};
template <typename T> constexpr bool is_basic_integer_view_v = is_basic_integer_view<T>::value;

template <std::unsigned_integral LimbT, typename VectorT>
void to_vector(basic_integer_view<LimbT> const& iv, int base, bool showbase, VectorT& result)
{
    bool reversed;
    if (iv.is_negative()) result.push_back('-');
    if (showbase) {
        switch (base) {
            case 8: result.push_back('0'); break;
            case 16: result.push_back('0'); result.push_back('x'); break;
        }
    }
    size_t pos = result.size();
    iv.with_limbs([&result, &reversed](std::span<const LimbT> sp, int) { to_string(sp, std::back_inserter(result), reversed); });
    
    if (reversed) {
        std::reverse(result.begin() + pos, result.end());
    }
}

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
    iv.with_limbs([&result, &reversed](std::span<const LimbT> sp, int) { to_string(sp, std::back_inserter(result), reversed); });
    if (iv.is_negative()) os << '-';
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


template <std::unsigned_integral LimbT>
inline size_t hash_value(basic_integer_view<LimbT> const& v) noexcept
{
    size_t seed = 0;

    size_t vsz = v.size();
    if (!vsz) return seed;

    LimbT const* b = v.data();
    LimbT const* e = b + vsz - 1;
    
    LimbT l = *e & v.last_limb_mask();
    if (l) [[likely]] {
        sonia::hash_combine(seed, l);
    } else {
        for (;;) {
            if (b == e) return seed; // holds 0
            --e;
            if (*e) { ++e; break; }
        }
    }

    for (; b != e; ++b) {
        sonia::hash_combine(seed, *b);
    }
    sonia::hash_combine(seed, v.sgn());
    return seed;
}

}
