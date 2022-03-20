/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include "sonia/config.hpp"

#include <iostream>

#include "sonia/utility/conversion/utf8/utf8.hpp"
#include "sonia/utility/conversion/utf16/utf16.hpp"
#include "sonia/utility/conversion/base64/base64.hpp"
#include "sonia/utility/conversion/rc5/rc5.hpp"

#include "sonia/utility/conversion/utility.hpp"

#include <boost/range/iterator_range.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/size.hpp>

#include "applied/sonia_test.hpp"

namespace cvt = sonia::conversion;

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
            r = std::equal(result.begin(), result.end(), boost::begin(vrange));
            BOOST_CHECK_MESSAGE(r, (descr + "[pull-test][range inconsistency]").c_str());
			if (!r) {
			    std::cout << "\nExpected/Result: ";
                print_ranges(vrange, result);
                std::cout << "\n\n";
			}
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

void utf16_utf32(std::wstring const& utf16string)
{
	//typedef cvt::encoder<wchar_t, char32_t> encoder_t;

	auto coder = cvt::utf16() >> cvt::utf32();
    typedef decltype(coder) encoder_t;
    typedef cvt::convert_input_iterator<encoder_t, std::wstring::const_iterator> it_t;
    typedef it_t::state_type state_type;
	std::basic_string<char32_t> result;
    state_type state;
	std::copy(
		it_t(coder, state, utf16string.begin(), utf16string.end()),
		it_t(),
		std::back_inserter(result)
	);

	typedef cvt::convert_output_iterator<encoder_t, std::back_insert_iterator<std::basic_string<char32_t> > > oit_t;
    oit_t::state_type state2;
	std::basic_string<char32_t> result2;
	std::copy(utf16string.begin(), utf16string.end(), oit_t(coder, state2, std::back_inserter(result))).unshift();
}

#include "sonia/utility/conversion/utility/buffer.hpp"
//#include "sonia/utility/conversion/deflate/deflate.hpp"

/*
BOOST_AUTO_TEST_CASE( compound_cvt_test_temp )
{
    std::string ctext = "Man is distinguished, not only by his reason, but by this singular "
		"passion from other animals, which is a lust of the mind, that by a perseverance of delight "
		"in the continued and indefatigable generation of knowledge, exceeds the short vehemence "
		"of any carnal pleasure.";

    std::vector<char> result, result2;
    cvt::push(ctext, std::back_inserter(result), cvt::int8() >> cvt::int8_buffer<1024>() >> cvt::deflate() >> cvt::int8());
    cvt::pull(ctext, std::back_inserter(result2), cvt::int8() >> cvt::int8_buffer<1024>() >> cvt::deflate() >> cvt::int8());
    BOOST_CHECK (compare(result, result2));
}
*/

void compound_cvt_test()
{
    const std::wstring text = L"Man is distinguished, not only by his reason, but by this singular "
		L"passion from other animals, which is a lust of the mind, that by a perseverance of delight "
		L"in the continued and indefatigable generation of knowledge, exceeds the short vehemence "
		L"of any carnal pleasure.";

	std::string ctext = "Man is distinguished, not only by his reason, but by this singular "
		"passion from other animals, which is a lust of the mind, that by a perseverance of delight "
		"in the continued and indefatigable generation of knowledge, exceeds the short vehemence "
		"of any carnal pleasure.";
    ctext.push_back(0);ctext.push_back(0);ctext.push_back(0); //<- \0 is a result of rc5 padding

    const std::string encoded_text =
		"1P4G6C36RMNgqqbFpmzzzO9xbS0b20DNVcJvAiDNu2Gkd5zHOAAaK/9REfT4heKF7LV3B3l/x+e51zoD"
		"svN5vsITGI8WrOtT7E5890UzbGxoALHgHgkYrtkI1ejR86eqmZFOyUmFwD4m7+NgjobIMXh1iWOTltsq"
		"IN/4FrtzTWsgeP+IjqBSaESwbuBTcAMV1+MvDIyfc+vgLavUIAPfCpxO0BTpOOdG9OwZZjgxLvRySf3u"
		"W4e5kBNg6ANlWHB/5XrWFQakqnRYysRnfLRPQf0B5Dbr5P3DXe1jkEzf8qJixdQ7Q1fTlzusBYAUEnAy"
		"kpMGsLE1KTLHT4ao/8HLf/oN0ujdd4sXHnb9I417lGY=";

	utf16_utf32(text);

	unsigned char key[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	//typedef cvt::encoder<wchar_t, char> encoder_t;

	auto coder = cvt::utf16() >> cvt::utf32() >> cvt::utf8 / cvt::int8() >> cvt::rc5<32,12,8>(key) / cvt::int8() >> cvt::base64();
    conv_pull_test(coder, boost::make_iterator_range(text), boost::make_iterator_range(encoded_text), "[polymorphic: utf16->utf8->rc5->base64]");
	conv_push_test(coder, boost::make_iterator_range(text), boost::make_iterator_range(encoded_text), "[polymorphic: utf16->utf8->rc5->base64]");

	conv_pull_test(
		cvt::utf16() >> cvt::utf32() >> cvt::utf8 / cvt::int8() >> cvt::rc5<32,12,8>(key) / cvt::int8() >> cvt::base64(),
		boost::make_iterator_range(text),
		boost::make_iterator_range(encoded_text),
		"[utf16->utf8->rc5->base64]");

	//typedef cvt::encoder<char, char> encoder2_t;
	auto coder2 = cvt::base64() >> cvt::int8() / cvt::rc5<32,12,8>(key) >> cvt::int8() / cvt::utf8;
	conv_pull_test(coder2, boost::make_iterator_range(encoded_text), boost::make_iterator_range(ctext), "[base64->rc5->utf8]");

	{
		std::string input("abcd"); // some character range with known encoding
		//typedef cvt::encoder<char, char32_t> encoder_t; // polymorphic encoder type declaration
		auto enc = cvt::utf8 >> cvt::utf32(); //concrete encoder instantiation, supported utf8, utf16, utf16le, utf16be, and locale string base encodings
        typedef decltype(enc) encoder_t;
		typedef cvt::convert_input_iterator<encoder_t, std::string::const_iterator> it_t;

		std::basic_string<char32_t> result;
        it_t::state_type state;
		std::copy(
			  it_t(enc, state, input.begin(), input.end())
			, it_t()
			, std::back_inserter(result)
		);
	}
}

void compound_cvt_test_registrar()
{
	register_test(BOOST_TEST_CASE(&compound_cvt_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(compound_cvt_test_registrar)
#endif