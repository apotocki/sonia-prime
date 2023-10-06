//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <cstdint>
#include <limits>
#include <concepts>
#include <stdexcept>

namespace sonia {

using std::int8_t;
using std::uint8_t;

using std::int16_t;
using std::uint16_t;

using std::int32_t;
using std::uint32_t;

using std::int64_t;
using std::uint64_t;

using std::intmax_t;
using std::uintmax_t;

using std::int_least16_t;
using std::int_least16_t;

using std::int_least32_t;
using std::uint_least32_t;

using std::int_least64_t;
using std::uint_least64_t;

using std::int_fast16_t;
using std::uint_fast16_t;

using std::int_fast32_t;
using std::uint_fast32_t;

using std::int_fast64_t;
using std::uint_fast64_t;

using float_t = float;
using double_t = long double;

template <std::integral DestT, std::integral SrcT>
inline DestT integral_cast(SrcT val)
{
    if constexpr (
        (std::numeric_limits<DestT>::min)() > (std::numeric_limits<SrcT>::min)() ||
        (std::numeric_limits<DestT>::max)() < (std::numeric_limits<SrcT>::max)()
    )
    {
        if constexpr (std::is_signed_v<SrcT>) {
            if (val < (std::numeric_limits<DestT>::min)()) {
                throw std::out_of_range("out of min bound");
            }
        }
        if (val > (std::numeric_limits<DestT>::max)()) {
            throw std::out_of_range("out of max bound");
        }
    }
    return static_cast<DestT>(val);
}

}
