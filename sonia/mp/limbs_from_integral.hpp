/*=============================================================================
    Copyright (c) Alexander Pototskiy

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include <concepts>
#include <type_traits>
#include <limits>
#include <memory>
#include <tuple>
#include <span>

namespace sonia::mp {

template <std::unsigned_integral LimbT, std::integral T, size_t N>
requires(N == std::dynamic_extent || N >= (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT))
std::tuple<size_t, int> to_limbs(T value, std::span<LimbT, N> sp) noexcept
{
    std::make_unsigned_t<T> absval = value;
    int sign = 1;
    if constexpr (std::is_signed_v<T>) {
        if (value < 0) {
            absval = ~absval + 1;
            sign = -1;
        }
    }

    constexpr size_t limbs_cnt = (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT);
    if constexpr (N == std::dynamic_extent) {
        if (sp.empty()) return { 0, sign };
    }
    sp.front() = static_cast<LimbT>(absval);
    size_t cnt = 1;
    if constexpr (limbs_cnt > 1) {
        absval >>= std::numeric_limits<LimbT>::digits;
        for (; !!absval && cnt < (std::min)(limbs_cnt, sp.size()); ++cnt) {
            sp[cnt] = static_cast<LimbT>(absval);
            absval >>= std::numeric_limits<LimbT>::digits;
        }
    }
    
    return { cnt, sign };
}

template <std::unsigned_integral LimbT, std::integral T, typename AllocatorT>
requires(std::is_same_v<LimbT, typename std::allocator_traits<std::remove_cvref_t<AllocatorT>>::value_type>)
std::tuple<LimbT*, size_t, size_t, int> to_limbs(T value, AllocatorT&& alloc)
{
    using alloc_traits_t = std::allocator_traits<std::remove_cvref_t<AllocatorT>>;
    constexpr size_t limbs_cnt = (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT);

    LimbT* buf = alloc_traits_t::allocate(alloc, limbs_cnt);
    auto [sz, sign] = to_limbs(value, std::span{ buf, limbs_cnt });
    return { buf, sz, limbs_cnt, sign };
}

}
