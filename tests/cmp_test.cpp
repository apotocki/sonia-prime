//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"
#include "sonia/utility/functional/range_less.hpp"
#include "sonia/utility/functional/range_equal.hpp"
#include "sonia/utility/functional/range_compare.hpp"

#include "applied/sonia_test.hpp"

using namespace sonia;

void integer_compare_test()
{
    BOOST_CHECK(!less_f((unsigned int)0xffffffff, (int)-1));
    BOOST_CHECK(less_f((int)-1, (unsigned int)0xffffffff));
    BOOST_CHECK(!less_f((unsigned int)1, (int)-1));
    BOOST_CHECK(less_f((int)-1, (unsigned int)1));

    BOOST_CHECK(!less_f((unsigned short)100, (int)-1));
    BOOST_CHECK(less_f((int)-1, (unsigned short)100));

    BOOST_CHECK(!less_f((short)100, (int)-1));
    BOOST_CHECK(less_f((int)-1, (short)100));
}

void range_compare_test()
{
    std::string s1 = "abc1";
    std::string s2 = "abc2";

    unsigned char us1[] = "abc1";
    unsigned char us2[] = "abc2";

    BOOST_CHECK(less_f(s1, s2));
    BOOST_CHECK(!less_f(s2, s1));

    BOOST_CHECK(less_f(us1, us2));
    BOOST_CHECK(!less_f(us2, us1));

    BOOST_CHECK(compare_f(s1, s2) < 0);
    BOOST_CHECK(compare_f(s2, s1) > 0);
    BOOST_CHECK_EQUAL(compare_f(s2, s2), 0);

    BOOST_CHECK(compare_f(us1, us2) < 0);
    BOOST_CHECK(compare_f(us2, us1) > 0);
    BOOST_CHECK_EQUAL(compare_f(us2, us2), 0);

    BOOST_CHECK(!equal_f(s2, s1));
    BOOST_CHECK(equal_f(s1, s1));

    BOOST_CHECK(!equal_f(to_string_view(s2), to_string_view(s1)));
    BOOST_CHECK(equal_f(to_string_view(s1), to_string_view(s1)));

    //BOOST_CHECK(ends_with("abcdefgh", "fgh"));
    //BOOST_CHECK(!ends_with("abcdefgh", "ffgh"));

    BOOST_CHECK(string_view("abcdefgh").starts_with(string_view("abc")));
    BOOST_CHECK(!string_view("abcdefgh").starts_with(string_view("abd")));
    BOOST_CHECK(string_view("abcdefgh").starts_with(string_view("abcdefgh")));
    BOOST_CHECK(!string_view("abcdefgh").starts_with(string_view("abcdefgha")));
}

void default_compare_nature_test()
{
    unsigned char rng0[] = { 129 };
    unsigned char rng1[] = { 130 };
    signed char rng2[] = { 127 };
    BOOST_CHECK(less_f(rng0, rng1));
    BOOST_CHECK(less_f(rng2, rng0));
}

void cmp_test_registrar()
{
    register_test(BOOST_TEST_CASE(&integer_compare_test));
    register_test(BOOST_TEST_CASE(&range_compare_test));
    register_test(BOOST_TEST_CASE(&default_compare_nature_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(cmp_test_registrar)
#endif
