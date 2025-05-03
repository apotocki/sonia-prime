//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#define TEST_FOLDER "mp_test"

#include "sonia/config.hpp"
#undef BOOST_ENABLE_ASSERT_HANDLER

#include "applied/sonia_test.hpp"

#include "sonia/mp/basic_integer.hpp"

#include <vector>
#include <iostream>
//#include <boost/container/small_vector.hpp>

void basic_integer_test0()
{
    static_assert(sizeof(sonia::mp::basic_integer<uint8_t, 1>) == sizeof(void*));

    static_assert(sonia::mp::basic_integer<uintptr_t, 1>::storage_type::limb_count_bits == 0);
    static_assert(sonia::mp::basic_integer<uintptr_t, 2>::storage_type::limb_count_bits == 1);
    static_assert(sonia::mp::basic_integer<uintptr_t, 3>::storage_type::limb_count_bits == 2);
    static_assert(sonia::mp::basic_integer<uintptr_t, 4>::storage_type::limb_count_bits == 2);
    static_assert(sonia::mp::basic_integer<uintptr_t, 5>::storage_type::limb_count_bits == 3);

    using bint_t = sonia::mp::basic_integer<uint64_t, 1>;
    using biv_t = sonia::mp::basic_integer_view<uint64_t>;
    
    using namespace std::string_view_literals;

    BOOST_CHECK(!bint_t{});
    BOOST_CHECK(!(biv_t)bint_t{});
    BOOST_CHECK(bint_t{1});
    BOOST_CHECK((biv_t)bint_t{ 1 });
    BOOST_CHECK(*bint_t{}.raw_data() == (uint64_t{ 1 } << 63)); // in_place_mask
    BOOST_CHECK(*bint_t{ 0 }.raw_data() == (uint64_t{ 1 } << 63)); // in_place_mask
    BOOST_CHECK(*bint_t{ 1 }.raw_data() == (uint64_t{ 1 } << 63) + 1); // in_place_mask + 1
    BOOST_CHECK(*bint_t{ -1 }.raw_data() == (uint64_t{ 3 } << 62) + 1); // in_place_mask | sign_mask + 1
    BOOST_CHECK(*bint_t{ 0x3fffFFFFffffFFFFll }.raw_data() == uint64_t{ 0xbfffFFFFffffFFFFll }); // max inplace value
    BOOST_CHECK(*bint_t{ -0x3fffFFFFffffFFFFll }.raw_data() == uint64_t{ 0xffffFFFFffffFFFFll }); // min inplace value
    BOOST_CHECK(bint_t{ 0x3fffFFFFffffFFFFll } == bint_t::inplace_max());
    BOOST_CHECK(bint_t{ -0x3fffFFFFffffFFFFll } == bint_t::inplace_min());
    BOOST_CHECK(!bint_t{ 0x4000000000000000ll }.is_inplaced());
    BOOST_CHECK(!bint_t{ -0x4000000000000000ll }.is_inplaced());
    BOOST_CHECK_EQUAL(-bint_t::inplace_max(), bint_t::inplace_min());
    BOOST_CHECK_EQUAL(-bint_t::inplace_min(), bint_t::inplace_max());
    BOOST_CHECK_EQUAL(-bint_t{ 0x4000000000000000ll }, bint_t{ -0x4000000000000000ll });
    BOOST_CHECK_EQUAL((uint64_t)bint_t { -0x4000000000000000ll }, -0x4000000000000000ll);

    
    BOOST_CHECK(*bint_t{ "0"sv }.raw_data() == (uint64_t{ 1 } << 63));
    BOOST_CHECK(*bint_t{ "1"sv }.raw_data() == (uint64_t{ 1 } << 63) + 1);
    BOOST_CHECK(*bint_t{ "-1"sv }.raw_data() == (uint64_t{ 3 } << 62) + 1);
    BOOST_CHECK(*bint_t{ "0x3fffFFFFffffFFFF"sv }.raw_data() == uint64_t{ 0xbfffFFFFffffFFFFll }); // max inplace value
    BOOST_CHECK(*bint_t{ "-0x3fffFFFFffffFFFF"sv }.raw_data() == uint64_t{ 0xffffFFFFffffFFFFll }); // min inplace value
    BOOST_CHECK(!bint_t{ "0x4000000000000000"sv }.is_inplaced());
    BOOST_CHECK(!bint_t{ "-0x4000000000000000"sv }.is_inplaced());
    BOOST_CHECK(bint_t{ "0"sv } == bint_t{});
    BOOST_CHECK(bint_t{ "-0x3fffFFFFffffFFFF"sv } == bint_t{ -0x3fffFFFFffffFFFFll });
    BOOST_CHECK(bint_t{ "-0x3fffFFFFffffFFFF"sv } == -0x3fffFFFFffffFFFFll);
    BOOST_CHECK(-0x3fffFFFFffffFFFFll == bint_t{ "-0x3fffFFFFffffFFFF"sv });
    BOOST_CHECK(bint_t{ "-0x3fffFFFFffffFFFF"sv } > bint_t{ "-0x4000000000000000"sv });
    BOOST_CHECK(bint_t{ "-0x3fffFFFFffffFFFF"sv } > -0x4000000000000000ll);
    BOOST_CHECK(-0x4000000000000000ll < bint_t{ "-0x3fffFFFFffffFFFF"sv });
    BOOST_CHECK(bint_t{ "0xFFFF3fffFFFFffffFFFF"sv } > bint_t{ "0xFFFF2fffFFFFffffFFFF"sv });
    BOOST_CHECK(bint_t{ "-0x3fffFFFFffffFFFF"sv } != bint_t{ "-0x4000000000000000"sv });
    BOOST_CHECK(bint_t{ "0xFFFF3fffFFFFffffFFFF"sv } != bint_t{ "0xFFFF2fffFFFFffffFFFF"sv });
    BOOST_CHECK(bint_t{ "0xFFFF7fffFFFFffffFFFF"sv } == bint_t{ "0xFFFF7fffFFFFffffFFFF"sv });

    using bint2_t = sonia::mp::basic_integer<uint32_t, 1>;
    static_assert(sizeof(bint2_t) == sizeof(void*));
    auto check_ui32s = [](uint32_t const* vals, std::initializer_list<uint32_t> ref) {
        return std::ranges::equal(std::span{ vals, ref.size() }, ref);
    };
    BOOST_CHECK(check_ui32s(bint2_t{}.raw_data(), {0, uint32_t{ 1 } << 31 }));
    BOOST_CHECK(check_ui32s(bint2_t{ 0 }.raw_data(), { 0, uint32_t{ 1 } << 31 }));
    BOOST_CHECK(check_ui32s(bint2_t{ 1 }.raw_data(), { 1, uint32_t{ 1 } << 31 }));
    BOOST_CHECK(check_ui32s(bint2_t{ -1 }.raw_data(), { 1, uint32_t{ 3 } << 30 }));
    BOOST_CHECK(check_ui32s(bint2_t{ 0x1fffFFFFffffFFFFll }.raw_data(), { 0xffffffffu, 0xbfffFFFFu })); // max inplace value
    BOOST_CHECK(check_ui32s(bint2_t{ -0x1fffFFFFffffFFFFll }.raw_data(), { 0xffffffffu, 0xffffFFFFu })); // min inplace value
    BOOST_CHECK(!bint2_t{ 0x2000000000000000ll }.is_inplaced());
    BOOST_CHECK(!bint2_t{ -0x2000000000000000ll }.is_inplaced());
    BOOST_CHECK(bint2_t{ 0x1fffFFFFffffFFFFll } == bint2_t::inplace_max());
    BOOST_CHECK(bint2_t{ -0x1fffFFFFffffFFFFll } == bint2_t::inplace_min());

    // copy with different inplace sizes
    {
        sonia::mp::basic_integer<uint64_t, 1> max1 = sonia::mp::basic_integer<uint64_t, 1>::inplace_max();
        sonia::mp::basic_integer<uint64_t, 2> v0{max1};
        BOOST_CHECK_EQUAL(v0, max1);
    }
    {
        sonia::mp::basic_integer<uint64_t, 2> max1 = sonia::mp::basic_integer<uint64_t, 2>::inplace_max();
        sonia::mp::basic_integer<uint64_t, 1> v0{ max1 };
        BOOST_CHECK_EQUAL(v0, max1);
    }

    using bint3_t = sonia::mp::basic_integer<uint32_t, 3>;
    static_assert(sizeof(bint3_t) == 2 * sizeof(void*));
    BOOST_CHECK_EQUAL(to_string(bint3_t{}), "0");
    BOOST_CHECK_EQUAL(to_string(bint3_t{ 0 }), "0");
    BOOST_CHECK_EQUAL(to_string(bint3_t{ 1 }), "1");
    BOOST_CHECK_EQUAL(to_string(bint3_t{ -1 }), "-1");
    BOOST_CHECK(bint3_t{ 1 }.is_inplaced());
    BOOST_CHECK_EQUAL(to_string(bint3_t{ 0x7fffFFFFffffFFFFll }, 16), "0x7fffffffffffffff");
    BOOST_CHECK(bint3_t{ 0xffffFFFFffffFFFFll }.is_inplaced());
    BOOST_CHECK_EQUAL(to_string(bint3_t{ "0x0fffFFFFffffFFFFffffFFFFffffFFFF"sv }, 16), "0xfffffffffffffffffffffffffffffff");
    BOOST_CHECK(bint3_t{ "0x0fffFFFFffffFFFFffffFFFFffffFFFF"sv }.is_inplaced());
    BOOST_CHECK_EQUAL(to_string(bint3_t{ "-0x0fffFFFFffffFFFFffffFFFFffffFFFF"sv }, 16), "-0xfffffffffffffffffffffffffffffff");
    BOOST_CHECK(bint3_t{ "-0x0fffFFFFffffFFFFffffFFFFffffFFFF"sv }.is_inplaced());
    BOOST_CHECK_EQUAL(to_string(bint3_t{ "0x10000000000000000000000000000000"sv }, 16), "0x10000000000000000000000000000000");
    BOOST_CHECK(!bint3_t{ "0x10000000000000000000000000000000"sv }.is_inplaced());
    BOOST_CHECK_EQUAL(to_string(bint3_t{ "-0x10000000000000000000000000000000"sv }, 16), "-0x10000000000000000000000000000000");
    BOOST_CHECK(!bint3_t{ "-0x10000000000000000000000000000000"sv }.is_inplaced());

    {
        bint_t b0{ "0x3fffFFFFffffFFFF"sv };
        bint_t b1{ b0 };
        BOOST_CHECK(*b1.raw_data() == uint64_t{ 0xbfffFFFFffffFFFFll }); // max inplace value
        BOOST_CHECK_EQUAL(to_string(b1), "4611686018427387903");
        BOOST_CHECK_EQUAL(to_string(b1, 16), "0x3fffffffffffffff");
        BOOST_CHECK_EQUAL(to_string(b1, 8), "0377777777777777777777");
        bint_t b2{ std::move(b0) };
        BOOST_CHECK(b0.is_inplaced());
        BOOST_CHECK_EQUAL(to_string(b2), "4611686018427387903");
    }
    {
        bint_t b0{ "0x4000000000000000"sv };
        bint_t b1{ b0 };
        BOOST_CHECK(!b1.is_inplaced());
        BOOST_CHECK_EQUAL(to_string(b1), "4611686018427387904");
        BOOST_CHECK_EQUAL(to_string(b1, 16), "0x4000000000000000");
        bint_t b11{ std::move(b0) };
        BOOST_CHECK(b0.is_inplaced());
        BOOST_CHECK_EQUAL(to_string(b11), "4611686018427387904");

        bint_t b2{ "-4611686018427387904"sv };
        bint_t b3{ b2 };
        BOOST_CHECK_EQUAL(to_string(b3), "-4611686018427387904");
        BOOST_CHECK_EQUAL(to_string(b3, 16), "-0x4000000000000000");
        bint_t b21{ std::move(b2) };
        BOOST_CHECK(b2.is_inplaced());
        BOOST_CHECK_EQUAL(to_string(b21), "-4611686018427387904");
        b2 = -4611686018427387904ll;
        BOOST_CHECK(!b2.is_inplaced());
        BOOST_CHECK_EQUAL(to_string(b2), "-4611686018427387904");
        bint_t b4 = -4611686018427387904ll;
        BOOST_CHECK(!b4.is_inplaced());
        BOOST_CHECK_EQUAL(to_string(b4), "-4611686018427387904");
    }

    // swap tests
    {
        using std::swap;
        bint_t b0{ "-0x3fffFFFFffffFFFF"sv };
        bint_t b1;
        swap(b0, b1);
        BOOST_CHECK_EQUAL(to_string(b1, 16), "-0x3fffffffffffffff");
        BOOST_CHECK_EQUAL(to_string(b0), "0");
        swap(b0, b1);
        BOOST_CHECK_EQUAL(to_string(b0, 16), "-0x3fffffffffffffff");
        BOOST_CHECK_EQUAL(to_string(b1), "0");
    }

    {
        using std::swap;
        bint_t b0{ "-0x4000000000000000"sv };
        bint_t b1;
        swap(b0, b1);
        BOOST_CHECK_EQUAL(to_string(b1, 16), "-0x4000000000000000");
        BOOST_CHECK_EQUAL(to_string(b0), "0");
        swap(b0, b1);
        BOOST_CHECK_EQUAL(to_string(b0, 16), "-0x4000000000000000");
        BOOST_CHECK_EQUAL(to_string(b1), "0");
    }

    {
        using std::swap;
        bint2_t b0{ "-0x2000000000000000"sv };
        bint2_t b1;
        swap(b0, b1);
        BOOST_CHECK_EQUAL(to_string(b1, 16), "-0x2000000000000000");
        BOOST_CHECK_EQUAL(to_string(b0), "0");
        swap(b0, b1);
        BOOST_CHECK_EQUAL(to_string(b0, 16), "-0x2000000000000000");
        BOOST_CHECK_EQUAL(to_string(b1), "0");
    }


    using namespace sonia::mp::literals;
    {
        {
            bint_t b0 = 10_bi + 0x20_bi;
            BOOST_CHECK(b0.is_inplaced());
            BOOST_CHECK_EQUAL(b0, 42);
        }
        {
            bint_t b0 = "10"_bi - "20"_bi;
            BOOST_CHECK(b0.is_inplaced());
            BOOST_CHECK_EQUAL(b0, -10);
        }

        BOOST_CHECK_EQUAL(0111_bi, 73);

        BOOST_CHECK_EQUAL("0x3fffFFFFffffFFFF"_bi + 1, 0x4000000000000000);
        BOOST_CHECK_EQUAL("-0xffffFFFFffffFFFF"_bi + "-340282366920938463408034375210639556610"_bi, "-340282366920938463426481119284349108225"_bi);
        BOOST_CHECK_EQUAL("340282366920938463408034375210639556610"_bi + 0xffffFFFFffffFFFFll, "340282366920938463426481119284349108225"_bi);
        BOOST_CHECK_EQUAL(0xffffFFFFffffFFFFll + "340282366920938463408034375210639556610"_bi, "340282366920938463426481119284349108225"_bi);
        BOOST_CHECK_EQUAL("340282366920938463408034375210639556610"_bi += 0xffffFFFFffffFFFFll, "340282366920938463426481119284349108225"_bi);

        BOOST_CHECK_EQUAL("-0xffffFFFFffffFFFF"_bi - "-340282366920938463408034375210639556610"_bi, "340282366920938463389587631136930004995"_bi);
        BOOST_CHECK_EQUAL("-340282366920938463408034375210639556610"_bi - 0xffffFFFFffffFFFFll, "-340282366920938463426481119284349108225"_bi);
        BOOST_CHECK_EQUAL("-340282366920938463408034375210639556610"_bi -= 0xffffFFFFffffFFFFll, "-340282366920938463426481119284349108225"_bi);
        BOOST_CHECK_EQUAL(0xffffFFFFffffFFFFll - "340282366920938463408034375210639556610"_bi, "-340282366920938463389587631136930004995"_bi);

        BOOST_CHECK_EQUAL("-0xffffFFFFffffFFFF"_bi * "0xffffFFFFffffFFFE"_bi, "-340282366920938463408034375210639556610"_bi);
        BOOST_CHECK_EQUAL("-0xffffFFFFffffFFFF"_bi * 0xffffFFFFffffFFFEll, "-340282366920938463408034375210639556610"_bi);
        BOOST_CHECK_EQUAL(0xffffFFFFffffFFFEll * "-0xffffFFFFffffFFFF"_bi, "-340282366920938463408034375210639556610"_bi);
        BOOST_CHECK_EQUAL("-0xffffFFFFffffFFFF"_bi *= 0xffffFFFFffffFFFEll, "-340282366920938463408034375210639556610"_bi);

        BOOST_CHECK_EQUAL("-340282366920938463408034375210639556610"_bi / "0xffffFFFFffffFFFE"_bi, "-0xffffFFFFffffFFFF"_bi);
        BOOST_CHECK_EQUAL("-340282366920938463408034375210639556610"_bi / 0xffffFFFFffffFFFEll, "-0xffffFFFFffffFFFF"_bi);
        BOOST_CHECK_EQUAL("340282366920938463408034375210639556610"_bi /= 0xffffFFFFffffFFFEll, 0xffffFFFFffffFFFFll);
        BOOST_CHECK_EQUAL(1082152022374638ull / "12345678"_bi, "87654321"_bi);
        BOOST_CHECK_EQUAL("2193998782732"_bi / 10000000000000000000ul, 0);
        BOOST_CHECK_EQUAL("219399878273287837459238450239485023985748738458787"_bi / 10000000000000000000ul / 10000000000000000000ul / 10000000000000000000ul, 0);

        BOOST_CHECK_EQUAL("340282366920938463408034375210639556610"_bi % "0xfffE"_bi, "210"_bi);
        BOOST_CHECK_EQUAL("340282366920938463408034375210639556610"_bi % 0xfffE, "210"_bi);
        BOOST_CHECK_EQUAL("340282366920938463408034375210639556610"_bi * "340282366920938463408034375210639556610"_bi, "115792089237316195385908374596367823274678918896366765567645960308857394692100"_bi);

        BOOST_CHECK_EQUAL(340282366920938463408034375210639556610_bi * 340282366920938463408034375210639556610_bi, 115792089237316195385908374596367823274678918896366765567645960308857394692100_bi);


        auto x0 = "5"_bi;
        BOOST_CHECK_EQUAL(x0* x0, (uint64_t)25);
        BOOST_CHECK("26"_bi > (x0 * x0));
        BOOST_CHECK(26 > (x0 * x0));
        BOOST_CHECK(26 != (x0 * x0));
        BOOST_CHECK((x0 * x0) > 24);
        //BOOST_CHECK_EQUAL(sonia::mp::pow("340282366920938463408034375210639556610"_bi, 0xf), "94971145180789141173792039356877348546615710136820159429620005475820741373484440733711065254441078508238038934605050990664553807873876669074305644271758756235065228100642097755554173541883690838600982589611688150716320040505871847239934643409009493854662453165945031933545041297089845350845567778753783952870388913863486884966424088583811509592259257648204772815239748003247381690020399370551715721516812596449220997661037663386512666647739896418940688124424283266194059250293267995087980181863547749199304889944124217666383500994250505363539943187173672572952901000000000000000"_bi);
    }
    bint_t val = -65536;
    BOOST_CHECK(val == -65536);
    BOOST_CHECK(val < -65535);
    BOOST_CHECK(val < 0);
    BOOST_CHECK_EQUAL((int8_t)"-65536"_bi, int8_t(0));
    BOOST_CHECK_EQUAL((int8_t)"-65535"_bi, int8_t(1));


    // 
    //     auto* pp = bint_t{ -1 }.raw_data();
    //std::cout << std::hex << sonia::mp::basic_integer<uintptr_t, 3>::last_limb_mask << "\n";

    //constexpr bint_t val0; // { std::allocator<uint8_t>{} };

    //size_t isz = sizeof(bint_t);
    //std::cout << isz << "\n" << std::hex << bint_t::limb_count_bits << "\n";
    //int_t val0{16};
    //val0 = val0 * 16 + 1;
    //BOOST_CHECK_EQUAL(257, (int)val0);
    //int_t val1{ 32 };
    //val0.swap(val1);
    //BOOST_CHECK_EQUAL(257, (int)val1);
    //BOOST_CHECK_EQUAL(32, (int)val0);
    //val0 = val1 * val1 * val1 * val1 * val1 * val1 * val1 * val1;
    //val1 = 257;
    //val1 = pow(val1, 8);
    //BOOST_CHECK_EQUAL(val1, val0);

    //val1 = pow(val1, 0);
    //BOOST_CHECK_EQUAL((int)val1, 1);
}

//void mp_mul_test()
//{
//    using namespace sonia::mp::literals;
//    auto x0 = "5"_bi;
//    BOOST_CHECK_EQUAL(x0 * x0, (uint64_t)25);
//    BOOST_CHECK("26"_bi > (x0 * x0));
//    BOOST_CHECK(26 > (x0 * x0));
//    BOOST_CHECK(26 != (x0 * x0));
//    BOOST_CHECK((x0 * x0) > 24 );
//    BOOST_CHECK_EQUAL(pow("340282366920938463408034375210639556610"_bi, 0xf), "94971145180789141173792039356877348546615710136820159429620005475820741373484440733711065254441078508238038934605050990664553807873876669074305644271758756235065228100642097755554173541883690838600982589611688150716320040505871847239934643409009493854662453165945031933545041297089845350845567778753783952870388913863486884966424088583811509592259257648204772815239748003247381690020399370551715721516812596449220997661037663386512666647739896418940688124424283266194059250293267995087980181863547749199304889944124217666383500994250505363539943187173672572952901000000000000000"_bi);
//}

void basic_integer_test_registrar()
{
    //register_test(BOOST_TEST_CASE(&mp_mul_test));
    register_test(BOOST_TEST_CASE(&basic_integer_test0));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(basic_integer_test_registrar)
#endif
