/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <expected>
#include <exception>
#include <stdexcept>

#include <string_view>
#include <tuple>
#include <span>
#include <memory>
#include <cctype>
#include <cmath>

#include "sonia/utility/scope_exit.hpp"

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

//template <std::integral LimbT, std::integral T, typename AllocatorT>
//requires(std::is_unsigned_v<LimbT>)
//std::tuple<LimbT*, size_t, size_t, int> to_limbs(T value, AllocatorT&& alloc)
//{
//    throw std::runtime_error("to_limbs from integer is not implemented"); //&
//}

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
std::expected<std::tuple<LimbT*, size_t, size_t, int>, std::exception_ptr> to_limbs(std::string_view & str, unsigned int base, AllocatorT && alloc, std::span<const unsigned char> alphabet_map = {}) noexcept
{
    using namespace sonia::arithmetic;
    using result_t = std::tuple<LimbT*, size_t, size_t, int>;
    constexpr size_t limb_bit_count = std::numeric_limits<LimbT>::digits;

    result_t result{ nullptr, 0, 0, 1 };

    std::string_view orig_str = str;

    while (!str.empty() && std::isspace(str.front())) str = str.substr(1);
    if (!str.empty()) {
        if (str.front() == '-') {
            str = str.substr(1);
            std::get<3>(result) = -1;
        } else if (str.front() == '+') {
            str = str.substr(1);
        }
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
    } else if (!base) {
        base = 10;
    }

    if (str.empty()) {
        return std::unexpected(std::make_exception_ptr(std::invalid_argument("no value"s)));
    } else if (base < 2 || base > 255) {
        return std::unexpected(std::make_exception_ptr(std::invalid_argument((std::ostringstream{} << "wrong base: "sv << base).str())));
    }
    
    if (alphabet_map.empty()) {
        if (base > 62) {
            return std::unexpected(std::make_exception_ptr(std::invalid_argument((std::ostringstream{} << "An alphabet must be specified for a base greater than 62, current base: "sv << base).str())));
        }
        alphabet_map = base <= 36 ? std::span{ detail::default_alphabet_map } : std::span{ detail::default_alphabet_big_map };
    }

    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;

    auto get_digit = [alphabet_map, base](const char* pc) noexcept {
        unsigned char c = *pc;
        if (c < alphabet_map.size()) {
            if (LimbT d = alphabet_map[c]; d < base) {
                return d;
            }
        }
        return static_cast<LimbT>(base);
    };
    
    try {
        const char* pc = str.data(), * pce = pc + str.size();

        if (!(base & (base - 1))) { // base is a power of 2
            // size estimation
            uint_least8_t bits_per_digit = 0;
            for (int tmp = base - 1; tmp; ++bits_per_digit, tmp >>= 1);
            size_t boffset = str.size() * bits_per_digit;
            size_t alloccnt = (boffset + limb_bit_count - 1) / limb_bit_count;
            std::get<0>(result) = alloc_traits_t::allocate(alloc, alloccnt);
            std::get<1>(result) = std::get<2>(result) = alloccnt;
            LimbT accum_val = 0;
            LimbT* limb = std::get<0>(result) + std::get<1>(result) - 1;
            for (;;) {
                boffset -= bits_per_digit;
                LimbT d = get_digit(pc);
                if (d >= base) [[unlikely]] { // unacceptable character, stop parsing
                    throw std::invalid_argument((std::ostringstream{} << "unacceptable character '"sv << *pc << '\'').str());
                }
                ++pc;
                size_t offs = boffset % limb_bit_count;
                if (!offs || offs + bits_per_digit > limb_bit_count) {
                    auto high_offset = ((limb_bit_count - offs) % limb_bit_count);
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
            size_t digits_per_limb = (size_t)(double(limb_bit_count) / std::log2((double)base));
            LimbT big_base = ipow<LimbT>(base, digits_per_limb);

            LimbT limb = get_digit(pc);
            if (limb == base) { // can't parse a digit
                str = { pc, pce };
                throw std::invalid_argument((std::ostringstream{} << "unacceptable character '"sv << *pc << '\'').str());
            }

            auto make_zero_result = [&result](std::string_view & str, const char* pc, const char* pce, AllocatorT& alloc) noexcept -> result_t&& {
                std::get<0>(result) = alloc_traits_t::allocate(alloc, 1);
                std::get<1>(result) = std::get<2>(result) = 1;
                *std::get<0>(result) = 0;
                str = { pc, pce };
                return std::move(result);
            };

            for (; !limb; limb = get_digit(pc)) {
                if (++pc == pce) return make_zero_result(str, pc, pce, alloc);
            }
            if (limb == base) return make_zero_result(str, pc, pce, alloc);
            ++pc;

            size_t left_digits = pce - pc;
            size_t limbs_count = (left_digits + digits_per_limb) / digits_per_limb; // total digits = 1 + left_digits
            std::get<0>(result) = alloc_traits_t::allocate(alloc, limbs_count);
            std::get<2>(result) = limbs_count;

            for (;;) {
                LimbT tmp;
                auto pack_sz = (std::min)(digits_per_limb - 1, left_digits);
                for (auto k = pack_sz; k != 0; --k, ++pc) {
                    tmp = get_digit(pc);
                    if (tmp == base) [[unlikely]] {
                        pack_sz -= k;
                        break;
                    }
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
                if (pc == pce || tmp == base) break;
                limb = get_digit(pc);
                if (limb == base) [[unlikely]] break;
                ++pc;
                left_digits -= digits_per_limb;
            }
        }
        str = { pc, pce };
        return result;
    } catch (...) {
        if (auto* ptr = std::get<0>(result); ptr) {
            alloc_traits_t::deallocate(alloc, ptr, std::get<2>(result));
        }
        str = orig_str;
        return std::unexpected(std::current_exception());
    }
}

}
