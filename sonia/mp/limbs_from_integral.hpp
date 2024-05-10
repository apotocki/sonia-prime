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
requires(N >= (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT))
std::tuple<size_t, int> to_limbs(T value, std::span<LimbT, N> sp) noexcept
{
    T absval;
    int sign;
    if constexpr (std::is_signed_v<T>) {
        absval = std::abs(value);
        sign = value < 0 ? -1 : 1;
    } else {
        absval = value;
        sign = 1;
    }

    constexpr size_t limbs_cnt = (sizeof(T) + sizeof(LimbT) - 1) / sizeof(LimbT);

    sp.front() = static_cast<LimbT>(absval);
    size_t cnt = 1;
    if constexpr (limbs_cnt > 1) {
        absval >>= std::numeric_limits<LimbT>::digits;
        for (; !!absval && cnt < limbs_cnt; ++cnt) {
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
