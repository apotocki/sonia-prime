//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include "sonia/utility/number/decimal.ipp"
#include "sonia/utility/number/decimal.hpp"

#include "applied/sonia_test.hpp"

using namespace sonia;

void decimal_test()
{
    int64_t v;
    int32_t e;

    decimal_parse("0", v, e);
    BOOST_CHECK_EQUAL(v, 0);
    BOOST_CHECK_EQUAL(e, 0);

    decimal_parse("100.10E2", v, e);
    BOOST_CHECK_EQUAL(v, 1001);
    BOOST_CHECK_EQUAL(e, 1);
    BOOST_CHECK_EQUAL(decimal_string(v, e), "10010");

    decimal_parse("-100.1E-2", v, e);
    BOOST_CHECK_EQUAL(v, -1001);
    BOOST_CHECK_EQUAL(e, -3);
    BOOST_CHECK_EQUAL(decimal_string(v, e), "-1.001");

    int64_t lv, rv;
    int32_t le, re;
    decimal_parse("100001E-3", lv, le);
    decimal_parse("1E+2", rv, re);
    BOOST_CHECK(!decimal_less(lv, le, rv, re));
    BOOST_CHECK(decimal_less(rv, re, lv, le));
    BOOST_CHECK(!decimal_equal(lv, le, rv, re));
    BOOST_CHECK(!decimal_equal(rv, re, lv, le));

    decimal_parse("-100001E-3", lv, le);
    decimal_parse("-1E+2", rv, re);
    BOOST_CHECK(decimal_less(lv, le, rv, re));
    BOOST_CHECK(!decimal_less(rv, re, lv, le));
    BOOST_CHECK(!decimal_equal(lv, le, rv, re));
    BOOST_CHECK(!decimal_equal(rv, re, lv, le));

    decimal_parse("-100000E-3", lv, le);
    BOOST_CHECK(decimal_equal(lv, le, rv, re));
    BOOST_CHECK(decimal_equal(rv, re, lv, le));

    decimal d0 = decimal::parse("12.3e-1");
    decimal d1(12);
    decimal d2(1.23);
    BOOST_CHECK(d0 == d2);
    BOOST_CHECK(d1 != d0);

    BOOST_CHECK(d0 < d1);
    BOOST_CHECK(!(d1 < d0));

    BOOST_CHECK(d1 > d0);
    BOOST_CHECK(!(d0 > d1));

    BOOST_CHECK(d0 <= d1);
    BOOST_CHECK(!(d1 <= d0));

    BOOST_CHECK(d1 >= d0);
    BOOST_CHECK(!(d0 >= d1));

    BOOST_CHECK(d0 <= d2);
    BOOST_CHECK(d2 >= d0);


    BOOST_CHECK(d0 < 2);
    BOOST_CHECK(d0 <= 2);
    BOOST_CHECK(d0 != 2);

    BOOST_CHECK(1 < d0);
    BOOST_CHECK(1 <= d0);
    BOOST_CHECK(2 != d0);

    BOOST_CHECK(d0 > 1);
    BOOST_CHECK(d0 >= 1);

    BOOST_CHECK(2 > d0);
    BOOST_CHECK(2 >= d0);
    
    BOOST_CHECK(1.24 >= d0);

    BOOST_CHECK_LE(std::abs(d0.get<float>() - 1.23), 0.0000001);
    BOOST_CHECK_EQUAL(d0.get<int>(), 1);
    BOOST_CHECK_EQUAL((int)d0, 1);

    BOOST_CHECK_NE(hash<decimal>()(d0), hash<decimal>()(d1));
    BOOST_CHECK_EQUAL(hash<decimal>()(d0), hash<decimal>()(d2));
}

void number_test_registrar()
{
    register_test(BOOST_TEST_CASE(&decimal_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(number_test_registrar)
#endif
