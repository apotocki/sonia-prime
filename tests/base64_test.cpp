/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include "sonia/config.hpp"

#include <iostream>

#include "sonia/utility/conversion/ascii85/ascii85.hpp"
#include "sonia/utility/conversion/base64/base64.hpp"
#include "sonia/utility/conversion/base32/base32.hpp"
#include "sonia/utility/conversion/utility.hpp"

#include "applied/sonia_test.hpp"

namespace cvt = sonia::conversion;

template <typename TestSetT, typename ConverterT>
void encode_base_test(TestSetT const& ts, ConverterT const& enc)
{
	try {
		for (size_t i = 0; i < sizeof(ts.testtext)/sizeof(char*); ++i)
		{
			const char * begin = ts.testtext[i];
			const char * end = begin + strlen(ts.testtext[i]);

            std::string result;
            cvt::push(begin, end, std::back_inserter(result), enc);
			BOOST_CHECK_EQUAL(result, ts.expresults[i]);

            std::string result2;
            cvt::pull(begin, end, std::back_inserter(result2), enc);
            BOOST_CHECK_EQUAL(result2, ts.expresults[i]);
		}
	} catch(std::exception & exp) {
		BOOST_ERROR(exp.what());
	}
}

template <typename TestSetT, typename ConverterT>
void decode_base_test(TestSetT const& ts, ConverterT const& enc)
{
	for (size_t i = 0; i < sizeof(ts.testtext)/sizeof(char*); ++i)
	{
		const char * begin = ts.testtext[i];
		const char * end = begin + strlen(ts.testtext[i]);

		try {
			std::string result;
			cvt::push(begin, end, std::back_inserter(result), enc);
			BOOST_CHECK_EQUAL(result, ts.expresults[i]);
		}
		catch (std::exception& exp) {
			BOOST_ERROR(exp.what());
		}
		try {
			std::string result2;
			cvt::pull(begin, end, std::back_inserter(result2), enc);
			BOOST_CHECK_EQUAL(result2, ts.expresults[i]);
		}
		catch (std::exception& exp) {
			BOOST_ERROR(exp.what());
		}
	}
}

struct base64_encode_test_set
{
	const char* testtext[4] = { "12345678", "123456789", "123456789a", "123456789ab" };
	const char* expresults[4] = { "MTIzNDU2Nzg=", "MTIzNDU2Nzg5", "MTIzNDU2Nzg5YQ==", "MTIzNDU2Nzg5YWI=" };
};

struct base64_decode_test_set
{
	const char* testtext[4] = { "MTIzNDU2Nzg=", "MTIzNDU2Nzg5", "MTIzNDU2Nzg5YQ==", "MTIzNDU2Nzg5YWI=" };
	const char* expresults[4] = { "12345678", "123456789", "123456789a", "123456789ab" };
};

struct base32_encode_test_set
{
	const char* testtext[5] = { "foo", "foob", "fooba", "foobar", "foobars" };
	const char* expresults[5] = { "MZXW6===", "MZXW6YQ=", "MZXW6YTB", "MZXW6YTBOI======", "MZXW6YTBOJZQ===="  };
};

struct base32_decode_test_set
{
	const char* testtext[5] = { "MZXW6===", "MZXW6YQ=", "MZXW6YTB", "MZXW6YTBOI======", "MZXW6YTBOJZQ====" };
	const char* expresults[5] = { "foo", "foob", "fooba", "foobar", "foobars" };
};

///////////////
struct base32hex_encode_test_set
{
	const char* testtext[7] = { "", "f", "fo", "foo", "foob", "fooba", "foobar" };
	const char* expresults[7] = { "", "CO======", "CPNG====", "CPNMU===", "CPNMUOG=", "CPNMUOJ1", "CPNMUOJ1E8======" };
};

struct base32hex_decode_test_set
{
	const char* testtext[7] = { "", "CO======", "CPNG====", "CPNMU===", "CPNMUOG=", "CPNMUOJ1", "CPNMUOJ1E8======" };
	const char* expresults[7] = { "", "f", "fo", "foo", "foob", "fooba", "foobar" };
};

///////////////
struct ascii85_encode_test_set
{
	//const char* testtext[1] = { "D" };
	//const char* expresults[1] = { "6i" };
	const char* testtext[6] = { "!", "!2", "!2Z", "!2Z0", "R!2Z0", "R!2Z    X" };
	const char* expresults[6] = { "+T", "+Yj", "+Ym4", "+Ym4T", ";B[f!0E", ";B[f!+<VdL=9" };
};

struct ascii85_decode_test_set
{
	const char* testtext[6] = { "+T", "+Yj", "+Ym4", "+Ym4T", ";B[f!0E", ";B[f!+<VdL=9" };
	const char* expresults[6] = { "!", "!2", "!2Z", "!2Z0", "R!2Z0", "R!2Z    X" };
};

void base64_test()
{
	encode_base_test(base64_encode_test_set{}, cvt::int8() >> cvt::base64<'='>() );
	decode_base_test(base64_decode_test_set{}, cvt::base64() >> cvt::int8() );
}

void base32_test()
{
	encode_base_test(base32_encode_test_set{}, cvt::int8() >> cvt::base32('='));
	decode_base_test(base32_decode_test_set{}, cvt::base32() >> cvt::int8());
}

void base32hex_test()
{
	encode_base_test(base32hex_encode_test_set{}, cvt::int8() >> cvt::base32hex());
	decode_base_test(base32hex_decode_test_set{}, cvt::base32hex() >> cvt::int8());
}

void ascii85_test()
{
	encode_base_test(ascii85_encode_test_set{}, cvt::int8() >> cvt::ascii85);
	decode_base_test(ascii85_decode_test_set{}, cvt::ascii85 >> cvt::int8());
}

void generate_matric();
void base64_test_registrar()
{
	register_test(BOOST_TEST_CASE(&base64_test));
	register_test(BOOST_TEST_CASE(&base32_test));
	register_test(BOOST_TEST_CASE(&base32hex_test));
	register_test(BOOST_TEST_CASE(&ascii85_test));
	//generate_matric();
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(base64_test_registrar)
#endif


void generate_matric()
{
	//const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
	const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
	uint8_t matrix[256];
	std::fill(matrix, matrix + sizeof(matrix), 255);
	for (size_t i = 0; i < sizeof(alphabet) - 1; ++i) {
		matrix[alphabet[i]] = (uint8_t)i;
	}

	std::ostringstream res;
	res << std::hex;
	for (size_t i = 0; i < sizeof(matrix); ++i) {
		res << "0x";
		uint8_t e = matrix[i];
		if (e < 16) res << "0";
		res << (int)e << ",";
		res << (((i + 1) % 16 == 0) ? "\n" : " ");
	}
	std::cout << res.str();
}
