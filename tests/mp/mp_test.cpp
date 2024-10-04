//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#define TEST_FOLDER "mp_test"

#include "sonia/config.hpp"
//#undef BOOST_ENABLE_ASSERT_HANDLER
#define SONIA_ARITHMETIC_USE_INVINT_DIV
#include "applied/sonia_test.hpp"

#include <fstream>
#include <iostream>

#include "sonia/filesystem.hpp"

#include "sonia/utility/scope_exit.hpp"
#include "sonia/mp/integer_view.hpp"
#include "sonia/mp/integer_view_arithmetic.hpp"
#include "sonia/mp/limbs_from_string.hpp"

template <typename LimbT>
void limbs_encode_decode_test(int base, int digit_count)
{
    std::string resstr;
    std::string numstr;
    std::string_view alphabet = base > 36 ? sonia::mp::detail::default_alphabet_big : sonia::mp::detail::default_alphabet;
    numstr.reserve(digit_count + 1);
    for (int i = 1; i < digit_count; ++i) {
        // generate string
        numstr.push_back(alphabet[i % base]);
        //if (i < 41) continue;
        std::string_view numstrv = numstr;
        auto optres = sonia::mp::to_limbs<LimbT>(numstrv, base, std::allocator<LimbT>{});
        auto [limbs, size, allocsize, sign] = *optres;
        defer{ if (limbs) std::allocator<LimbT>{}.deallocate(limbs, allocsize); };
        //*
        bool reversed = false;
        std::span sp0{ limbs, size };
        sonia::mp::to_string(sp0, std::back_inserter(resstr), reversed, base);
        if (reversed) {
            std::reverse(resstr.begin(), resstr.end());
        }
        //std::cout << numstr << "\n";
        if (numstr != resstr) {
            std::cout << "i: " << i << "\nexpected:\n\t" << numstr << "\nretrieved:\n\t" << resstr << "\n------------------------\n";
        }
        resstr.clear();
        //*/
        if (!(i % 1024)) std::cout << "done " << i << "\n";
    }
}

template <std::unsigned_integral T>
void test_sqrt()
{
    namespace sa = sonia::arithmetic;
    for (T v = 0; v < (std::numeric_limits<T>::max)(); ++v)
    {
        //std::cout << "sqrt(" << (int)v << ") = " << (int)sa::sqrt<T>(v) << "\n";
        T v1 = v + 1;
        T s = sa::sqrt<T>(v1);
        if ( !(v & 0xffffff)) {
            std::cout << "sqrt(" << (unsigned int)v1 << ") = " << (int)sa::sqrt<T>(v1) << "\n";
        }
        bool flag = s * s <= v1 && ((uint64_t)(s + 1)) * ((uint64_t)(s + 1)) > v1;
        BOOST_CHECK(flag);
        if (!flag) {
            std::cout << "wrong sqrt(" << std::hex << (unsigned int)v1 << ") = " << (unsigned int)sa::sqrt<T>(v1) << "\n";
            break;
        }
    }
}

void mp_enc_dec_test()
{
    namespace ct = sonia::mpct;

    std::cout << "uint8_t...\n";
    test_sqrt<uint8_t>();
    std::cout << "uint16_t...\n";
    test_sqrt<uint16_t>();
    std::cout << "uint32_t...\n";
    test_sqrt<uint32_t>();

    constexpr uint64_t m = 0x12341234ull * 0x12341234ull;
    constexpr uint64_t r = sonia::arithmetic::sqrt<uint64_t>(m);
    static_assert(r == 0x12341234ull);

#if 0
    limbs_encode_decode_test<uint64_t>(10, 1024);

    //*
    limbs_encode_decode_test<uint64_t>(8, 1024);
    limbs_encode_decode_test<uint32_t>(8, 1024);
    limbs_encode_decode_test<uint16_t>(8, 1024);
    limbs_encode_decode_test<uint8_t>(8, 1024);

    limbs_encode_decode_test<uint64_t>(16, 1024);
    limbs_encode_decode_test<uint32_t>(16, 1024);
    limbs_encode_decode_test<uint16_t>(16, 1024);
    limbs_encode_decode_test<uint8_t>(16, 1024);

    limbs_encode_decode_test<uint64_t>(32, 1024);
    limbs_encode_decode_test<uint32_t>(32, 1024);
    limbs_encode_decode_test<uint16_t>(32, 1024);
    limbs_encode_decode_test<uint8_t>(32, 1024);
    //*/

    //*
    limbs_encode_decode_test<uint8_t>(10, 1024);
    limbs_encode_decode_test<uint16_t>(10, 1024);
    limbs_encode_decode_test<uint32_t>(10, 1024);
    limbs_encode_decode_test<uint64_t>(10, 1024);
    //*/
    int dcount = 256;
    for (int base = 2; base < 63; ++base) {
        std::cout << "Base: " << base << "\n";
        limbs_encode_decode_test<uint64_t>(base, dcount);
        limbs_encode_decode_test<uint32_t>(base, dcount);
        limbs_encode_decode_test<uint16_t>(base, dcount);
        limbs_encode_decode_test<uint8_t>(base, dcount);
    }

    //limbs_encode_decode_test<uint64_t>(10, 1024);
#endif
}

void mp_ct_test()
{
    namespace ct = sonia::mpct;
    using namespace ct::literals;
    using ct::W;

    static_assert(getsize(normalize(W<1, 1, 0, 0, 0>)) == 2);

    //using r0_t = ct::add<ct::limbs<uint32_t, 0xffffFFFF>, ct::limbs<uint32_t, 3>>;
    using r0_t = decltype(ct::limbs<uint32_t, 0xffffFFFF>{} + ct::limbs<uint32_t, 3>{});
    static_assert(ct::at<r0_t, 0> == 2);
    static_assert(ct::at<r0_t, 1> == 1);
    static_assert(ct::back<r0_t> == 1);
    static_assert(ct::size<r0_t> == 2);

    //using r1_t = ct::mul1<ct::limbs<uint32_t, 0xffffFFFF, 0xffffFFFF>, uint32_t(16)>;
    using r1_t = decltype(ct::limbs<uint32_t, 0xffffFFFF, 0xffffFFFF>{} *ct::limbs<uint32_t, 16>{});
    static_assert(ct::at<r1_t, 0> == 0xffffFFF0);
    static_assert(ct::at<r1_t, 1> == 0xffffFFFF);
    static_assert(ct::at<r1_t, 2> == 0xf);
    static_assert(ct::back<r1_t> == 0xf);
    static_assert(ct::size<r1_t> == 3);

    using r1_1_t = decltype(100_W * 65536_W);
    static_assert(ct::size<r1_1_t> == 1);
    static_assert(ct::back<r1_1_t> == 6553600);

    //using r2_t = ct::mul1<ct::limbs<uint32_t, 0xffffFFFF, 0xffffFFFF>, uint32_t(0xffffFFFF)>;
    using r2_t = decltype(ct::W32<0xffffFFFF, 0xffffFFFF>* ct::W32<0xffffFFFF>);
    static_assert(ct::at<r2_t, 0> == 1);
    static_assert(ct::at<r2_t, 1> == 0xffffFFFF);
    static_assert(ct::at<r2_t, 2> == 0xffffFFFe);
    static_assert(ct::size<r2_t> == 3);

    //using r3_t = ct::mul1<ct::limbs<uint64_t, 0xffffFFFFffffFFFFULL, 0xffffFFFFffffFFFFULL>, uint64_t(0xffffFFFFffffFFFFULL)>;
    using r3_t = decltype(ct::limbs<uint64_t, 0xffffFFFFffffFFFFULL, 0xffffFFFFffffFFFFULL>{} *ct::limbs<uint64_t, 0xffffFFFFffffFFFFULL>{});
    static_assert(ct::at<r3_t, 0> == 1);
    static_assert(ct::at<r3_t, 1> == 0xffffFFFFffffFFFFULL);
    static_assert(ct::at<r3_t, 2> == 0xffffFFFFffffFFFE);
    static_assert(ct::size<r3_t> == 3);

    using r4_t = ct::mul<ct::limbs<uint64_t, 0xffffFFFFffffFFFFULL, 0xffffFFFFffffFFFFULL>, ct::limbs<uint64_t, 0xffffFFFFffffFFFFULL, 0xffffFFFFffffFFFFULL>>;
    static_assert(ct::at<r4_t, 0> == 1);
    static_assert(ct::at<r4_t, 1> == 0);
    static_assert(ct::at<r4_t, 2> == 0xffffFFFFffffFFFEULL);
    static_assert(ct::at<r4_t, 3> == 0xffffFFFFffffFFFFULL);
    static_assert(ct::size<r4_t> == 4);

    using r5_t = decltype(W<1> -W<3, 0>);
    static_assert(ct::size<r5_t> == 2);
    static_assert(ct::front<r5_t> == 0xffffFFFFffffFFFEULL);
    static_assert(ct::back<r5_t> == 0xffffFFFFffffFFFFULL);

    static_assert(ct::cmp<decltype(100_W), decltype(101_W)> == -1);
    static_assert(ct::cmp<ct::limbs<uint64_t, 1, 0, 0>, ct::limbs<uint64_t, 1>> == 0);
    static_assert(ct::cmp<ct::limbs<uint64_t, 1, 0, 1>, ct::limbs<uint64_t, 1>> == 1);
    static_assert(ct::cmp<ct::limbs<uint64_t, 1, 0, 1>, ct::limbs<uint64_t, 2, 0, 1>> == -1);

    using r6_t = decltype(upow(100_W, 12_W)); // = 54210 * 2 ^ 64 + 2003764205206896640 = 1000000000000000000000000
    static_assert(ct::size<r6_t> == 2);
    static_assert(ct::back<r6_t> == 54210);
    static_assert(ct::front<r6_t> == 2003764205206896640UL);

    using r7_t = ct::shift_left<decltype(W<0xffffFFFFffffFFFFULL, 0xffffFFFFffffFFFFULL>), 1>;
    static_assert(ct::size<r7_t> == 3);
    static_assert(ct::front<r7_t> == 0xffffFFFFffffFFFEULL);
    static_assert(ct::at<r7_t, 1> == 0xffffFFFFffffFFFFULL);
    static_assert(ct::back<r7_t> == 1);

    using r8_t = ct::shift_left<decltype(W<0xffffFFFFffffFFFFULL, 0x7fffFFFFffffFFFFULL>), 1>;
    static_assert(ct::size<r8_t> == 2);
    static_assert(ct::back<r8_t> == 0xffffFFFFffffFFFFULL);
    static_assert(ct::front<r8_t> == 0xffffFFFFffffFFFEULL);

    //using r9_t = ct::shift_left<decltype(W<0xffffFFFFffffFFFFULL, 0x7fffFFFFffffFFFFULL>), 65>;
    using r9_t = decltype(W<0xffffFFFFffffFFFFULL, 0x7fffFFFFffffFFFFULL> << W<65>);
    static_assert(ct::size<r9_t> == 3);
    static_assert(ct::back<r9_t> == 0xffffFFFFffffFFFFULL);
    static_assert(ct::at<r9_t, 1> == 0xffffFFFFffffFFFEULL);
    static_assert(ct::front<r9_t> == 0);

    {
        constexpr auto max64 = 0xffffFFFFffffFFFFULL;
        using r0_t = decltype(W<max64, max64-1, max64-2, max64-3, max64-4, max64-5>);
        static_assert(ct::back<ct::slice_limbs<r0_t, 1>> == -1);
        static_assert(ct::back<ct::slice_limbs<r0_t, 2>> == -2);
        static_assert(ct::back<ct::slice_limbs<r0_t, 3>> == -3);
        static_assert(ct::back<ct::slice_limbs<r0_t, 4>> == -4);
        static_assert(ct::back<ct::slice_limbs<r0_t, 5>> == -5);
        static_assert(ct::back<ct::slice_limbs<r0_t, 6>> == -6);
        static_assert(ct::back<ct::slice<r0_t, 128>> == -2);
        static_assert(ct::back<ct::slice<r0_t, 124>> == 0x0fffFFFFffffFFFEULL);
    }
    {
        using r0_t = ct::shift_right<decltype(W<0xffffFFFFffffFFFFULL, 0x7fffFFFFffffFFFFULL>), 0>;
        static_assert(ct::size<r0_t> == 2);
        static_assert(ct::at<r0_t, 0> == 0xffffFFFFffffFFFFULL);
        static_assert(ct::at<r0_t, 1> == 0x7fffFFFFffffFFFFULL);

        using r1_t = ct::shift_right<decltype(W<0xffffFFFFffffFFFFULL, 0x7fffFFFFffffFFFFULL>), 1>;
        static_assert(ct::size<r1_t> == 2);
        static_assert(ct::at<r1_t, 0> == 0xffffFFFFffffFFFFULL);
        static_assert(ct::at<r1_t, 1> == 0x3fffFFFFffffFFFFULL);

        using r2_t = ct::shift_right<decltype(W<0xffffFFFFffffFFFFULL, 0x7fffFFFFffffFFFFULL>), 63>;
        static_assert(ct::size<r2_t> == 1);
        static_assert(ct::at<r1_t, 0> == 0xffffFFFFffffFFFFULL);
    }

    using r10_t = ct::div1<r3_t, 0xffffFFFFffffFFFFULL>;
    static_assert(ct::at<r10_t, 0> == 0xffffFFFFffffFFFFULL);
    static_assert(ct::at<r10_t, 1> == 0xffffFFFFffffFFFFULL);
    static_assert(ct::size<r10_t> == 2);

    //constexpr auto c2_64 = W<2> ^ W<64>;
    //constexpr auto mdb = W<0x8ac7230489e80000ULL>;
    //using r11_t = decltype(c2_64 * c2_64);
    //using r11_t = decltype(c2_64* (c2_64 - mdb));
    //static_assert(ct::front<r11_t> == 0xd83c94fb6d2ac34aULL);
    //std::cout << typeid(ct::mul<decltype(c2_64), decltype(c2_64)>).name() << "\n";
    //std::cout << typeid(r6_t).name() << "\n" << sonia::mp::arithmetic::consteval_log2<uint32_t>(16);


    //using r6_t = decltype(100_W);
    //using ct10 = ct::limbs<uint32_t, 10>;
    //using r5_t = ct::pow<ct10, 12>;
    //std::cout << typeid(ct::norm<ct::limbs<uint64_t, 1, 0, 0, 0, 0, 1>>).name() << "\n";
    //std::cout << typeid(r6_t).name() << "\n";

    /*
    using ct10 = ct::limbs<uint32_t, 10>;
    using ct16 = ct::limbs<uint32_t, 16>;
    using ct1010 = ct::limbs<uint32_t, 1010>;

    using ctx100 = ct::mul1<ct16, uint32_t(16)>;
    static_assert(ct::at<ctx100, 0> == 256);
    using ctx10000 = ct::mul1<ctx100, uint32_t(256)>;
    static_assert(ct::at<ctx10000, 0> == 0x10000);
    using ctx100000000 = ct::mul1<ctx10000, uint32_t(0x10000)>;
    static_assert(ct::at<ctx100000000, 0> == 0);
    static_assert(ct::at<ctx100000000, 1> == 1);

    using ct16nff = ct::limbs<uint32_t, 0xffffFFFF, 0xffffFFFF>;
    using r_t = ct::mul1<ct16nff, uint32_t(0xffffFFFF)>;
    static_assert(ct::at<r_t, 0> == 0xffffFFFE);
    static_assert(ct::at<r_t, 1> == 0xffffFFFF);
    //using r_t = ct::mul<ct10, ct1010>;
    //using r_t = ct::mul1<ct16, uint32_t(16)>;
    std::cout << typeid(r_t).name() << "\n";
    */
    //0x1ffffffffFFFFfffe

}

void mp_integer_test()
{
    namespace ct = sonia::mpct;
    using namespace sonia::mp;
    using namespace ct::literals;
    using ct::W;

    // to integral casts
    {
        basic_integer_view<uint32_t> v0 = ct::W32<0xffffFFFEU>;
        BOOST_CHECK_EQUAL((uint64_t)v0, 0xffffFFFEU);
        BOOST_CHECK_EQUAL((uint32_t)v0, 0xffffFFFEU);
        BOOST_CHECK_EQUAL((uint16_t)v0, 0xFFFE);
        BOOST_CHECK_EQUAL((uint8_t)v0, 0xFE);

        basic_integer_view<uint32_t> v1{ct::W32<0x7fffFF7EU, 0x4fffFFFFU, 0xffffFFFFU>, -1};
        BOOST_CHECK_EQUAL((uint8_t)v1, 0x82);
        BOOST_CHECK_EQUAL((int8_t)v1, -0x7e);
        BOOST_CHECK_EQUAL((uint16_t)v1, 0x82);
        BOOST_CHECK_EQUAL((int16_t)v1, 0x82);
        BOOST_CHECK_EQUAL((uint32_t)v1, -0x7fffFF7E);
        BOOST_CHECK_EQUAL((int32_t)v1, -0x7fffFF7E);
        BOOST_CHECK_EQUAL((uint64_t)v1, -0x4fffFFFF7fffFF7E);
        BOOST_CHECK_EQUAL((int64_t)v1, -0x4fffFFFF7fffFF7E);

        basic_integer_view<uint8_t> v2(ct::W8<0x1>, -1);
        BOOST_CHECK_EQUAL((int64_t)v2, -1);
        BOOST_CHECK_EQUAL((uint64_t)v2, 0xFFFFffffFFFFffffULL);
    }
    // unary minus
    {
        basic_integer_view v0 = ct::W<0x222>;
        BOOST_CHECK_EQUAL((int64_t)-v0, -0x222);
    }
    //euqals
    {
        basic_integer_view v0 = ct::W<0x221, 0x222>;
        basic_integer_view v1 = ct::W<0x221, 0x222, 0>;
        basic_integer_view v2 = ct::W<0x221, 0x222, 1>;
        BOOST_CHECK(v0 == v1);
        BOOST_CHECK(v1 != v2);

        basic_integer_view v3 = ct::W8<0x21, 0x22>;
        BOOST_CHECK(v3 == 0x2221);
        BOOST_CHECK(0x2221 == v3);
        basic_integer_view v4 = ct::W<0x2221, 0, 1>;
        BOOST_CHECK(v4 != 0x2221);
        BOOST_CHECK(0x2221 != v4);

        basic_integer_view v3m = -v3;
        BOOST_CHECK(v3m == -0x2221);
        BOOST_CHECK(-0x2221 == v3m);
        BOOST_CHECK(v3m != 0x2221);
    }
    // compare
    {
        basic_integer_view v0 = ct::W<0x221, 0x222>;
        basic_integer_view v1 = ct::W<0x221, 0x222, 0>;
        BOOST_CHECK(!(v0 < v1));
        BOOST_CHECK(!(v1 < v0));
        BOOST_CHECK(v0 <= v1);
        BOOST_CHECK(v1 <= v0);
        BOOST_CHECK(v0 > 0x222);

        v0 = ct::W<0x221>;
        v1 = ct::W<0x222>;
        BOOST_CHECK(v0 < v1);
        BOOST_CHECK(-v1 < -v0);
        BOOST_CHECK(v0 > -0x221);
        BOOST_CHECK(-0x221 < v1);
        BOOST_CHECK(0x223 > v1);
    }
    // plus
    {
        {
            basic_integer_view v0 = ct::W32<0xFFFFffffULL>;
            basic_integer_view v1 = ct::W32<2, 0x222, 0>;
            auto [limbs, sz, rsz, sign] = add(v0, v1, std::allocator<uint32_t>{});
            defer{ std::allocator<uint32_t>{}.deallocate(limbs, rsz); };
            basic_integer_view r{ std::span{limbs, sz}, sign };
            BOOST_CHECK((r == ct::W32<0x1, 0x223>));
        }
        {
            auto [limbs, sz, rsz, sign] = add(-basic_integer_view{ 5_W }, basic_integer_view{ 12_W }, std::allocator<uint64_t>{});
            defer{ std::allocator<uint64_t>{}.deallocate(limbs, rsz); };
            basic_integer_view r{ std::span{limbs, sz}, sign };
            BOOST_CHECK(r == 7_W);
        }
    }
}

#include "sonia/mp/basic_integer.hpp"

void mp_integer_test2()
{
    using int_t = sonia::mp::basic_integer<uint64_t, 1>;
    size_t isz = sizeof(int_t);
    int_t val0{16};
    val0 = val0 * 16 + 1;
    BOOST_CHECK_EQUAL(257, (int)val0);
    int_t val1{ 32 };
    swap(val0, val1);
    BOOST_CHECK_EQUAL(257, (int)val1);
    BOOST_CHECK_EQUAL(32, (int)val0);
    val0 = val1 * val1 * val1 * val1 * val1 * val1 * val1 * val1;
    val1 = 257;
    val1 = pow(val1, 8);
    BOOST_CHECK_EQUAL(val1, val0);

    val1 = pow(val1, 0);
    BOOST_CHECK_EQUAL((int)val1, 1);
}

std::string tohex(double a, double r, double g, double b)
{
    uint32_t val = ((((uint32_t)(a * 255)) * 256 + ((uint32_t)(r * 255))) * 256 + ((uint32_t)(g * 255))) * 256 + ((uint32_t)(b * 255));
    std::ostringstream ss;
    ss << std::hex << std::uppercase << "#" << val;
    return ss.str();
}

using namespace sonia;

void div_test()
{
    char const* path = std::getenv("TESTS_HOME");
    fs::path suitedir{ (path ? fs::path(path) / "testdata" : fs::path("testdata")) / "mp" };
    fs::path filepath = suitedir / "test_div_data.txt";

    std::ifstream file;
    file.exceptions(std::ifstream::badbit);
    file.open(filepath.string().c_str());
    
    using data_tpl_t = std::tuple<mp::integer, mp::integer, mp::integer, mp::integer>;
    std::vector<data_tpl_t> data_set;
    for (;;)
    {
        std::string s_str, d_str, q_str, r_str, emt_str;
        if (!std::getline(file, s_str) || s_str.empty()) break;
        std::getline(file, d_str);
        std::getline(file, q_str);
        std::getline(file, r_str);
        std::getline(file, emt_str);
        BOOST_REQUIRE(emt_str.empty());

        mp::integer s{ s_str, 10 };
        mp::integer d{ d_str, 10 };
        mp::integer q{ q_str, 10 };
        mp::integer r{ r_str, 10 };
        
        //auto q_calc = s.div_qr(d);
        //auto c0 = q_calc.limbs();
        //auto c1 = q.limbs();
        //BOOST_CHECK_EQUAL(q_calc, q);
        //BOOST_CHECK_EQUAL(s, r);

        //data_set.emplace_back(std::move(s), std::move(d), std::move(q), std::move(r));
        data_set.emplace_back(s, d, q, r);
    }
    std::cout << "loaded #" << data_set.size() << "\n";

    for (int test_cnt = 0; test_cnt < 32; ++test_cnt)
    {
        std::vector<data_tpl_t> data_set_copy = data_set;
        auto start = std::chrono::steady_clock::now();
        for (auto & tpl : data_set_copy)
        {
            mp::integer& s = get<0>(tpl);
            mp::integer& d = get<1>(tpl);
            mp::integer& q = get<2>(tpl);
            mp::integer& r = get<3>(tpl);
        
            //BOOST_CHECK_EQUAL(q * d + r - s, 0);
            auto q_calc = s.div_qr(d);

            BOOST_CHECK_EQUAL(q_calc, q);
            BOOST_CHECK_EQUAL(s, r);
        }
        auto finish = std::chrono::steady_clock::now();
        std::cout << "done in: " << (finish - start) << "\n";
    }
        // q * d = s - r

        //if (cnt >= 180) {
            //auto s_calc = q * d + r - s;
            //auto c1 = q.limbs();
            //auto d1 = d.limbs();
            //BOOST_CHECK_EQUAL(q * d + r - s, 0);
            //auto q_calc = s.div_qr(d);
            //auto c0 = q_calc.limbs();
            //auto r0 = s.limbs();
            //auto r1 = r.limbs();

            //BOOST_CHECK_EQUAL(q_calc, q);
            //BOOST_CHECK_EQUAL(s, r);
        //}
        //auto s_calc = q * d + r;
        //BOOST_CHECK_EQUAL(s_calc, mp::integer{ 0 });
        //BOOST_CHECK_EQUAL(s_calc, s);

        //std::istringstream iss(line);
        //int a, b;
        //if (!(iss >> a >> b)) { break; } // error

        // process pair (a,b)

        //std::cout << "done #" << cnt++ << "\n";

    //auto finish = std::chrono::steady_clock::now();
    //std::cout << "done in: " << (finish - start) << "\n";
    //std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(suite_text));

}

void mp_test_registrar()
{
    //std::cout << tohex(1.0, 0.957, 0.961, 0.961) << "\n";
    //std::cout << tohex(1.0, 0.247, 0.247, 0.275) << "\n";
    register_test(BOOST_TEST_CASE(&div_test));
    //register_test(BOOST_TEST_CASE(&mp_enc_dec_test));
    //register_test(BOOST_TEST_CASE_SILENT(&mp_ct_test));
    //register_test(BOOST_TEST_CASE(&mp_integer_test));
    //register_test(BOOST_TEST_CASE(&mp_integer_test2));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(mp_test_registrar)
#endif