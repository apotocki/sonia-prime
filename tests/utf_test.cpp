/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include "sonia/config.hpp"

#include <iostream>

#include "sonia/utility/conversion/utf32/utf32.hpp"
#include "sonia/utility/conversion/utf16/utf16.hpp"
#include "sonia/utility/conversion/utf8/utf8.hpp"
#include "sonia/utility/conversion/utf7/utf7.hpp"
#include "sonia/utility/conversion/cp/cp.hpp"

#include "sonia/utility/conversion/utility.hpp"

#include <boost/range/iterator_range.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/size.hpp>

#include "applied/sonia_test.hpp"

namespace cvt = sonia::conversion;

// errors
/*0x2F, 0xC0, 0xAE, 0x2E, 0x2F,*/ /*0xED, 0xA1, 0x8C, 0xED, 0xBE, 0xB4,*/

const unsigned char utf8text[] = {
	//0xef, 0xbb, 0xbf,
    0x65, 0x6e, 0x67, 0x6c, 0x69, 0x73, 0x68, 0x3a, 0x20, 0x61, 0x62, 0x63, 0x64,
	0x65, 0x66, 0x67, 0x68, 0x2c, 0x20, 0xd1, 0x80, 0xd1, 0x83, 0xd1, 0x81, 0xd1, 0x81, 0xd0, 0xba,
	0xd0, 0xb8, 0xd0, 0xb9, 0x3a, 0x20, 0xd0, 0xb0, 0xd0, 0xb1, 0xd0, 0xb2, 0xd0, 0xb3, 0xd0, 0xb4,
	0xd0, 0xb5, 0xd1, 0x91, 0xd0, 0xb6, 0xd0, 0xb7, 0xd0, 0xb8, 0xd0, 0x99, 0xd0, 0x9a, 0xd0, 0x9b,
	0xd0, 0x9c, 0xd0, 0x9d, 0xd0, 0x9e, 0xd0, 0x9f, 0xd0, 0xa0, 0xd0, 0xa1, 0xd0, 0xa2, 0xd0, 0xa3,
	0xd0, 0xa4, 0xd0, 0xa5, 0xd0, 0xa6, 0xd0, 0xa7, 0xd0, 0xa8, 0xd0, 0xa9, 0xd0, 0xaa, 0xd0, 0xab,
	0xd1, 0x8c, 0xd0, 0xad, 0xd0, 0xae, 0xd0, 0xaf,
	0xe6, 0x8c, 0x99,			//U+6319
	0xee, 0x8f, 0x99,			//U+e3d9
	0xf4, 0x8f, 0x95, 0x95,		//U+10f555
	0xf0, 0xA0, 0x80, 0xA7,     //U+20027
	0xf0, 0xA0, 0xA4, 0xA9     //U+20929
};

const char16_t utf16text[] = {
    //0xfeff,
	0x065, 0x06e, 0x067, 0x06c, 0x069, 0x073, 0x068, 0x03a,
	0x020, 0x061, 0x062, 0x063, 0x064, 0x065, 0x066, 0x067,
	0x068, 0x02c, 0x020, 0x440, 0x443, 0x441, 0x441, 0x43a,
	0x438, 0x439, 0x03a, 0x020, 0x430, 0x431, 0x432, 0x433,
	0x434, 0x435, 0x451, 0x436, 0x437, 0x438, 0x419, 0x41a,
	0x41b, 0x41c, 0x41d, 0x41e, 0x41f, 0x420, 0x421, 0x422,
	0x423, 0x424, 0x425, 0x426, 0x427, 0x428, 0x429, 0x42a,
	0x42b, 0x44c, 0x42d, 0x42e, 0x42f,
	0x6319, //U+6319
	0xe3d9, //U+e3d9
	0xdbfd, 0xdd55, //U+10f555
	0xd840, 0xdc27, //U+20027
	0xd842, 0xdd29  //U+20929
};

const unsigned char utf16le[] = {
    //0xfe, 0xff,
	0x65, 0x0, 0x6e, 0x0, 0x67, 0x0, 0x6c, 0x0, 0x69, 0x0, 0x73, 0x0, 0x68, 0x0,
	0x3a, 0x0, 0x20, 0x0, 0x61, 0x0, 0x62, 0x0, 0x63, 0x0, 0x64, 0x0, 0x65, 0x0, 0x66,
	0x0, 0x67, 0x0, 0x68, 0x0, 0x2c, 0x0, 0x20, 0x0, 0x40, 0x4, 0x43, 0x4, 0x41, 0x4,
	0x41, 0x4, 0x3a, 0x4, 0x38, 0x4, 0x39, 0x4, 0x3a, 0x0, 0x20, 0x0, 0x30, 0x4, 0x31,
	0x4, 0x32, 0x4, 0x33, 0x4, 0x34, 0x4, 0x35, 0x4, 0x51, 0x4, 0x36, 0x4, 0x37, 0x4,
	0x38, 0x4, 0x19, 0x4, 0x1a, 0x4, 0x1b, 0x4, 0x1c, 0x4, 0x1d, 0x4, 0x1e, 0x4, 0x1f,
	0x4, 0x20, 0x4, 0x21, 0x4, 0x22, 0x4, 0x23, 0x4, 0x24, 0x4, 0x25, 0x4, 0x26, 0x4,
	0x27, 0x4, 0x28, 0x4, 0x29, 0x4, 0x2a, 0x4, 0x2b, 0x4, 0x4c, 0x4, 0x2d, 0x4, 0x2e, 0x4, 0x2f, 0x4,
	0x19, 0x63, //U+6319
	0xd9, 0xe3, //U+e3d9
	0xfd, 0xdb, 0x55, 0xdd, //U+10f555
	0x40, 0xd8, 0x27, 0xdc, //U+20027
	0x42, 0xd8, 0x29, 0xdd  //U+20929
};

BOOST_STATIC_ASSERT(sizeof(utf16le)%2 == 0);

const unsigned char utf32le[] = {
    //0xff, 0xfe, 0x00, 0x0,
	0x65, 0x00, 0x00, 0x0, 0x6e, 0x0, 0x0, 0x0, 0x67, 0x0, 0x0, 0x0, 0x6c, 0x0, 0x0, 0x0,
	0x69, 0x00, 0x00, 0x0, 0x73, 0x0, 0x0, 0x0, 0x68, 0x0, 0x0, 0x0, 0x3a, 0x0, 0x0, 0x0,
	0x20, 0x00, 0x00, 0x0, 0x61, 0x0, 0x0, 0x0, 0x62, 0x0, 0x0, 0x0, 0x63, 0x0, 0x0, 0x0,
	0x64, 0x00, 0x00, 0x0, 0x65, 0x0, 0x0, 0x0, 0x66, 0x0, 0x0, 0x0, 0x67, 0x0, 0x0, 0x0,
	0x68, 0x00, 0x00, 0x0, 0x2c, 0x0, 0x0, 0x0, 0x20, 0x0, 0x0, 0x0, 0x40, 0x4, 0x0, 0x0,
	0x43, 0x04, 0x00, 0x0, 0x41, 0x4, 0x0, 0x0, 0x41, 0x4, 0x0, 0x0, 0x3a, 0x4, 0x0, 0x0,
	0x38, 0x04, 0x00, 0x0, 0x39, 0x4, 0x0, 0x0, 0x3a, 0x0, 0x0, 0x0, 0x20, 0x0, 0x0, 0x0,
	0x30, 0x04, 0x00, 0x0, 0x31, 0x4, 0x0, 0x0, 0x32, 0x4, 0x0, 0x0, 0x33, 0x4, 0x0, 0x0,
	0x34, 0x04, 0x00, 0x0, 0x35, 0x4, 0x0, 0x0, 0x51, 0x4, 0x0, 0x0, 0x36, 0x4, 0x0, 0x0,
	0x37, 0x04, 0x00, 0x0, 0x38, 0x4, 0x0, 0x0, 0x19, 0x4, 0x0, 0x0, 0x1a, 0x4, 0x0, 0x0,
	0x1b, 0x04, 0x00, 0x0, 0x1c, 0x4, 0x0, 0x0, 0x1d, 0x4, 0x0, 0x0, 0x1e, 0x4, 0x0, 0x0,
	0x1f, 0x04, 0x00, 0x0, 0x20, 0x4, 0x0, 0x0, 0x21, 0x4, 0x0, 0x0, 0x22, 0x4, 0x0, 0x0,
	0x23, 0x04, 0x00, 0x0, 0x24, 0x4, 0x0, 0x0, 0x25, 0x4, 0x0, 0x0, 0x26, 0x4, 0x0, 0x0,
	0x27, 0x04, 0x00, 0x0, 0x28, 0x4, 0x0, 0x0, 0x29, 0x4, 0x0, 0x0, 0x2a, 0x4, 0x0, 0x0,
	0x2b, 0x04, 0x00, 0x0, 0x4c, 0x4, 0x0, 0x0, 0x2d, 0x4, 0x0, 0x0, 0x2e, 0x4, 0x0, 0x0,
	0x2f, 0x04, 0x00, 0x0,
	0x19, 0x63, 0x00, 0x0,
	0xd9, 0xe3, 0x00, 0x0,
	0x55, 0xf5, 0x10, 0x0,
	0x27, 0x00, 0x02, 0x0,
	0x29, 0x09, 0x02, 0x0
};

BOOST_STATIC_ASSERT(sizeof(utf32le)%4 == 0);

const std::string utf7text = "english: abcdefgh, +BEAEQwRBBEEEOgQ4BDk-: +BDAEMQQyBDMENAQ1BFEENgQ3BDgEGQQaBBsEH"
	"AQdBB4EHwQgBCEEIgQjBCQEJQQmBCcEKAQpBCoEKwRMBC0ELgQvYxnj2dv93VXYQNwn2ELdKQ-";

const wchar_t utf16text2[] = {
	0x0065, 0x006e, 0x0067, 0x0069, 0x0073, 0x0068, 0x002b, 0x0440,
	0x0443, 0x0441,	0x0441, 0x043a, 0x0438, 0x0439
};

//OEM Russian (DOS)
const unsigned char cp866text[] = {
	0x65, 0x6e, 0x67, 0x69, 0x73, 0x68, 0x2b, 0xe0, 0xe3, 0xe1, 0xe1, 0xaa, 0xa8, 0xa9
};

//ansi-Cyrillic
const unsigned char cp1251text[] = {
	0x65, 0x6e, 0x67, 0x69, 0x73, 0x68, 0x2b, 0xf0, 0xf3, 0xf1, 0xf1, 0xea, 0xe8, 0xe9
};

//KOI-8R
const unsigned char cp20866text[] = {
	0x65, 0x6e, 0x67, 0x69, 0x73, 0x68, 0x2b, 0xd2, 0xd5, 0xd3, 0xd3, 0xcb, 0xc9, 0xca
};

//Cyrillic(ISO)
const unsigned char cp28595text[] = {
	0x65, 0x6e, 0x67, 0x69, 0x73, 0x68, 0x2b, 0xe0, 0xe3, 0xe1, 0xe1, 0xda, 0xd8, 0xd9
};

#include <iomanip>

template <typename Range0T, typename Range1T>
void print_ranges(Range0T r0, Range1T r1) {
    typedef typename boost::range_iterator<Range0T>::type it0_t;
    typedef typename boost::range_iterator<Range1T>::type it1_t;
    it0_t r0it = boost::begin(r0), r0end = boost::end(r0);
    it1_t r1it = boost::begin(r1), r1end = boost::end(r1);

    for (; r0it != r0end || r1it != r1end; ) {
        std::cout << "[";
        if (r0it != r0end) { std::cout << (unsigned long)*r0it; ++r0it; }
        else { std::cout << "end"; }
        std::cout << ",";
        if (r1it != r1end) { std::cout << (unsigned long)*r1it; ++r1it; }
        else { std::cout << "end"; }
        std::cout << "]";
        if (r0it != r0end || r1it != r1end) std::cout << ",";
    }
}

template <typename converterT, typename irangeT, typename vrangeT>
void conv_pull_test(converterT const& enc, irangeT irange, vrangeT vrange, std::string const& descr = "")
{
	try {
		typedef typename boost::range_value<vrangeT>::type outtype_t;
		typedef std::basic_string<outtype_t> result_t;

		result_t result;
        cvt::pull(irange, std::back_inserter(result), enc);

        bool r = result.size() == (size_t)boost::size(vrange);
        BOOST_CHECK_MESSAGE(r, (descr + "[pull-test][range size inconsistency]").c_str());
		if (r) {
            BOOST_CHECK_MESSAGE(
                std::equal(result.begin(), result.end(), boost::begin(vrange)),
                (descr + "[pull-test][range inconsistency]").c_str());
		} else {
            std::cout << "\nExpected/Result: ";
            print_ranges(vrange, result);
            std::cout << "\n\n";
		}
	} catch(std::exception & exp) {
        BOOST_ERROR(descr + "[pull-test] " + exp.what());
	}
}

template <typename converterT, typename irangeT, typename vrangeT>
void conv_push_test(converterT const& enc, irangeT irange, vrangeT vrange, std::string const& descr = "")
{
	try {
		typedef typename boost::range_value<vrangeT>::type outtype_t;
		typedef std::basic_string<outtype_t> result_t;

        result_t result;
        cvt::push(irange, std::back_inserter(result), enc);

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

template <typename EncExprT>
void utf8_to_utf16_test(EncExprT const& expr)
{
	conv_pull_test(expr, boost::make_iterator_range(utf8text), boost::make_iterator_range(utf16text), "[utf8_to_utf16]");
	conv_push_test(expr, boost::make_iterator_range(utf8text), boost::make_iterator_range(utf16text), "[utf8_to_utf16]");
}

template <typename EncExprT>
void utf16_to_utf8_test(EncExprT const& expr)
{
	// take into account BOM
	conv_pull_test(expr, boost::make_iterator_range(utf16text), boost::make_iterator_range(utf8text, utf8text + sizeof(utf8text)), "[utf16_to_utf8]");
	conv_push_test(expr, boost::make_iterator_range(utf16text), boost::make_iterator_range(utf8text, utf8text + sizeof(utf8text)), "[utf16_to_utf8]");
}

template <typename EncExprT>
void utf16_to_utf16le_test(EncExprT const& expr)
{
	conv_pull_test(expr, boost::make_iterator_range(utf16text), boost::make_iterator_range(utf16le), "[utf16_to_utf16le]");
	conv_push_test(expr, boost::make_iterator_range(utf16text), boost::make_iterator_range(utf16le), "[utf16_to_utf16le]");
}

template <typename EncExprT>
void utf16le_to_utf16_test(EncExprT const& expr)
{
	conv_pull_test(expr, boost::make_iterator_range(utf16le), boost::make_iterator_range(utf16text), "[utf16le_to_utf16]");
	conv_push_test(expr, boost::make_iterator_range(utf16le), boost::make_iterator_range(utf16text), "[utf16le_to_utf16]");
}

template <typename EncExprT>
void utf16_to_utf16be_test(EncExprT const& expr)
{
	// preparing utf16be
	std::string utf16be;
	for (size_t i = 0; i < sizeof(utf16le); i += 2)
	{
		utf16be.push_back(utf16le[i + 1]);
		utf16be.push_back(utf16le[i]);
	}

	conv_pull_test(expr, boost::make_iterator_range(utf16text), utf16be, "[utf16_to_utf16be]");
	conv_push_test(expr, boost::make_iterator_range(utf16text), utf16be, "[utf16_to_utf16be]");
}

template <typename EncExprT>
void utf16be_to_utf16_test(EncExprT const& expr)
{
	// preparing utf16be
	std::string utf16be;
	for (size_t i = 0; i < sizeof(utf16le); i += 2)
	{
		utf16be.push_back(utf16le[i + 1]);
		utf16be.push_back(utf16le[i]);
	}

	conv_pull_test(expr, utf16be, boost::make_iterator_range(utf16text), "[utf16be_to_utf16]");
	conv_push_test(expr, utf16be, boost::make_iterator_range(utf16text), "[utf16be_to_utf16]");
}

template <typename EncExprT>
void utf16_to_utf32le_test(EncExprT const& expr)
{
	conv_pull_test(expr, boost::make_iterator_range(utf16text), boost::make_iterator_range(utf32le), "[utf16_to_utf32le]");
	conv_push_test(expr, boost::make_iterator_range(utf16text), boost::make_iterator_range(utf32le), "[utf16_to_utf32le]");
}

template <typename EncExprT>
void utf32le_to_utf16_test(EncExprT const& expr)
{
	conv_pull_test(expr, boost::make_iterator_range(utf32le), boost::make_iterator_range(utf16text), "[utf32le_to_utf16]");
	conv_push_test(expr, boost::make_iterator_range(utf32le), boost::make_iterator_range(utf16text), "[utf32le_to_utf16]");
}

template <typename EncExprT>
void utf16_to_utf32be_test(EncExprT const& expr)
{
	// preparing utf32be
	std::string utf32be;
	for (size_t i = 0; i < sizeof(utf32le); i += 4)
	{
		utf32be.push_back(utf32le[i + 3]);
		utf32be.push_back(utf32le[i + 2]);
		utf32be.push_back(utf32le[i + 1]);
		utf32be.push_back(utf32le[i]);
	}

	conv_pull_test(expr, boost::make_iterator_range(utf16text), boost::make_iterator_range(utf32be), "[utf16_to_utf32be]");
	conv_push_test(expr, boost::make_iterator_range(utf16text), boost::make_iterator_range(utf32be), "[utf16_to_utf32be]");
}

template <typename EncExprT>
void utf32be_to_utf16_test(EncExprT const& expr)
{
	// preparing utf32be
	std::string utf32be;
	for (size_t i = 0; i < sizeof(utf32le); i += 4)
	{
		utf32be.push_back(utf32le[i + 3]);
		utf32be.push_back(utf32le[i + 2]);
		utf32be.push_back(utf32le[i + 1]);
		utf32be.push_back(utf32le[i]);
	}

	conv_pull_test(expr, boost::make_iterator_range(utf32be), boost::make_iterator_range(utf16text), "[utf32be_to_utf16]");
	conv_push_test(expr, boost::make_iterator_range(utf32be), boost::make_iterator_range(utf16text), "[utf32be_to_utf16]");
}

template <typename EncExprT, typename vrageT>
void utf16_to_cp_test(EncExprT const& expr, vrageT const& vrange, const char* testname = "[utf16_to_cp]")
{
	conv_pull_test(expr, boost::make_iterator_range(utf16text2), vrange, testname);
	conv_push_test(expr, boost::make_iterator_range(utf16text2), vrange, testname);
}

template <typename EncExprT, typename irageT>
void cp_to_utf16_test(EncExprT const& expr, irageT const& irange, const char* testname = "[cp_to_utf16]")
{
	conv_pull_test(expr, irange, boost::make_iterator_range(utf16text2), testname);
	conv_push_test(expr, irange, boost::make_iterator_range(utf16text2), testname);
}

template <typename customConverterT>
void utf7_to_utf16_test(customConverterT const& enc)
{
	conv_pull_test(enc, boost::make_iterator_range(utf7text), boost::make_iterator_range(utf16text), "[utf7_to_utf16_test]");
	conv_push_test(enc, boost::make_iterator_range(utf7text), boost::make_iterator_range(utf16text), "[utf7_to_utf16_test]");
}

template <typename customConverterT>
void utf16_to_utf7_test(customConverterT const& enc)
{
	conv_pull_test(enc, boost::make_iterator_range(utf16text), boost::make_iterator_range(utf7text), "[utf16_to_utf7_test]");
	conv_push_test(enc, boost::make_iterator_range(utf16text), boost::make_iterator_range(utf7text), "[utf16_to_utf7_test]");
}

void utf_test()
{
    utf8_to_utf16_test( cvt::utf8 >> cvt::utf32() >> cvt::utf16() );
    utf16_to_utf8_test( cvt::utf16() >> cvt::utf32() >> cvt::utf8 );
    utf8_to_utf16_test( cvt::utf8 >> cvt::utf16() );
    utf16_to_utf8_test( cvt::utf16() >> cvt::utf8 );

    utf16_to_utf16le_test(cvt::utf16() >> cvt::utf16le());
    utf16le_to_utf16_test(cvt::utf16le() >> cvt::utf16());
    utf16_to_utf16be_test(cvt::utf16() >> cvt::utf16be());
    utf16be_to_utf16_test(cvt::utf16be() >> cvt::utf16());

    utf16_to_utf32le_test(cvt::utf16() >> cvt::utf32() >> cvt::utf32le());
    utf32le_to_utf16_test(cvt::utf32le() >> cvt::utf32() >> cvt::utf16());
    utf16_to_utf32be_test(cvt::utf16() >> cvt::utf32() >> cvt::utf32be());
    utf32be_to_utf16_test(cvt::utf32be() >> cvt::utf32() >> cvt::utf16());

    utf16_to_utf7_test( cvt::utf16() >> cvt::utf7() );
    utf7_to_utf16_test( cvt::utf7() >> cvt::utf16() );

#if defined(_MSC_VER)
    utf16_to_cp_test(cvt::utf16() >> cvt::cp(".866"), boost::make_iterator_range(cp866text), "[utf16_to_cp866]");
	utf16_to_cp_test(cvt::utf16() >> cvt::cp(".1251"), boost::make_iterator_range(cp1251text), "[utf16_to_cp1251]");
	utf16_to_cp_test(cvt::utf16() >> cvt::cp(".20866"), boost::make_iterator_range(cp20866text), "[utf16_to_cp20866]");
	utf16_to_cp_test(cvt::utf16() >> cvt::cp(".28595"), boost::make_iterator_range(cp28595text), "[utf16_to_cp28595]");

	cp_to_utf16_test(cvt::cp(".866") >> cvt::utf16(), boost::make_iterator_range(cp866text), "[cp866_to_utf16]");
	cp_to_utf16_test(cvt::cp(".1251") >> cvt::utf16(), boost::make_iterator_range(cp1251text), "[cp1251_to_utf16]");
	cp_to_utf16_test(cvt::cp(".20866") >> cvt::utf16(), boost::make_iterator_range(cp20866text), "[cp20866_to_utf16]");
	cp_to_utf16_test(cvt::cp(".28595") >> cvt::utf16(), boost::make_iterator_range(cp28595text), "[cp28595_to_utf16]");
#else
	/*
	utf16_to_cp_test(cvt::utf16() >> cvt::cp("ru_RU.koi8r"), boost::make_iterator_range(cp20866text), "[utf16_to_cp20866]");
	utf16_to_cp_test(cvt::utf16() >> cvt::cp("ru_RU.iso88595"), boost::make_iterator_range(cp28595text), "[utf16_to_cp28595]");

	cp_to_utf16_test(cvt::cp("ru_RU.koi8r") >> cvt::utf16(), boost::make_iterator_range(cp20866text), "[cp20866_to_utf16]");
	cp_to_utf16_test(cvt::cp("ru_RU.iso88595") >> cvt::utf16(), boost::make_iterator_range(cp28595text), "[cp28595_to_utf16]");
	*/
#endif
}

void utf_test_registrar()
{
	register_test(BOOST_TEST_CASE(&utf_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(utf_test_registrar)
#endif
