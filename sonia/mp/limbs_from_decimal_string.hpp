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
#include <sstream>
#include <locale>

#include "sonia/utility/scope_exit.hpp"

#include "limbs_from_string.hpp"

namespace sonia::mp {

template <std::unsigned_integral LimbT, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
std::expected<std::tuple<LimbT*, size_t, size_t, int>, std::exception_ptr> to_significand_limbs(std::string_view & str, AllocatorT&& alloc, int64_t & exponent) noexcept
{
    using namespace sonia::arithmetic;
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;

    //constexpr size_t limb_bit_count = std::numeric_limits<LimbT>::digits;
    constexpr size_t digits_per_limb = std::numeric_limits<LimbT>::digits10;
    constexpr LimbT big_base = ipow<LimbT>(10u, digits_per_limb);

    using result_t = std::tuple<LimbT*, size_t, size_t, int>;
    result_t result{ nullptr, 0, 0, 1 };
    exponent = 0;

    while (!str.empty() && std::isspace(str.front())) str = str.substr(1);
    if (!str.empty() && str.front() == '-') {
        str = str.substr(1);
        std::get<3>(result) = -1;
    }
    if (str.empty()) [[unlikely]] {
        return std::unexpected(std::make_exception_ptr(std::invalid_argument("no value"s)));;
    }
    const char* pc = str.data(), *pce = pc + str.size();

    int point_pos = -1;
    // returns {not zero digit, preceding number of zeros}
    auto get_digit = [&point_pos, &pc, pce]() -> std::pair<LimbT, size_t> {
        size_t zcnt = 0;
        for (; pc != pce; ++pc) {
            //std::cout << "p get_digit\n" << std::flush;
            unsigned char c = *pc;
            if (c >= '1' && c <= '9') {
                ++pc;
                if (point_pos >= 0) ++point_pos;
                return { static_cast<LimbT>(c - '0'), zcnt };
            } else if (c == '.' && point_pos < 0) {
                ++point_pos;
            } else if (c != '0') {
                return { 0, zcnt };
            } else { // c == '0'
                if (point_pos >= 0) ++point_pos;
                ++zcnt;
            }
        }
        return { 0, zcnt };
    };

    // skip zeros
    auto [limb, zcnt] = get_digit();
    if (!limb) { // the result is 0 or empty
        if (!zcnt) return std::unexpected(std::make_exception_ptr(std::invalid_argument("no value"s)));
        str = { pc, pce };
        while (pc != pce) {// not a loop, jsust the scope
            if ((*pc != 'e' && *pc != 'E') || pc < str.data() + 1) [[unlikely]] break;
            // the result is 0, just validate exponent part
            if (++pc == pce) [[unlikely]] break;
            if (*pc == '+' || *pc == '-') {
                ++pc;
                if (pc == pce) [[unlikely]] break;
            }
            do {
                if (*pc < '0' || *pc > '9') [[unlikely]] break;
                ++pc;
            } while (pc != pce);
            str = { pc, pce }; // has eaten the complete exponent part
        }
        std::get<0>(result) = alloc_traits_t::allocate(alloc, 1);
        std::get<1>(result) = std::get<2>(result) = 1;
        *std::get<0>(result) = 0;
        return result;
    }
    size_t left_digits = pce - pc;
    size_t max_limbs_count = (left_digits + digits_per_limb) / digits_per_limb; // total digits = 1 + left_digits
    std::get<0>(result) = alloc_traits_t::allocate(alloc, max_limbs_count);
    std::get<2>(result) = max_limbs_count;
    *std::get<0>(result) = limb;
    std::get<1>(result) = 1;
    limb = 0;
    for (size_t dc = 0;;) {
        auto [nextlimb, zcnt] = get_digit();
        if (!nextlimb) { // significand is finished
            exponent = point_pos < 0 ? zcnt : -static_cast<int64_t>(point_pos);
            assert(dc || !limb);
            if (dc) {
                if (LimbT climb = arithmetic::umul1(std::span{ std::get<0>(result), std::get<1>(result) }, ipow<LimbT>(10, dc), limb); climb) {
                    *(std::get<0>(result) + std::get<1>(result)) = climb;
                    ++std::get<1>(result);
                }
            }
            break;
        }
        while (zcnt) {
            size_t k = (std::min)(digits_per_limb - dc, zcnt);
            dc += k;
            zcnt -= k;
            limb = limb ? limb * ipow<LimbT>(10, k) : 0;
            if (dc < digits_per_limb) break;
            if (LimbT climb = arithmetic::umul1(std::span{ std::get<0>(result), std::get<1>(result) }, big_base, limb); climb) {
                *(std::get<0>(result) + std::get<1>(result)) = climb;
                ++std::get<1>(result);
            }
            dc = 0;
            limb = 0;
        }
        limb = limb * 10 + nextlimb;
        ++dc;
        
        if (dc < digits_per_limb) continue;

        if (LimbT climb = arithmetic::umul1(std::span{ std::get<0>(result), std::get<1>(result) }, big_base, limb); climb) {
            *(std::get<0>(result) + std::get<1>(result)) = climb;
            ++std::get<1>(result);
        }
        dc = 0;
        limb = 0;
    }
    str = { pc, pce };
    return std::move(result);
}

}
