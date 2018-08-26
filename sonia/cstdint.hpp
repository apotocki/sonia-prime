//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_CSTDINT_HPP
#define SONIA_CSTDINT_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <cstdint>

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

typedef float float_t;
typedef long double double_t;

}

#endif // SONIA_CSTDINT_HPP
