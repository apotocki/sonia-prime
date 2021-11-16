//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include "sonia/utility/optimized/holder.hpp"
#include "sonia/utility/optimized/integer.hpp"

#include "applied/sonia_test.hpp"

using namespace sonia;

void optimized_holder_test()
{
    using opt0_t = optimized_holder<8, 4>;

    opt0_t v0;
    *v0.data() = 1;
    BOOST_CHECK(!v0.is_ptr());

    for (uint32_t cv = 0; cv < 16; ++cv) {
        v0.set_uint(0);
        v0.set_service_cookie(cv);
        BOOST_CHECK(!v0.is_ptr());
        BOOST_CHECK_EQUAL(cv, v0.get_service_cookie());
        BOOST_CHECK_EQUAL(0, v0.get_uint());
        v0.set_uint(opt0_t::uint_max);
        BOOST_CHECK_EQUAL(opt0_t::uint_max, v0.get_uint());
        BOOST_CHECK_EQUAL(cv, v0.get_service_cookie());
    }

    using opt1_t = optimized_holder<10, 9>;
    opt1_t v1;
    *v1.data() = 1;
    BOOST_CHECK(!v1.is_ptr());
    for (uint32_t cv = 0; cv < 256; ++cv) {
        v1.set_uint(0);
        v1.set_service_cookie(cv);
        BOOST_CHECK(!v1.is_ptr());
        BOOST_CHECK_EQUAL(cv, v1.get_service_cookie());
        BOOST_CHECK_EQUAL(0, v1.get_uint());
        v1.set_uint(opt1_t::uint_max);
        BOOST_CHECK_EQUAL(opt1_t::uint_max, v1.get_uint());
        BOOST_CHECK_EQUAL(cv, v1.get_service_cookie());
        v1.set_service_cookie(cv);
        BOOST_CHECK_EQUAL(opt1_t::uint_max, v1.get_uint());
    }
}

void optimized_integer_test()
{
    using holder_t = optimized_holder<8, 4>;
    using oint_t = optimized_integer_impl<holder_t>;

    holder_t h0;
    oint_t::init(&h0, 0);
    h0.set_service_cookie(15);

    BOOST_CHECK_EQUAL(oint_t::get<uint32_t>(&h0), 0);
    BOOST_CHECK_EQUAL(15, h0.get_service_cookie());

    for (int i = 128; i >= -128; --i) {
        oint_t::set(&h0, i);
        BOOST_CHECK_EQUAL(oint_t::get<int>(&h0), i);
        BOOST_CHECK_EQUAL(15, h0.get_service_cookie());
    }

    uint32_t tv0 = 0xffff;
    oint_t::set(&h0, tv0);
    BOOST_CHECK_EQUAL(oint_t::get<uint32_t>(&h0), tv0);
    BOOST_CHECK(!h0.is_ptr());

    int64_t tv1 = 0x8000ffff0000ffff;
    oint_t::set(&h0, tv1);
    BOOST_CHECK_EQUAL(oint_t::get<int64_t>(&h0), tv1);
    BOOST_CHECK(h0.is_ptr());

    holder_t h1(h0);
    BOOST_CHECK_EQUAL(oint_t::get<int64_t>(&h1), tv1);
    BOOST_CHECK_EQUAL(15, h1.get_service_cookie());
    BOOST_CHECK(h1.is_ptr());

    holder_t h2(cref(h1));
    BOOST_CHECK_EQUAL(oint_t::get<int64_t>(&h1), tv1);
    BOOST_CHECK_EQUAL(15, h1.get_service_cookie());
    BOOST_CHECK(h1.is_ptr());
}

void optimized_test_registrar()
{
    register_test(BOOST_TEST_CASE(&optimized_holder_test));
    register_test(BOOST_TEST_CASE(&optimized_integer_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(optimized_test_registrar)
#endif
