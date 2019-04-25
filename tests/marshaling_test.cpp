//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <boost/test/unit_test.hpp>

#include <vector>

#include "sonia/utility/marshaling/marshaling.hpp"
#include "sonia/utility/marshaling/string.hpp"

#include "sonia/utility/serialization/reference.hpp"
#include "sonia/utility/serialization/placeholder.hpp"
#include "sonia/utility/serialization/integral.hpp"
#include "sonia/utility/serialization/string.hpp"

using namespace sonia;

namespace {

std::string test_function0(int iv, std::string const& v)
{
    return std::to_string(iv) + "_" + v;
}

}

BOOST_AUTO_TEST_CASE(marshaling_test)
{
    SONIA_DECLARE_BINDING_TAG(tag0_type, &test_function0);
    SONIA_DECLARE_BINDING_TAG(tag1_type, &test_function0, 0);
    SONIA_DECLARE_BINDING_TAG(tag2_type, &test_function0, 1, 0);

    std::vector<char> data;
    tag0_type::proxy_encode<serialization::compressed_t>(std::back_inserter(data), 0, std::string("0"));
    std::string res = tag0_type::stub_invoke<serialization::compressed_t>(data.begin());
    BOOST_CHECK_EQUAL(res, "0_0");

    data.clear();
    tag1_type::proxy_encode<serialization::compressed_t>(std::back_inserter(data), std::string("1"));
    std::string res2 = tag1_type::stub_invoke<serialization::compressed_t>(data.begin(), 1);
    BOOST_CHECK_EQUAL(res2, "1_1");

    data.clear();
    tag2_type::proxy_encode<serialization::compressed_t>(std::back_inserter(data));
    std::string res3 = tag2_type::stub_invoke<serialization::compressed_t>(data.begin(), std::string("2"), 2);
    BOOST_CHECK_EQUAL(res3, "2_2");
}
