//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SAL_HPP
#define SONIA_SAL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#ifdef _MSC_VER
#   include <Intrin.h>
#endif

#include "cstdint.hpp"
#include "string.hpp"
#include "concurrency.hpp"
#include "shared_ptr.hpp"
#include "sal_types.hpp"
#include "sonia/services/bundle.hpp"

// system abstract level functions

namespace sonia::sal {

void set_thread_name(thread::id, string_view name);
void set_thread_name(thread::id, std::string const& name);

int get_pid();

shared_ptr<sonia::services::bundle> load_bundle(sonia::services::bundle_configuration const&);

// file routine
uint64_t file_size(file_handle_type h);
void delete_file(cstring_view path);


#ifdef __GNUC__
inline uint8_t log2(uint64_t x)
{
    BOOST_ASSERT(x);
    return (uint8_t)(8 * sizeof(unsigned long long) - __builtin_clzll((x)) - 1);
}
#elif _MSC_VER
inline uint8_t log2(uint64_t x)
{
    BOOST_ASSERT(x);
    unsigned long log2;
    _BitScanReverse64(&log2, x);
    return (uint8_t)log2;
}
#else
const uint8_t log2_tab64[64] = {
    63,  0, 58,  1, 59, 47, 53,  2,
    60, 39, 48, 27, 54, 33, 42,  3,
    61, 51, 37, 40, 49, 18, 28, 20,
    55, 30, 34, 11, 43, 14, 22,  4,
    62, 57, 46, 52, 38, 26, 32, 41,
    50, 36, 17, 19, 29, 10, 13, 21,
    56, 45, 25, 31, 35, 16,  9, 12,
    44, 24, 15,  8, 23,  7,  6,  5};

inline uint8_t log2(uint64_t x)
{
    BOOST_ASSERT(x);

    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    return log2_tab64[((uint64_t)((x - (x >> 1))*0x07EDD5E59A4E28C2)) >> 58];
}
#endif

}

#endif // SONIA_SAL_HPP
