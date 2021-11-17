//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "applied/sonia_test.hpp"
#include "sonia/utility/serialization/bit_coder.hpp"
#include "sonia/utility/streaming/vector.hpp"

void bit_coder_test()
{
    using namespace sonia::serialization;

    uint8_t bitshift = 0;
    uint8_t accum = 0;
    std::vector<uint8_t> buff{0,0,0,0};

    uint8_t * it = &buff.front();
    it = bit_coder<>::encode_bits(5, 3, bitshift, accum, it);
    BOOST_CHECK_EQUAL(accum, 0xa0);
    BOOST_CHECK_EQUAL(buff, std::vector<uint8_t>({0, 0, 0, 0}));

    it = bit_coder<>::encode_bits(0xe, 2, bitshift, accum, it);
    BOOST_CHECK_EQUAL(accum, 0xb0);
    BOOST_CHECK_EQUAL(buff, std::vector<uint8_t>({0, 0, 0, 0}));

    it = bit_coder<>::encode_bits(0xff, 3, bitshift, accum, it);
    BOOST_CHECK_EQUAL(accum, 0);
    BOOST_CHECK_EQUAL(bitshift, 0);
    BOOST_CHECK_EQUAL(buff, std::vector<uint8_t>({0xb7, 0, 0, 0}));

    it = bit_coder<>::encode_bits(5, 3, bitshift, accum, it);
    BOOST_CHECK_EQUAL(accum, 0xa0);
    BOOST_CHECK_EQUAL(buff, std::vector<uint8_t>({0xb7, 0, 0, 0}));

    it = bit_coder<>::encode_bits(0xffbef, 14, bitshift, accum, it);
    BOOST_CHECK_EQUAL(accum, 0x80);
    BOOST_CHECK_EQUAL(bitshift, 1);
    BOOST_CHECK_EQUAL(buff, std::vector<uint8_t>({0xb7, 0xbd, 0xf7, 0}));

    // test compressing
    std::vector<std::vector<uint8_t>> buffs;

    for (uint32_t v = 0; v <= 0x3ffff; ++v) {
        buffs.emplace_back(16);
        uint8_t * it = &buffs.back().front();
        bitshift = accum = 0;
        it = bit_coder<>::ordered_compressed_encode_integral(v, 5, bitshift, accum, it);
        bit_coder<>::encode_bits_finalize(bitshift, accum, it);
    }
    //   check order
    for (size_t l = 0; l < buffs.size() - 1; ++l) {
        const uint8_t* lit = &buffs[l].front();
        const uint8_t* rit = &buffs[l + 1].front();
        int r = memcmp(lit, rit, 16);
        BOOST_CHECK(r < 0);

        bitshift = 0;
        uint32_t v = 0;
        bit_coder<>::ordered_compressed_decode_integral(lit, bitshift, 5, v);
        BOOST_CHECK_EQUAL(l, v);
    }

    // test compressing(2) 5 bits per order
    buffs.clear();
    for (uint32_t v = 0; v <= 0x3ffff; ++v) {
        buffs.emplace_back(16);
        uint8_t * it = &buffs.back().front();
        bitshift = accum = 0;
        it = bit_coder<>::ordered_compressed_encode_integral(v, 3, 5, bitshift, accum, it);
        bit_coder<>::encode_bits_finalize(bitshift, accum, it);
    }
    //   check order
    for (size_t l = 0; l < buffs.size() - 1; ++l) {
        const uint8_t* lit = &buffs[l].front();
        const uint8_t* rit = &buffs[l + 1].front();
        int r = memcmp(lit, rit, 16);
        BOOST_CHECK(r < 0);

        bitshift = 0;
        uint32_t v = 0;
        bit_coder<>::ordered_compressed_decode_integral(lit, bitshift, 3, 5, v);
        BOOST_CHECK_EQUAL(l, v);
    }

    // test compressing(3) adaptive
    buffs.clear();
    for (uint32_t v = 0; v <= 0x3ffff; ++v) {
        buffs.emplace_back(16);
        uint8_t * it = &buffs.back().front();
        bitshift = accum = 0;
        it = bit_coder<>::ordered_compressed_adaptive_encode_integral(v, 3, bitshift, accum, it);
        bit_coder<>::encode_bits_finalize(bitshift, accum, it);
    }
    //   check order
    for (size_t l = 0; l < buffs.size() - 1; ++l) {
        const uint8_t* lit = &buffs[l].front();
        const uint8_t* rit = &buffs[l + 1].front();
        int r = memcmp(lit, rit, 16);
        BOOST_CHECK(r < 0);

        bitshift = 0;
        uint32_t v = 0;
        bit_coder<>::ordered_compressed_adaptive_decode_integral(lit, bitshift, 3, v);
        BOOST_CHECK_EQUAL(l, v);
    }
}

void bit_coder_test_registrar()
{
    register_test(BOOST_TEST_CASE(&bit_coder_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(bit_coder_test_registrar)
#endif
