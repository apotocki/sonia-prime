//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include <vector>
#include <thread>

#include "applied/sonia_test.hpp"

// based on https://sourceforge.net/p/mingw-w64/bugs/527/

void tls_crash_test0()
{
    static const thread_local std::vector<int> two(2, 3);
    BOOST_CHECK_EQUAL(2, two.size());
}

thread_local std::string s("Hello");
void tls_crash_test1()
{
    std::thread([]
    {
        (void)s;
    }).join();
    BOOST_CHECK(true);
}

void tls_crash_test_registrar()
{
    register_test(BOOST_TEST_CASE(&tls_crash_test0));
    register_test(BOOST_TEST_CASE(&tls_crash_test1));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(tls_crash_test_registrar)
#endif
