//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#define TEST_FOLDER "mp_test"

#include "sonia/config.hpp"
#undef BOOST_ENABLE_ASSERT_HANDLER

#include "applied/sonia_test.hpp"

#include "sonia/mp/basic_decimal.hpp"
#include "sonia/mp/basic_integer.hpp"
#include <vector>
#include <iostream>
#include <cfloat>
//#include <boost/container/small_vector.hpp>

void basic_decimal_test0()
{
    using dec_t = sonia::mp::basic_decimal<uint64_t, 1, 8>;
    using mdec_t = sonia::mp::basic_decimal<uint8_t, 1, 2>;

    //std::cout << mdec_t::storage_type::sig_limb_count_bits << "\n";
    //using biv_t = sonia::mp::basic_integer_view<uint64_t>;
    using namespace std::string_view_literals;
    using namespace sonia::mp::literals;

    //auto vvv = -.022e-12;

    BOOST_CHECK_EQUAL(dec_t{ ".022"sv }.exponent_as<int>(), -3);
    BOOST_CHECK_EQUAL(dec_t{ "-.022"sv }.significand(), -22);
    BOOST_CHECK_EQUAL(dec_t{ "-0.022"sv }.significand(), -22);

    BOOST_CHECK(!dec_t{});
    BOOST_CHECK(dec_t{ 1 });
    BOOST_CHECK(dec_t{ 10000000000ll }.is_inplaced());
    BOOST_CHECK_EQUAL(dec_t{ 10000000000ll }.significand(), 1);
    BOOST_CHECK_EQUAL(dec_t{ 10000000000ll }.exponent_as<int>(), 10);

    BOOST_CHECK(!dec_t{ "0"sv });
    BOOST_CHECK_EQUAL(dec_t{ "-10000000000"sv }.significand(), -1);
    BOOST_CHECK_EQUAL(dec_t{ "-10000000000"sv }.exponent_as<int>(), 10);
    BOOST_CHECK_EQUAL(dec_t{ "0.1"sv }.exponent_as<int>(), -1);
    BOOST_CHECK_EQUAL(dec_t{ ".1"sv }.exponent_as<int>(), -1);

    BOOST_CHECK_EQUAL(dec_t{ "0.1"sv }.exponent_as<int>(), -1);
    BOOST_CHECK_EQUAL(dec_t{ "0.1"sv }.significand(), 1);
    BOOST_CHECK_EQUAL(dec_t{ "00.2"sv }.exponent_as<int>(), -1);
    BOOST_CHECK_EQUAL(dec_t{ "00.2"sv }.significand(), 2);
    BOOST_CHECK_EQUAL(dec_t{ ".3"sv }.exponent_as<int>(), -1);
    BOOST_CHECK_EQUAL(dec_t{ ".3"sv }.significand(), 3);
   
    BOOST_CHECK_EQUAL(dec_t{ "9.00000000990000000000001"sv }.significand(), 900000000990000000000001_bi);
    BOOST_CHECK_EQUAL(mdec_t{ "9.00000000990000000000001"sv }.significand(), (sonia::mp::basic_integer<uint8_t, 1>("900000000990000000000001"sv)));
    
    BOOST_CHECK_EQUAL(dec_t{ "3.1e5"sv }.significand(), 31);
    BOOST_CHECK_EQUAL(dec_t{ "3.1e5"sv }.exponent_as<int>(), 4);
    BOOST_CHECK_EQUAL(to_string(dec_t{ "3.1e5"sv }), "310000");
    BOOST_CHECK_EQUAL(to_string(dec_t{ "3.1e-5"sv }), "0.000031");
    
    BOOST_CHECK_LE(std::abs((float)dec_t{ "3.1234e-5"sv } - 0.000031234f), FLT_EPSILON);
    BOOST_CHECK_LE(std::abs((double)dec_t { "3.1e-5"sv } - 0.000031), DBL_EPSILON);
    
    BOOST_CHECK_EQUAL(dec_t{ "31e5"sv }, dec_t{ "3.1e6"sv });
    BOOST_CHECK_EQUAL(dec_t{ "-3.1e6"sv }, dec_t{ "-31e5"sv });
    BOOST_CHECK_EQUAL(dec_t{ "31e5"sv }, 3100000);
    
    BOOST_CHECK_LT(dec_t{ "3.11e5"sv }, dec_t{ "3.1e6"sv });
    BOOST_CHECK_GT(dec_t{ "3.1e6"sv }, dec_t{ "3.11e5"sv });
    BOOST_CHECK_GT(dec_t{ "3110000"sv }, dec_t{ "3.1e6"sv });
    BOOST_CHECK_LT(dec_t{ "3.1e6"sv }, dec_t{ "3110000"sv });

    BOOST_CHECK_GT(dec_t{ "3.11000000000000000000000000000001e6"sv }, dec_t{ "3110000"sv });
    BOOST_CHECK_LT(dec_t{ "-3.11000000000000000000000000000001e6"sv }, dec_t{ "-3110000"sv });
    BOOST_CHECK_LT(dec_t{ "3110000"sv }, dec_t{ "3.11000000000000000000000000000001e6"sv });
    BOOST_CHECK_GT(dec_t{ "-3110000"sv }, dec_t{ "-3.11000000000000000000000000000001e6"sv });
    BOOST_CHECK_LT(dec_t{ "3.10999999999999999999999999999999e6"sv }, dec_t{ "3110000"sv });
    BOOST_CHECK_GT(dec_t{ "-3.10999999999999999999999999999999e6"sv }, dec_t{ "-3110000"sv });
    BOOST_CHECK_GT(dec_t{ "3110000"sv }, dec_t{ "3.10999999999999999999999999999999e6"sv });
    BOOST_CHECK_LT(dec_t{ "-3110000"sv }, dec_t{ "-3.10999999999999999999999999999999e6"sv });
}

void basic_decimal_test_registrar()
{
    //register_test(BOOST_TEST_CASE(&mp_mul_test));
    register_test(BOOST_TEST_CASE(&basic_decimal_test0));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(basic_decimal_test_registrar)
#endif
