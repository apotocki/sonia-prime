/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include "sonia/config.hpp"

#include <iostream>

#include "sonia/utility/conversion/base64/base64.hpp"
#include "sonia/utility/conversion/utility.hpp"

#include "applied/sonia_test.hpp"

namespace cvt = sonia::conversion;

template <typename ConverterT>
void encode_base64test(ConverterT const& enc)
{
	try {
		const char * testtext[] = {"12345678", "123456789", "123456789a", "123456789ab"};
		const char * expresults[] = {"MTIzNDU2Nzg=", "MTIzNDU2Nzg5", "MTIzNDU2Nzg5YQ==", "MTIzNDU2Nzg5YWI="};

		for (size_t i = 0; i < sizeof(testtext)/sizeof(char*); ++i)
		{
			const char * begin = testtext[i];
			const char * end = begin + strlen(testtext[i]);

            std::string result;
            cvt::push(begin, end, std::back_inserter(result), enc);
            BOOST_CHECK(result == expresults[i]);

            std::string result2;
            cvt::pull(begin, end, std::back_inserter(result2), enc);
            BOOST_CHECK(result2 == expresults[i]);
		}
	} catch(std::exception & exp) {
		BOOST_ERROR(exp.what());
	}
}

template <typename ConverterT>
void decode_base64test(ConverterT const& enc)
{
	try {
		const char * testtext[] = {"MTIzNDU2Nzg=", "MTIzNDU2Nzg5", "MTIzNDU2Nzg5YQ==", "MTIzNDU2Nzg5YWI="};
		const char * expresults[] = {"12345678", "123456789", "123456789a", "123456789ab"};

		typedef cvt::convert_input_iterator<ConverterT, const char*> it_t;
        typedef cvt::convert_output_iterator<ConverterT, std::back_insert_iterator<std::string> > oit_t;

		for (size_t i = 0; i < sizeof(testtext)/sizeof(char*); ++i)
		{
			const char * begin = testtext[i];
			const char * end = begin + strlen(testtext[i]);

            std::string result;
            cvt::push(begin, end, std::back_inserter(result), enc);
            BOOST_CHECK(result == expresults[i]);

            std::string result2;
            cvt::pull(begin, end, std::back_inserter(result2), enc);
            BOOST_CHECK(result2 == expresults[i]);
		}
	} catch(std::exception & exp) {
		BOOST_ERROR(exp.what());
	}
}

void base64_test()
{
    encode_base64test( cvt::int8() >> cvt::base64<'='>() );
	decode_base64test( cvt::base64() >> cvt::int8() );
}

void base64_test_registrar()
{
	register_test(BOOST_TEST_CASE(&base64_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(base64_test_registrar)
#endif
