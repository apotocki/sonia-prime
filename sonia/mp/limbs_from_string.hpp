/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <string_view>
#include <tuple>
#include <memory>

#include "limb_arithmetic.hpp"

namespace sonia::mp::detail {

using namespace std::string_view_literals;

inline std::string_view default_alphabet = "0123456789abcdefghijklmnopqrstuvwxyz"sv;
inline std::string_view default_alphabet_big = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"sv;

inline const unsigned char default_alphabet_map[] =
{
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    0,1,2,3,4,5,6,7,8,9,255,255,255,255,255,255,
    255,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
    25,26,27,28,29,30,31,32,33,34,35,255,255,255,255,255,
    255,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
    25,26,27,28,29,30,31,32,33,34,35
};

inline const unsigned char default_alphabet_big_map[] =
{
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    0,1,2,3,4,5,6,7,8,9,255,255,255,255,255,255,
    255,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
    25,26,27,28,29,30,31,32,33,34,35,255,255,255,255,255,
    255,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,
    51,52,53,54,55,56,57,58,59,60,61
};

}

namespace sonia::mp {


template <std::integral LimbT, typename AllocatorT>
requires(std::is_unsigned_v<LimbT> && std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
std::tuple<LimbT*, size_t, size_t, int> to_limbs(std::string_view str, unsigned int base, AllocatorT && alloc, std::span<const unsigned char> alphabet_map = {})
{
    using namespace sonia::arithmetic;
    using result_t = std::tuple<LimbT*, size_t, size_t, int>;
    result_t result{ nullptr, 0, 0, 1 };

    while (!str.empty() && std::isspace(str.front())) str = str.substr(1);
    if (!str.empty() && str.front() == '-') {
        str = str.substr(1);
        std::get<3>(result) = -1;
    }

    // use or skip prefix
    if (str.size() > 1 && str.front() == '0') {
        char c = str[1];
        if ((0 == base || 16 == base) && (c == 'X' || c == 'x')) {
            base = 16;
            str = str.substr(2);
        } else if ((0 == base || 2 == base) && (c == 'b' || c == 'B')) {
            base = 2;
            str = str.substr(2);
        } else if (0 == base || 8 == base) {
            base = 2;
            str = str.substr(2);
        }
    } else if (base < 2) {
        base = 10;
    }

    if (str.empty() || base < 2 || base > 255) {
        return result;
    }

    if (alphabet_map.empty()) {
        if (base > 62) {
            return result;
        }
        alphabet_map = base <= 36 ? std::span{ detail::default_alphabet_map } : std::span{ detail::default_alphabet_big_map };
    }

    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;

    /*
    using allocator_type = std::conditional_t<
        std::is_same_v<LimbT, typename alloc_traits_t::value_type>,
        AllocatorT,
        alloc_traits_t::template rebind_alloc<T>
    >;
    allocator_type alloc{argalloc}
    */

    auto free_limbs = [&alloc, &result]() {
        if (auto * ptr = std::get<0>(result); ptr) {
            alloc_traits_t::deallocate(alloc, ptr, std::get<2>(result));
        }
    };
    
    auto get_digit = [alphabet_map, base, &free_limbs](const unsigned char* pc) {
        unsigned char c = *pc;
        if (c < alphabet_map.size()) {
            LimbT d = alphabet_map[c];
            if (d < base) {
                return d;
            } else {
                free_limbs();
                throw std::invalid_argument("unacceptable character");
            }
        } else {
            free_limbs();
            throw std::invalid_argument("unacceptable character");
        }
    };
    
    /*
    auto get_digit = [&alphabet_map](const unsigned char* pc) {
        return alphabet_map[*pc];
    };
    */
    if (!(base & (base - 1))) { // base is a power of 2
        // size estimation
        uint_least8_t bits_per_digit = 0;
        for (int tmp = base - 1; tmp; ++bits_per_digit, tmp >>= 1);
        size_t boffset = str.size() * bits_per_digit;
        size_t alloccnt = (boffset + sizeof(LimbT) * CHAR_BIT - 1) / (sizeof(LimbT) * CHAR_BIT);
        std::get<0>(result) = alloc_traits_t::allocate(alloc, alloccnt);
        std::get<1>(result) = std::get<2>(result) = alloccnt;
        LimbT accum_val = 0;
        LimbT* limb = std::get<0>(result) + std::get<1>(result) - 1;
        for (const unsigned char* pc = reinterpret_cast<const unsigned char*>(str.data());; ++pc) {
            boffset -= bits_per_digit;
            LimbT d = get_digit(pc);
            size_t offs = boffset % std::numeric_limits<LimbT>::digits;
            if (!offs || offs + bits_per_digit > std::numeric_limits<LimbT>::digits) {
                auto high_offset = ((std::numeric_limits<LimbT>::digits - offs) % std::numeric_limits<LimbT>::digits);
                accum_val |= d >> high_offset;
                *limb = accum_val;
                if (limb == std::get<0>(result)) break;
                --limb;
                accum_val = offs ? (d << offs) : 0;
            } else {
                accum_val |= (d << offs);
            }
        }
        assert(!boffset);

    } else {
        //size_t digits_per_limb = (size_t)(std::log((std::numeric_limits<LimbT>::max)()) / std::log((double)base));
        size_t digits_per_limb = (size_t)(double(sizeof(LimbT) * CHAR_BIT) / std::log2((double)base));
        LimbT big_base = ipow<LimbT>(base, digits_per_limb);

        const unsigned char* pc = reinterpret_cast<const unsigned char*>(str.data()), *pce = pc + str.size();
        LimbT limb = get_digit(pc); ++pc;
        while (!limb) {
            if (pc == pce) {
                std::get<0>(result) = alloc_traits_t::allocate(alloc, 1);
                std::get<1>(result) = std::get<2>(result) = 1;
                *std::get<0>(result) = 0;
                return result;
            }
            limb = get_digit(pc); ++pc;
        }
        size_t left_digits = pce - pc;
        size_t limbs_count = (left_digits + digits_per_limb) / digits_per_limb; // total digits = 1 + left_digits
        std::get<0>(result) = alloc_traits_t::allocate(alloc, limbs_count);
        std::get<2>(result) = limbs_count;

        for (;;) {
            auto pack_sz = (std::min)(digits_per_limb - 1, left_digits);
            for (auto k = pack_sz; k != 0; --k, ++pc) {
                limb = limb * base + get_digit(pc);
            }
            if (std::get<1>(result)) {
                LimbT mpr = pack_sz == digits_per_limb - 1 ? big_base : ipow<LimbT>(base, pack_sz + 1);
                limb = arithmetic::umul1(std::span{ std::get<0>(result), std::get<1>(result) }, mpr, limb);
            }
            if (limb) {
                *(std::get<0>(result) + std::get<1>(result)) = limb;
                ++std::get<1>(result);
            }
            if (pc == pce) break;
            limb = get_digit(pc); ++pc;
            left_digits -= digits_per_limb;
        }
    }

    return result;
}


}
