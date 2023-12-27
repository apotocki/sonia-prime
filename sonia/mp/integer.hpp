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

namespace sonia::mp {

template <typename LimbT>
class basic_integer_view
{
public:
    explicit basic_integer_view(std::span<LimbT> limbs, int sign = 1) noexcept
        : limbs_{ limbs.data() }, size_ { static_cast<intptr_t>(limbs.size()) }
    {
        if (sign < 0) size_ = -size_;
    }

    inline int sign() const noexcept { return size_ < 0 ? -1 : 1; }
    inline operator std::span<LimbT>() const noexcept { return { limbs_, static_cast<size_t>(size_ < 0 ? -size_ : size_) }; }

private:
    LimbT* limbs_;
    intptr_t size_;
};

template <typename LimbT, typename AllocatorT>
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

template <std::integral LimbT, typename OutputIteratorT>
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
            constexpr auto max_dec_base_c = W<10> ^ W<std::numeric_limits<LimbT>::digits10>;
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

template <typename LimbT, typename OutputIteratorT>
OutputIteratorT to_string_converter(std::span<LimbT> limbs, OutputIteratorT out, bool& reversed, int base = 10, std::string_view alphabet = {})
{
    using namespace std::string_view_literals;

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
        const LimbT* plimb = limbs.data() + limbs.size() - 1;
        
        // skip leading zeros
        bool skip_zeroes = true;
        for (LimbT cur = *plimb, carry = 0;;) {
            LimbT nf = carry | (cur >> limb_pos);
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
        return bc_get_str(limbs, base, alphabet, std::move(out));

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
