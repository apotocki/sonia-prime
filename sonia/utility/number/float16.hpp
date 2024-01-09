//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <cstdint>
#include <ostream>

namespace sonia {

struct float16
{
    uint16_t data;

    float16() = default;
    explicit float16(uint16_t val) noexcept : data{ val } { }
    explicit float16(float) noexcept;

    explicit operator float() const noexcept;
};

inline bool operator== (float16 const& l, float16 const& r) { return l.data == r.data; }

template <typename E, typename T>
inline std::basic_ostream<E, T>& operator<<(std::basic_ostream<E, T>& os, float16 const& v)
{
    return os << (float)v;
}

inline float16::float16(float f32val) noexcept
{
    uint32_t f32data = std::bit_cast<uint32_t>(f32val);
#ifndef __F16CINTRIN_H
    const uint32_t frac32 = f32data & 0x7fffff;
    const uint8_t exp32 = f32data >> 23;
    const int8_t exp32_diff = exp32 - 127;

    uint16_t exp16 = 0;
    uint16_t frac16 = frac32 >> 13;

    if (exp32 == 0xff || exp32_diff > 15) {
        exp16 = 0x1f;
    } else if (!exp32 || exp32_diff < -14) {
        exp16 = 0;
    } else {
        exp16 = exp32_diff + 15;
    }

    if (exp32 == 0xff && frac32 && !frac16) {
        frac16 = 0x200;
    } else if (!exp32 || (exp16 == 0x1f && exp32 != 0xff)) {
        frac16 = 0;
    } else if (!exp16 && exp32) {
        frac16 = (0x200 | (frac16 >> 1)) >> (17 - exp32_diff);
    }

    data = (f32data & 0x80000000) >> 16; // sign
    data |= exp16 << 10;
    data |= frac16;
#else
    data = _cvtss_sh(val, 0);
#endif
}

inline float16::operator float() const noexcept
{
#ifndef __F16CINTRIN_H
    uint32_t exponent = (data >> 10) & 0x1F;
    uint32_t mantissa = (data & 0x3FF);
    
    uint32_t f32data;

    if (exponent == 0x1f) {
        f32data = 0xff;
    } else if (!exponent) {
        f32data = 0;
    } else {
        f32data = exponent + 112;
    }

    if (!exponent && mantissa) {
        uint8_t offset = 0;
        do {
            ++offset;
            mantissa <<= 1;
        } while (!(mantissa & 0x400));
        mantissa &= 0x3ff;
        f32data = 113 - offset;
    }

    f32data <<= 23;
    f32data |= static_cast<uint32_t>(data & 0x8000) << 16;
    f32data |= (mantissa << 13);

    return std::bit_cast<float>(f32data);
#else
    return _cvtsh_ss(data);
#endif

}

}
