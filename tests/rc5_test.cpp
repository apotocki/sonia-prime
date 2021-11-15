/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include "sonia/config.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "sonia/utility/conversion/rc5/rc5.hpp"
#include "sonia/utility/conversion/utility.hpp"

#include <boost/range/iterator_range.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/size.hpp>

namespace cvt = sonia::conversion;

template <typename ConverterT, typename IrangeT, typename VrangeT>
void conv_pull_test(ConverterT const& enc, IrangeT irange, VrangeT vrange, std::string const& descr = "")
{
	try {
		typedef typename boost::range_value<VrangeT>::type outtype_t;
		typedef std::basic_string<outtype_t> result_t;

		result_t result;
        cvt::pull(irange, std::back_inserter(result), enc);

        bool r = result.size() == (size_t)boost::size(vrange);
        BOOST_CHECK_MESSAGE(r, (descr + "[pull-test][range size inconsistency]").c_str());
		if (r) {
            BOOST_CHECK_MESSAGE(
                std::equal(result.begin(), result.end(), boost::begin(vrange)),
                (descr + "[pull-test][range inconsistency]").c_str());
		}

		/*
		static const int digits = boost::integer_traits<outtype_t>::digits + (boost::integer_traits<outtype_t>::is_signed?1:0);
		for (size_t i = 0; i < result.size(); ++i)
		{
			std::cout << "," + !i << " " + !i << "0x" << std::hex << std::setw((digits+3)/4) << std::setfill('0')
				<< (boost::low_bits_mask_t<digits>::sig_bits_fast & result[i]);
		}
		std::cout << "\n";
		//*/
	} catch(std::exception & exp) {
		BOOST_ERROR(descr + "[pull-test] " + exp.what());
	}
}

template <typename ConverterT, typename IrangeT, typename VrangeT>
void conv_push_test(ConverterT const& enc, IrangeT irange, VrangeT vrange, std::string const& descr = "")
{
	try {
		typedef typename boost::range_value<VrangeT>::type outtype_t;
		typedef std::basic_string<outtype_t> result_t;

        result_t result;
        cvt::push(irange, std::back_inserter(result), enc);

		/*
		static const int digits = boost::integer_traits<outtype_t>::digits + (boost::integer_traits<outtype_t>::is_signed?1:0);
		for (size_t i = 0; i < result.size(); ++i)
		{
			std::cout << "," + !i << " " + !i << "0x" << std::hex << std::setw((digits+3)/4) << std::setfill('0')
				<< (boost::low_bits_mask_t<digits>::sig_bits_fast & result[i]);
		}
		std::cout << "\n";
		//*/
		if (result.size() == (size_t)boost::size(vrange)) {
			if (!std::equal(result.begin(), result.end(), boost::begin(vrange))) {
				BOOST_ERROR((descr + "[push-test][range inconsistency]").c_str());
			}
		} else {
			BOOST_ERROR((descr + "[push-test][range size inconsistency]").c_str());
		}
	} catch(std::exception & exp) {
		BOOST_ERROR(descr + "[push-test] " + exp.what());
	}
}

void rc5_32_12_5()
{
	unsigned char password[] = {0x27, 0xd8, 0x6d, 0xd2, 0x43};
	unsigned char iv[] = {0xf6, 0x75, 0x17, 0x1a, 0x59, 0xb7, 0xea, 0xd0};
	const std::string message = "The unknown message is: The RC5-32/12/5-test contest's plaintext\x8\x8\x8\x8\x8\x8\x8\x8";
	unsigned char rc5_32_12_5_cipher[] = {
		0xb4, 0x0a, 0x53, 0x88, 0xb1, 0x38, 0x82, 0xad, 0x93, 0x0d, 0xa8, 0xb6, 0x1e, 0x51, 0xa3, 0xb1,
		0xff, 0xae, 0xe1, 0xd8, 0x14, 0xa5, 0xfa, 0x15, 0x40, 0x0b, 0x66, 0x3d, 0x58, 0x13, 0x8a, 0x33,
		0x19, 0x60, 0x3c, 0x85, 0x10, 0xc7, 0xb0, 0xd4, 0x87, 0x91, 0x8e, 0x32, 0xf8, 0x34, 0x77, 0x5b,
		0x20, 0x65, 0xe9, 0x74, 0x33, 0xb6, 0xea, 0xda, 0x54, 0x8a, 0xd9, 0xd7, 0xf7, 0x08, 0xf6, 0x45,
		0xc2, 0x33, 0xce, 0x8a, 0x83, 0xb2, 0x76, 0xe0
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 5>(password, iv, '\x8'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_5_cipher), "[rc5_32_12_5_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 5>(password, iv, '\x8'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_5_cipher), "[rc5_32_12_5_enc]");

	conv_pull_test(cvt::rc5<32, 12, 5>(password, iv, '\x8') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_5_cipher), boost::make_iterator_range(message), "[rc5_32_12_5_dec]");
	conv_push_test(cvt::rc5<32, 12, 5>(password, iv, '\x8') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_5_cipher), boost::make_iterator_range(message), "[rc5_32_12_5_dec]");
}

void rc5_32_12_6()
{
	unsigned char password[] = {0x28, 0x26, 0x04, 0x78, 0x06, 0x51};
	unsigned char iv[] = {0x23, 0x8c, 0x3c, 0xc9, 0xb0, 0xfd, 0x9f, 0x9e};
	const std::string message = "The unknown message is: The RC5-32/12/6-test contest's plaintext\x8\x8\x8\x8\x8\x8\x8\x8";
	unsigned char rc5_32_12_6_cipher[] = {
		0xbf, 0x88, 0x86, 0xd2, 0xa9, 0x50, 0x84, 0x1c, 0xb8, 0x83, 0xf3, 0xd4, 0xcf, 0xd9, 0x55, 0x1f,
		0x98, 0xab, 0xd2, 0xe6, 0xbf, 0x40, 0xd4, 0x92, 0x91, 0x85, 0x2f, 0x28, 0xd0, 0x89, 0xfb, 0xb4,
		0xf0, 0xf9, 0xc3, 0x27, 0xa6, 0xc5, 0xaa, 0xa2, 0xdf, 0x4f, 0x36, 0x3f, 0x17, 0xfb, 0xcf, 0x1e,
		0xfc, 0x6e, 0x65, 0x85, 0x30, 0xf8, 0x5e, 0x02, 0x00, 0x58, 0x31, 0xb1, 0xe0, 0x67, 0x2e, 0x49,
		0x50, 0xcb, 0xea, 0x3c, 0x95, 0x1d, 0x46, 0x31
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 6>(password, iv, '\x8'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_6_cipher), "[rc5_32_12_6_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 6>(password, iv, '\x8'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_6_cipher), "[rc5_32_12_6_enc]");

	conv_pull_test(cvt::rc5<32, 12, 6>(password, iv, '\x8') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_6_cipher), boost::make_iterator_range(message), "[rc5_32_12_6_dec]");
	conv_push_test(cvt::rc5<32, 12, 6>(password, iv, '\x8') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_6_cipher), boost::make_iterator_range(message), "[rc5_32_12_6_dec]");
}

void rc5_32_12_7()
{
	unsigned char password[] = {0x14, 0x11, 0xb3, 0x5a, 0x9a, 0x98, 0xda};
	unsigned char iv[] = {0xfb, 0xb2, 0xe9, 0xe4, 0x12, 0xd0, 0xec, 0xbc};
	const std::string message = "The unknown message is: The RC5-32/12/7-test contest's plaintext\x8\x8\x8\x8\x8\x8\x8\x8";
	unsigned char rc5_32_12_7_cipher[] = {
		0x39, 0xde, 0x1b, 0x56, 0x3f, 0x1d, 0x5c, 0x25, 0xe7, 0xe8, 0xce, 0xbb, 0xe7, 0x2a, 0xec, 0x22,
		0x6c, 0xb1, 0x90, 0x34, 0xc1, 0xef, 0x0f, 0xd3, 0x84, 0x77, 0xd9, 0x6d, 0x52, 0xd9, 0x34, 0x50,
		0xf8, 0x08, 0xe8, 0xb2, 0xf5, 0x6a, 0xdc, 0xcb, 0xe5, 0xdf, 0x16, 0x01, 0xe2, 0xc0, 0x0a, 0x25,
		0x23, 0xdf, 0x22, 0x67, 0xd6, 0x60, 0x93, 0x4a, 0x39, 0x68, 0xc1, 0x42, 0x6c, 0x69, 0x73, 0x2f,
		0x3b, 0x4a, 0x4e, 0x25, 0x35, 0xc2, 0xce, 0xc9
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 7>(password, iv, '\x8'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_7_cipher), "[rc5_32_12_7_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 7>(password, iv, '\x8'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_7_cipher), "[rc5_32_12_7_enc]");

	conv_pull_test(cvt::rc5<32, 12, 7>(password, iv, '\x8') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_7_cipher), boost::make_iterator_range(message), "[rc5_32_12_7_dec]");
	conv_push_test(cvt::rc5<32, 12, 7>(password, iv, '\x8') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_7_cipher), boost::make_iterator_range(message), "[rc5_32_12_7_dec]");
}

void rc5_32_12_8()
{
	unsigned char password[] = {0x82, 0xe5, 0x1b, 0x9f, 0x9c, 0xc7, 0x18, 0xf9};
	unsigned char iv[] = {0xd9, 0xa5, 0x39, 0xf8, 0xc1, 0x78, 0x1f, 0xc4};
	const std::string message = "The unknown message is: The RC5-32/12/8-test contest's plaintext\x8\x8\x8\x8\x8\x8\x8\x8";
	unsigned char rc5_32_12_8_cipher[] = {
		0x41, 0xe0, 0x4b, 0xb7, 0x29, 0xef, 0x6d, 0x49, 0xb9, 0x85, 0x72, 0x64, 0x52, 0xfe, 0x90, 0x11,
		0xb0, 0xbf, 0xa4, 0xb2, 0x5b, 0xae, 0x2a, 0x48, 0x0c, 0xa4, 0x81, 0x7a, 0xe8, 0x09, 0x0f, 0xba,
		0x5a, 0x13, 0x41, 0xe0, 0xbc, 0xc3, 0x8a, 0xe8, 0x16, 0x87, 0x4e, 0xad, 0xc6, 0xdf, 0x53, 0x87,
		0xd2, 0x37, 0x4a, 0x82, 0x14, 0x65, 0xcc, 0xb8, 0x2c, 0x92, 0x92, 0xd1, 0x28, 0xf3, 0xba, 0xef,
		0x10, 0x19, 0x2c, 0x0f, 0x39, 0xc7, 0xa7, 0x6d
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 8>(password, iv, '\x8'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_8_cipher), "[rc5_32_12_8_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 8>(password, iv, '\x8'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_8_cipher), "[rc5_32_12_8_enc]");

	conv_pull_test(cvt::rc5<32, 12, 8>(password, iv, '\x8') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_8_cipher), boost::make_iterator_range(message), "[rc5_32_12_8_dec]");
	conv_push_test(cvt::rc5<32, 12, 8>(password, iv, '\x8') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_8_cipher), boost::make_iterator_range(message), "[rc5_32_12_8_dec]");
}

void rc5_32_12_9()
{
	unsigned char password[] = {0xc9, 0x0c, 0x03, 0x53, 0xc0, 0xd4, 0xe1, 0xfe, 0x85};
	unsigned char iv[] = {0x07, 0xce, 0x59, 0x1f, 0x86, 0x14, 0x9a, 0x41};
	const std::string message = "The unknown message is: The RC5-32/12/9-test contest's plaintext\x8\x8\x8\x8\x8\x8\x8\x8";
	unsigned char rc5_32_12_9_cipher[] = {
		0x5a, 0x28, 0x2d, 0x56, 0x2a, 0x85, 0xb7, 0x2f, 0xef, 0xae, 0x30, 0xba, 0x93, 0x54, 0xda, 0x15,
		0xac, 0xba, 0x79, 0x1a, 0xa1, 0xcb, 0x0e, 0x85, 0x9a, 0xc3, 0x31, 0xa5, 0x36, 0x19, 0x85, 0xe3,
		0x77, 0xdf, 0xdd, 0x07, 0x34, 0x3b, 0xfa, 0xed, 0x84, 0x84, 0x41, 0x90, 0x9d, 0x7b, 0x6f, 0x17,
		0x2d, 0x51, 0xea, 0xb2, 0x1c, 0x5e, 0x45, 0x1c, 0xe9, 0x6a, 0x34, 0xde, 0xa5, 0xf5, 0xb9, 0x56,
		0x22, 0x16, 0xe2, 0x60, 0x15, 0xe1, 0x7e, 0xb2
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 9>(password, iv, '\x8'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_9_cipher), "[rc5_32_12_9_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 9>(password, iv, '\x8'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_9_cipher), "[rc5_32_12_9_enc]");

	conv_pull_test(cvt::rc5<32, 12, 9>(password, iv, '\x8') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_9_cipher), boost::make_iterator_range(message), "[rc5_32_12_9_dec]");
	conv_push_test(cvt::rc5<32, 12, 9>(password, iv, '\x8') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_9_cipher), boost::make_iterator_range(message), "[rc5_32_12_9_dec]");
}

void rc5_32_12_10()
{
	unsigned char password[] = {0xa4, 0xac, 0x9b, 0xaf, 0xac, 0xb1, 0x27, 0x5f, 0xb9, 0x6f};
	unsigned char iv[] = {0x31, 0xef, 0x08, 0x46, 0xe1, 0x43, 0xdd, 0xa7};
	const std::string message = "The unknown message is: The RC5-32/12/10-test contest's plaintext\x7\x7\x7\x7\x7\x7\x7";
	unsigned char rc5_32_12_10_cipher[] = {
		0x4d, 0xf8, 0x76, 0xd7, 0xe6, 0x49, 0x1a, 0xc7, 0xd6, 0x05, 0xe9, 0x37, 0xb5, 0xc2, 0x13, 0xb9,
		0x29, 0x6f, 0x6c, 0xc1, 0x20, 0x18, 0x57, 0x97, 0x6a, 0x37, 0xf1, 0x96, 0x63, 0xb0, 0x7a, 0x7d,
		0x4f, 0x69, 0x96, 0x97, 0x9a, 0xa9, 0x6d, 0x62, 0x2e, 0xd2, 0xca, 0x83, 0x07, 0x74, 0x43, 0xfc,
		0xbe, 0x23, 0x4e, 0x6d, 0xcf, 0x2f, 0x93, 0x06, 0xa3, 0x26, 0x67, 0xaa, 0x61, 0x0c, 0xd8, 0x1d,
		0x60, 0xca, 0xe8, 0xf5, 0xb1, 0xd9, 0x74, 0x49
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 10>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_10_cipher), "[rc5_32_12_10_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 10>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_10_cipher), "[rc5_32_12_10_enc]");

	conv_pull_test(cvt::rc5<32, 12, 10>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_10_cipher), boost::make_iterator_range(message), "[rc5_32_12_10_enc]");
	conv_push_test(cvt::rc5<32, 12, 10>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_10_cipher), boost::make_iterator_range(message), "[rc5_32_12_10_enc]");
}

void rc5_32_12_11()
{
	unsigned char password[] = {0x65, 0x82, 0x8d, 0xe0, 0x89, 0x1d, 0x6a, 0x88, 0xbe, 0xc9, 0xcd};
	unsigned char iv[] = {0x09, 0xfb, 0x2f, 0x69, 0x87, 0xb2, 0xf2, 0xfd};
	const std::string message = "The unknown message is: The RC5-32/12/11-test contest's plaintext\x7\x7\x7\x7\x7\x7\x7";
	unsigned char rc5_32_12_11_cipher[] = {
		0x76, 0x85, 0x6d, 0xdd, 0x78, 0x66, 0x9a, 0x9a, 0xa2, 0x57, 0xb5, 0x3c, 0xf9, 0x65, 0x7a, 0x44,
		0x2a, 0x2c, 0xf4, 0x8b, 0x8e, 0xd8, 0x7a, 0x68, 0x36, 0xc2, 0xcb, 0x6f, 0xed, 0x11, 0x65, 0x01,
		0xd9, 0x3e, 0x3a, 0xcc, 0xdb, 0x29, 0xaa, 0xd2, 0x82, 0x5a, 0xa1, 0x9a, 0x9b, 0xb4, 0xdf, 0x79,
		0xe5, 0xa1, 0x35, 0xd7, 0xd5, 0x62, 0x3e, 0x59, 0x4e, 0xb8, 0xb1, 0x1e, 0x56, 0xe0, 0xfd, 0xa0,
		0x1a, 0x0a, 0xd6, 0xc5, 0x1f, 0x6c, 0xdc, 0x62
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 11>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_11_cipher), "[rc5_32_12_11_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 11>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_11_cipher), "[rc5_32_12_11_enc]");

	conv_pull_test(cvt::rc5<32, 12, 11>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_11_cipher), boost::make_iterator_range(message), "[rc5_32_12_11_enc]");
	conv_push_test(cvt::rc5<32, 12, 11>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_11_cipher), boost::make_iterator_range(message), "[rc5_32_12_11_enc]");
}

void rc5_32_12_12()
{
	unsigned char password[] = {0x67, 0x40, 0x23, 0x1b, 0x6d, 0x1a, 0x7e, 0x0c, 0x92, 0x96, 0x05, 0xc2};
	unsigned char iv[] = {0xf0, 0xed, 0xe8, 0x46, 0x43, 0x48, 0xbd, 0x62};
	const std::string message = "The unknown message is: The RC5-32/12/12-test contest's plaintext\x7\x7\x7\x7\x7\x7\x7";
	unsigned char rc5_32_12_12_cipher[] = {
		0xae, 0x0e, 0xe8, 0x11, 0xdf, 0xfd, 0xc8, 0xd2, 0x06, 0xe9, 0x2e, 0xb4, 0xda, 0x75, 0x04, 0xfc,
		0x33, 0x17, 0x47, 0x91, 0x98, 0x50, 0x0f, 0xf5, 0x2e, 0x70, 0x5b, 0xf7, 0x38, 0x8b, 0x24, 0x53,
		0x34, 0x86, 0x40, 0x95, 0x44, 0x46, 0xd5, 0x0a, 0x67, 0x1a, 0x2b, 0xc0, 0x2f, 0x84, 0xdf, 0x92,
		0x64, 0x74, 0x8e, 0xda, 0x1a, 0x4b, 0xa6, 0xe3, 0x1a, 0xc8, 0x67, 0xd3, 0x52, 0x58, 0xa4, 0xb7,
		0x54, 0x7f, 0xc4, 0x83, 0xbf, 0xad, 0x1f, 0xfe
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 12>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_12_cipher), "[rc5_32_12_12_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 12>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_12_cipher), "[rc5_32_12_12_enc]");

	conv_pull_test(cvt::rc5<32, 12, 12>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_12_cipher), boost::make_iterator_range(message), "[rc5_32_12_12_enc]");
	conv_push_test(cvt::rc5<32, 12, 12>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_12_cipher), boost::make_iterator_range(message), "[rc5_32_12_12_enc]");
}

void rc5_32_12_13()
{
	unsigned char password[] = {0x32, 0x32, 0xd5, 0x0c, 0xc6, 0x46, 0x8b, 0xf4, 0x3e, 0xb9, 0xcb, 0xba, 0x82};
	unsigned char iv[] = {0xdb, 0xce, 0x3b, 0x13, 0xf0, 0x81, 0xaf, 0x9e};
	const std::string message = "The unknown message is: The RC5-32/12/13-test contest's plaintext\x7\x7\x7\x7\x7\x7\x7";
	unsigned char rc5_32_12_13_cipher[] = {
		0x50, 0x63, 0x53, 0x2e, 0xdc, 0xb0, 0x5a, 0xc4, 0xf6, 0xe0, 0x13, 0x7e, 0xbb, 0x0b, 0xf1, 0xe4,
		0x9b, 0x0c, 0x2b, 0xed, 0xc3, 0xc1, 0xf8, 0x34, 0x6a, 0x09, 0x63, 0xad, 0xca, 0x8f, 0xa3, 0x9b,
		0x30, 0xec, 0xfc, 0xba, 0x21, 0x63, 0x5d, 0xd8, 0xdf, 0x37, 0xa0, 0xbc, 0x7c, 0x24, 0xd6, 0xb0,
		0xea, 0xe1, 0x05, 0x04, 0x56, 0xce, 0x35, 0x6b, 0x4f, 0xb7, 0xa7, 0x23, 0x08, 0xd1, 0xa3, 0x5b,
		0xf3, 0x3d, 0x7f, 0x9a, 0x57, 0x96, 0xb4, 0x50
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 13>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_13_cipher), "[rc5_32_12_13_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 13>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_13_cipher), "[rc5_32_12_13_enc]");

	conv_pull_test(cvt::rc5<32, 12, 13>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_13_cipher), boost::make_iterator_range(message), "[rc5_32_12_13_enc]");
	conv_push_test(cvt::rc5<32, 12, 13>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_13_cipher), boost::make_iterator_range(message), "[rc5_32_12_13_enc]");
}

void rc5_32_12_14()
{
	unsigned char password[] = {0xb5, 0x6e, 0x61, 0x11, 0x16, 0x02, 0xc4, 0x46, 0xfd, 0x45, 0x2b, 0xa8, 0x15, 0x9c};
	unsigned char iv[] = {0x64, 0xc0, 0xb2, 0xff, 0x53, 0x23, 0x51, 0xb3};
	const std::string message = "The unknown message is: The RC5-32/12/14-test contest's plaintext\x7\x7\x7\x7\x7\x7\x7";
	unsigned char rc5_32_12_14_cipher[] = {
		0xdf, 0x0b, 0x2e, 0x7a, 0x13, 0x29, 0x63, 0x2b, 0xd6, 0x81, 0xef, 0xf8, 0x64, 0x76, 0x58, 0x64,
		0xaa, 0x48, 0xac, 0x91, 0xf8, 0xbd, 0x9c, 0xf8, 0xea, 0xb8, 0x2c, 0xe6, 0xc3, 0xfb, 0xac, 0x54,
		0x88, 0x2e, 0xd1, 0xa1, 0x22, 0xd8, 0x1b, 0x0e, 0x5e, 0xd3, 0x5b, 0x0f, 0x07, 0x3d, 0x17, 0xfc,
		0xd2, 0xe5, 0xf9, 0x37, 0x35, 0xaf, 0xfc, 0xae, 0x00, 0x09, 0x19, 0x29, 0x7f, 0x3b, 0xf1, 0x5f,
		0x5d, 0xf3, 0xa2, 0x8d, 0x9e, 0xf9, 0x9d, 0x4f
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 14>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_14_cipher), "[rc5_32_12_14_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 14>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_14_cipher), "[rc5_32_12_14_enc]");

	conv_pull_test(cvt::rc5<32, 12, 14>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_14_cipher), boost::make_iterator_range(message), "[rc5_32_12_14_enc]");
	conv_push_test(cvt::rc5<32, 12, 14>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_14_cipher), boost::make_iterator_range(message), "[rc5_32_12_14_enc]");
}

void rc5_32_12_15()
{
	unsigned char password[] = {0x4f, 0x74, 0xb5, 0xf4, 0x4d, 0x24, 0x4e, 0xce, 0xb4, 0x24, 0xa8, 0x31, 0x5e, 0xc7, 0x0f};
	unsigned char iv[] = {0xfa, 0xc9, 0x86, 0xa0, 0x90, 0x48, 0x30, 0xa2};
	const std::string message = "The unknown message is: The RC5-32/12/15-test contest's plaintext\x7\x7\x7\x7\x7\x7\x7";
	unsigned char rc5_32_12_15_cipher[] = {
		0x8e, 0xd7, 0x83, 0xd4, 0x97, 0x00, 0x67, 0x0b, 0x98, 0xac, 0x58, 0x18, 0xc1, 0xdb, 0x75, 0xbf,
		0xe0, 0x3c, 0x0a, 0x6e, 0x1c, 0x5b, 0xcc, 0xb9, 0xa5, 0xa9, 0x07, 0xbc, 0xdf, 0xc6, 0xf9, 0x6a,
		0x80, 0x1f, 0x3c, 0x98, 0x78, 0x21, 0x6f, 0xe3, 0x0d, 0x2e, 0x9b, 0x8d, 0xb3, 0x9c, 0x85, 0x2a,
		0x97, 0x03, 0x61, 0x30, 0x2b, 0xce, 0xbb, 0x55, 0xce, 0x19, 0x62, 0x7f, 0xd9, 0x69, 0xb5, 0x55,
		0x5e, 0xb4, 0x63, 0xde, 0xd7, 0xf8, 0x1e, 0xa3
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 15>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_15_cipher), "[rc5_32_12_15_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 15>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_15_cipher), "[rc5_32_12_15_enc]");

	conv_pull_test(cvt::rc5<32, 12, 15>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_15_cipher), boost::make_iterator_range(message), "[rc5_32_12_15_enc]");
	conv_push_test(cvt::rc5<32, 12, 15>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_15_cipher), boost::make_iterator_range(message), "[rc5_32_12_15_enc]");
}

void rc5_32_12_16()
{
	unsigned char password[] = {0xb1, 0x6c, 0x2a, 0xb9, 0x74, 0xfb, 0x5e, 0x48, 0x7f, 0x8a, 0x65, 0xd7, 0xf9, 0xe4, 0x0c, 0x62};
	unsigned char iv[] = {0x0b, 0x3c, 0x9d, 0x50, 0xfd, 0x06, 0xf9, 0xf1};
	const std::string message = "The unknown message is: The RC5-32/12/16-test contest's plaintext\x7\x7\x7\x7\x7\x7\x7";
	unsigned char rc5_32_12_16_cipher[] = {
		0xde, 0x4e, 0xae, 0xe9, 0x6c, 0xf5, 0x85, 0xc2, 0x36, 0xff, 0x6f, 0xb6, 0x9c, 0x50, 0xda, 0xd5,
		0x6a, 0x57, 0xfe, 0xb6, 0xd2, 0x41, 0x28, 0x60, 0x73, 0x04, 0x89, 0xca, 0x36, 0xc8, 0x4f, 0x7c,
		0xaa, 0xb0, 0x35, 0x3c, 0x63, 0xd0, 0xcb, 0xe0, 0xc4, 0xe3, 0x0d, 0x79, 0x77, 0xe9, 0xc6, 0x48,
		0x7f, 0x13, 0xa0, 0xda, 0x11, 0xee, 0x38, 0xb9, 0x97, 0x2e, 0xa5, 0x3d, 0x3b, 0x48, 0x4f, 0x3e,
		0x9a, 0x93, 0x37, 0x18, 0x87, 0x7c, 0x37, 0xac
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 16>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_16_cipher), "[rc5_32_12_16_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 16>(password, iv, '\x7'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_16_cipher), "[rc5_32_12_16_enc]");

	conv_pull_test(cvt::rc5<32, 12, 16>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_16_cipher), boost::make_iterator_range(message), "[rc5_32_12_16_enc]");
	conv_push_test(cvt::rc5<32, 12, 16>(password, iv, '\x7') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_16_cipher), boost::make_iterator_range(message), "[rc5_32_12_16_enc]");
}

//
void rc5_32_12_8_bonus()
{
	unsigned char password[] = {0x63, 0xDE, 0x7D, 0xC1, 0x54, 0xF4, 0xD0, 0x39};
	unsigned char iv[] = {0x79, 0xce, 0xd5, 0xd5, 0x50, 0x75, 0xea, 0xfc };
	const std::string message = "The unknown message is: Some things are better left unread.\x5\x5\x5\x5\x5";
	unsigned char rc5_32_12_8_cipher[] = {
		0xbf, 0x55, 0x01, 0x55, 0xdc, 0x26, 0xf2, 0x4b, 0x26, 0xe4, 0x85, 0x4d, 0xf9, 0x0a, 0xd6, 0x79,
		0x66, 0x93, 0xab, 0x92, 0x3c, 0x72, 0xf1, 0x37, 0xc8, 0xb7, 0x0d, 0x1f, 0x60, 0x11, 0x0c, 0x92,
		0xae, 0x2e, 0xcd, 0xfd, 0x70, 0xd3, 0xfd, 0x17, 0xdf, 0xb0, 0x42, 0x12, 0xb9, 0x7d, 0xcf, 0x22,
		0x18, 0x6b, 0xa7, 0x15, 0xce, 0x2c, 0x84, 0xbf, 0xce, 0x0d, 0xd0, 0x4d, 0x00, 0x6b, 0xe1, 0x46
	};
	conv_pull_test(cvt::int8() >> cvt::rc5<32, 12, 8>(password, iv, '\x8'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_8_cipher), "[rc5_32_12_8b_enc]");
	conv_push_test(cvt::int8() >> cvt::rc5<32, 12, 8>(password, iv, '\x8'), boost::make_iterator_range(message), boost::make_iterator_range(rc5_32_12_8_cipher), "[rc5_32_12_8b_enc]");

	conv_pull_test(cvt::rc5<32, 12, 8>(password, iv, '\x8') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_8_cipher), boost::make_iterator_range(message), "[rc5_32_12_8b_dec]");
	conv_push_test(cvt::rc5<32, 12, 8>(password, iv, '\x8') >> cvt::int8(), boost::make_iterator_range(rc5_32_12_8_cipher), boost::make_iterator_range(message), "[rc5_32_12_8b_dec]");
}

BOOST_AUTO_TEST_CASE( rc5_test )
{
    rc5_32_12_5();
	rc5_32_12_6();
	rc5_32_12_7();
	rc5_32_12_8();
	rc5_32_12_9();
	rc5_32_12_10();
	rc5_32_12_11();
	rc5_32_12_12();
	rc5_32_12_13();
	rc5_32_12_14();
	rc5_32_12_15();
	rc5_32_12_16();
	rc5_32_12_8_bonus();
}

/*
#include <fstream>
void file_test()
{
    // to encrypt file
	{
		std::ifstream input("file_to_encrypt.exe", std::ios::binary);
		std::ofstream output("encrypted_file.exe", std::ios::binary);
		unsigned char password[] = {0x63, 0xDE, 0x7D, 0xC1, 0x54, 0xF4, 0xD0, 0x39};

        cvt::pull( // or cvt::push
            std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(output),
            cvt::int8() >> cvt::rc5<32, 12, 8>(password)
        );
	}

    // to decrypt file
	{
		std::ifstream input("encrypted_file.exe", std::ios::binary);
		std::ofstream output("decrypted.exe", std::ios::binary);
		unsigned char password[] = {0x63, 0xDE, 0x7D, 0xC1, 0x54, 0xF4, 0xD0, 0x39};

        cvt::push( // or cvt::pull
            std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(output),
            cvt::rc5<32, 12, 8>(password) >> cvt::int8()
        );
	}

    // !!! note: files are padded
}
*/
