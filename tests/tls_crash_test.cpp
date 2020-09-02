//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include <boost/test/unit_test.hpp>

#include <vector>
#include <thread>

// based on https://sourceforge.net/p/mingw-w64/bugs/527/

BOOST_AUTO_TEST_CASE(tls_crash_test0)
{
    static const thread_local std::vector<int> two(2, 3);
    BOOST_CHECK_EQUAL(2, two.size());
}

thread_local std::string s("Hello");
BOOST_AUTO_TEST_CASE(tls_crash_test1)
{
    std::thread([]
    {
        (void)s;
    }).join();
    BOOST_CHECK(true);
}
