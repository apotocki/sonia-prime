//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_BIT_CODER_HPP
#define SONIA_SERIALIZATION_BIT_CODER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/integer/static_log2.hpp>

#include "sonia/cstdint.hpp"
#include "sonia/sal.hpp"
#include "serialization.hpp"

namespace sonia::serialization {

template <uint8_t BitWidthV = CHAR_BIT>
class bit_coder
{
    static constexpr uint8_t bw = BitWidthV;
    static constexpr uint64_t mask = (((uint64_t)1) << BitWidthV) - 1;
    static constexpr uint8_t bwbound = (uint8_t)1 << boost::static_log2<BitWidthV>::value;
    static constexpr uint8_t bitwidth_mask = bwbound - 1;

public:
    template <typename OutputIteratorT>
    static OutputIteratorT encode_bits_flush(uint8_t & bitshift, uint8_t & accum, OutputIteratorT oi)
    {
        if (bitshift) {
            *oi = accum; ++oi;
            bitshift = 0;
            accum = 0;
        }
        return std::move(oi);
    }

    template <typename OutputIteratorT>
    static void encode_bits_finalize(uint8_t bitshift, uint8_t accum, OutputIteratorT oi)
    {
        if (bitshift) {
            *oi = accum;
        }
    }

    template <typename OutputIteratorT>
    static OutputIteratorT encode_bits(uint64_t val, uint8_t cnt, uint8_t & bitshift, uint8_t & accum, OutputIteratorT oi)
    {
        BOOST_ASSERT(bitshift < bw);
        uint8_t bits_in_cur_res = bw - bitshift;
        uint64_t first_mask = ((((uint64_t)1) << bits_in_cur_res) - 1);
        if (cnt < bits_in_cur_res) {
            accum |= (val << (bits_in_cur_res - cnt)) & first_mask;
            bitshift += cnt;
        } else {
            bitshift = (bitshift + cnt) % bw;
            uint8_t offs = cnt - bits_in_cur_res;
            accum |= (val >> offs) & first_mask;
            *oi = accum; ++oi;
            cnt -= bits_in_cur_res;
            while (cnt) {
                if (cnt >= bw) {
                    *oi = (val >> (cnt - bw)) & mask; ++oi;
                    cnt -= bw;
                } else {
                    accum = (val << (bw - cnt)) & mask;
                    return std::move(oi);
                }
            }
            accum = 0;
        }
        return std::move(oi);
    }

    template <typename InputIteratorT, typename T>
    static InputIteratorT decode_bits(InputIteratorT ii, uint8_t & bitshift, uint8_t cnt, T & val)
    {
        BOOST_ASSERT(bitshift < bw);
        uint8_t bits_in_cur_res = bw - bitshift;
        uint64_t first_mask = ((((uint64_t)1) << bits_in_cur_res) - 1);
        if (cnt < bits_in_cur_res) {
            val = static_cast<T>((*ii & first_mask) >> (bits_in_cur_res - cnt));
            bitshift += cnt;
        } else {
            bitshift = (bitshift + cnt) % bw;
            uint8_t offs = cnt - bits_in_cur_res;
            val = (*ii & first_mask) << offs; ++ii;
            cnt -= bits_in_cur_res;
            while (cnt) {
                if (cnt >= bw) {
                    val |= static_cast<T>(*ii) << (cnt - bw); ++ii;
                    cnt -= bw;
                } else {
                    val |= static_cast<T>(*ii) >> (bw - cnt);
                    break;
                }
            }
        }
        return std::move(ii);
    }

    /**
        order = x ? log2(x) : 0
        0, 1    - encoded as {order}, {0|1}
        2 - max - encoded as {order}, {value bits except first left bit that is always = 1}
    */
    template <typename T, typename OutputIteratorT>
    static enable_if_t<is_integral_v<T> && is_unsigned_v<T>, OutputIteratorT>
        ordered_compressed_encode_integral(T value, uint8_t order_bit_cnt, uint8_t & bitshift, uint8_t & accum, OutputIteratorT oi)
    {
        if (order_bit_cnt) {
            BOOST_ASSERT(value <= (((((uint64_t)1) << ((1 << order_bit_cnt) - 1)) - 1) << 1) + 1);
        } else {
            order_bit_cnt = boost::static_log2<sizeof(T) * CHAR_BIT>::value;
        }
        if (value < 4) {
            return encode_bits(value, order_bit_cnt + 1, bitshift, accum, std::move(oi));
        } else {
            auto bits = sonia::sal::log2(value);
            oi = encode_bits(bits, order_bit_cnt, bitshift, accum, std::move(oi));
            return encode_bits(value, bits, bitshift, accum, std::move(oi));
        }
    }

    template <typename T, typename InputIteratorT>
    static enable_if_t<is_integral_v<T> && is_unsigned_v<T>, InputIteratorT>
        ordered_compressed_decode_integral(InputIteratorT ii, uint8_t & bitshift, uint8_t order_bit_cnt, T & value)
    {
        if (!order_bit_cnt) {
            order_bit_cnt = boost::static_log2<sizeof(T) * CHAR_BIT>::value;
        }
        uint64_t tmp = 0;
        ii = decode_bits(std::move(ii), bitshift, order_bit_cnt + 1, tmp);
        if (tmp < 4) {
            value = static_cast<T>(tmp);
        } else {
            uint8_t bits = static_cast<uint8_t>(tmp >> 1);
            ii = decode_bits(std::move(ii), bitshift, bits - 1, value);
            value |= (((tmp | 2) & 3) << (bits - 1));
        }
        return std::move(ii);
    }

    template <typename T, typename OutputIteratorT>
    static enable_if_t<is_integral_v<T> && is_unsigned_v<T>, OutputIteratorT>
        ordered_compressed_encode_integral(T value, uint8_t order_bit_cnt, uint8_t bits_per_order, uint8_t & bitshift, uint8_t & accum, OutputIteratorT oi)
    {
        BOOST_ASSERT(order_bit_cnt && bits_per_order);
        auto bits = value ? sonia::sal::log2(value) + 1 : 1;
        auto order = (bits + bits_per_order - 1) / bits_per_order;
        auto usebits = order * bits_per_order;
        
        if (BOOST_UNLIKELY(order > ((uint8_t)1 << order_bit_cnt))) {
            THROW_INTERNAL_ERROR("ordered_compressed_encode_integral : value (%1%) is too big to fit storage"_fmt % value);
        }
        oi = encode_bits(order - 1, order_bit_cnt, bitshift, accum, std::move(oi));
        return encode_bits(value, usebits, bitshift, accum, std::move(oi));
    }

    template <typename T, typename InputIteratorT>
    static enable_if_t<is_integral_v<T> && is_unsigned_v<T>, InputIteratorT>
        ordered_compressed_decode_integral(InputIteratorT ii, uint8_t & bitshift, uint8_t order_bit_cnt, uint8_t bits_per_order, T & value)
    {
        BOOST_ASSERT(order_bit_cnt && bits_per_order);
        uint64_t tmp = 0;
        ii = decode_bits(std::move(ii), bitshift, order_bit_cnt + bits_per_order, tmp);
        if (tmp < ((uint64_t)1 << bits_per_order)) {
            value = static_cast<T>(tmp);
        } else {
            uint8_t leftbits = bits_per_order * static_cast<uint8_t>(tmp >> bits_per_order);
            ii = decode_bits(std::move(ii), bitshift, leftbits, value);
            value |= (tmp & ((1 << bits_per_order) - 1)) << leftbits;
        }
        return std::move(ii);
    }

    // adapt first order bit width to fit last byte, other orders have bw bits
    template <typename T, typename OutputIteratorT>
    static enable_if_t<is_integral_v<T> && is_unsigned_v<T>, OutputIteratorT>
        ordered_compressed_adaptive_encode_integral(T value, uint8_t order_bit_cnt, uint8_t & bitshift, uint8_t & accum, OutputIteratorT oi)
    {
        BOOST_ASSERT(order_bit_cnt);
        uint8_t first_order_bits = (2 * bw - bitshift - order_bit_cnt) % bw;
        first_order_bits += first_order_bits ? 0 : bw;

        auto bits = value ? sonia::sal::log2(value) + 1 : 1;
        auto order = (bits <= first_order_bits) ? 0 : (bits - first_order_bits + bw - 1) / bw;
        auto usebits = first_order_bits + order * bw;

        if (BOOST_UNLIKELY(order >= ((uint8_t)1 << order_bit_cnt))) {
            throw internal_error("ordered_compressed_adaptive_encode_integral : value (%1%) is too big to fit storage"_fmt % value);
        }
        oi = encode_bits(order, order_bit_cnt, bitshift, accum, std::move(oi));
        return encode_bits(value, usebits, bitshift, accum, std::move(oi));
    }

    template <typename T, typename InputIteratorT>
    static enable_if_t<is_integral_v<T> && is_unsigned_v<T>, InputIteratorT>
        ordered_compressed_adaptive_decode_integral(InputIteratorT ii, uint8_t & bitshift, uint8_t order_bit_cnt, T & value)
    {
        BOOST_ASSERT(order_bit_cnt);
        uint8_t first_order_bits = (2 * bw - bitshift - order_bit_cnt) % bw;
        first_order_bits += first_order_bits ? 0 : bw;

        uint64_t tmp = 0;
        ii = decode_bits(std::move(ii), bitshift, order_bit_cnt + first_order_bits, tmp);
        if (tmp < ((uint64_t)1 << first_order_bits)) {
            value = static_cast<T>(tmp);
        } else {
            uint8_t leftbits = bw * static_cast<uint8_t>(tmp >> first_order_bits);
            ii = decode_bits(std::move(ii), bitshift, leftbits, value);
            value |= (tmp & (((uint64_t)1 << first_order_bits) - 1)) << leftbits;
        }
        return std::move(ii);
    }

    ///**
    //instead of order we store the number of additional bytes to store the given value,
    //so encoding can take us less space
    //*/
    //template <typename T, typename OutputIteratorT>
    //enable_if_t<is_integral_v<T> && is_unsigned_v<T>, OutputIteratorT>
    //    compressed_encode_integral(T value, uint8_t byte_order_bit_cnt, uint8_t bitshift, uint8_t accum, OutputIteratorT oi) const
    //{
    //    BOOST_ASSERT(bitshift < bw);
    //    uint8_t bits_in_cur_res = bw - bitshift;
    //    if (bits_in_cur_res > byte_order_bit_cnt) {
    //        if (BOOST_UNLIKELY(!value)) {
    //            return encode_bits(0, bits_in_cur_res, bitshift, accum, std::move(oi));
    //        }
    //        uint8_t valbits_in_first_byte = bits_in_cur_res - byte_order_bit_cnt;
    //        auto bits = sonia::sal::log2(value) + 1;
    //        if (BOOST_UNLIKELY(bits <= valbits_in_first_byte)) {
    //            return encode_bits(0, bits_in_cur_res, bitshift, accum, std::move(oi));
    //        }
    //    }
    //    if (value < 4) {
    //}

};

}

#endif // SONIA_SERIALIZATION_BIT_CODER_HPP
