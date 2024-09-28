/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <type_traits>
#include <limits>
#include <span>
#include <vector>

#include "ct.hpp"

namespace sonia::mp {

// limbs are destructed during the string conversion
template <std::unsigned_integral LimbT, typename OutputIteratorT>
requires(!std::is_const_v<LimbT>)
OutputIteratorT bc_get_str(std::span<LimbT> limbs, int base, std::string_view alphabet, OutputIteratorT oi)
{
    using namespace sonia::arithmetic;
    namespace mpa = sonia::mp::arithmetic;
    using mpct::W;

    assert(!limbs.empty());
    assert(alphabet.size() >= base);
    constexpr uint32_t limb_bit_count = std::numeric_limits<LimbT>::digits;
    using limb_traits_t = uint_t<limb_bit_count>;
    
    static_assert(sizeof(LimbT) <= 8);
    char tempbuff[64]; // not more than 64 chars per limb

    if (base == 10) { // questionable choice
        while (limbs.size() > 1) {
            constexpr auto max_dec_base_c = upow(W<10>, W<std::numeric_limits<LimbT>::digits10>);
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
                    LimbT digit = frac >> (std::numeric_limits<LimbT>::digits - 4);
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
                *s = alphabet[d & 0xff];
                if (s == tempbuff) break;
                --s;
            }
            oi = std::copy(tempbuff, tempbuff + chars_per_limb, std::move(oi));
        }
    }

    for (LimbT ul = limbs.front(); ul; ++oi)
    {
        auto [q, r] = div1<LimbT>(ul, base);
        ul = q;
        *oi = alphabet[r & 0xff];
    }
    
    return std::move(oi);
}

template <std::unsigned_integral LimbT, typename OutputIteratorT>
OutputIteratorT to_string(std::span<LimbT> limbs, OutputIteratorT out, bool& reversed, int base = 10, std::string_view alphabet = {})
{
    using namespace std::string_view_literals;
    using limb_type = std::remove_cv_t<LimbT>;

    if (base < 2 || base > (alphabet.empty() ? detail::default_alphabet_big.size() : alphabet.size())) {
        throw std::invalid_argument("wrong base");
    }
    if (limbs.empty() || (limbs.size() == 1 && !limbs.front())) {
        reversed = false;
        *out = alphabet.empty() ? '0' : alphabet[0];
        ++out;
        return std::move(out);
    }

    if (alphabet.empty()) {
        alphabet = base <= detail::default_alphabet.size() ? detail::default_alphabet : detail::default_alphabet_big;
    }

    if (!(base & (base - 1))) {
        reversed = false;
        uint_least8_t mask = static_cast<uint_least8_t>(base - 1);
        uint_least8_t bits_per_digit = 0;
        for (int tmp = base - 1; tmp; ++bits_per_digit, tmp >>= 1);
        uint_least8_t offset_bits = (std::numeric_limits<LimbT>::digits * limbs.size()) % bits_per_digit;
        if (!offset_bits) offset_bits = bits_per_digit;
        int limb_pos = static_cast<int>(std::numeric_limits<LimbT>::digits) - offset_bits;
        const limb_type* plimb = limbs.data() + limbs.size() - 1;
        
        // skip leading zeros
        bool skip_zeroes = true;
        for (limb_type cur = *plimb, carry = 0;;) {
            limb_type nf = carry | (cur >> limb_pos);
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
                limb_pos += static_cast<int>(std::numeric_limits<LimbT>::digits);
            } else {
                if (skip_zeroes) {
                    *out = alphabet[0];
                    ++out;
                }
                return std::move(out);
            }
        }
    }

    //if (limbs.size() < 35) {
        reversed = true;
        if constexpr (std::is_const_v<LimbT>) {
            std::vector<limb_type> mls( limbs.begin(), limbs.end() );
            return bc_get_str(std::span{mls}, base, alphabet, std::move(out));
        } else {
            return bc_get_str(limbs, base, alphabet, std::move(out));
        }
    //} else {
    
    //}

    /*
    powtab_mem = new mp_limb_t[2 + un];
    //powtab_mem = ((mp_limb_t*)__gmp_tmp_reentrant_alloc(&__tmp_marker, (un + 2 * 64) * sizeof(mp_limb_t)));
    size_t ndig;
    mp_size_t xn;
    
    ndig = umulpp(__gmpn_bases[base].logb2, std::numeric_limits<LimbT>::digits * limbs.size()).first;
    
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

}
