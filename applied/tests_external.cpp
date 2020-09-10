//
//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
//
#include "sonia/config.hpp"

//#ifndef BOOST_TEST_MODULE
//#   define BOOST_TEST_MODULE sonia_regression_tests
//#endif

//#define BOOST_TEST_NO_MAIN
//#define BOOST_TEST_MAIN
//#include <boost/test/unit_test.hpp>

#include <iostream>

#if 1
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>

#if !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_TEST_NO_LIB) && \
    !defined(BOOST_TEST_SOURCE) && !defined(BOOST_TEST_INCLUDED)
#  define BOOST_LIB_NAME boost_unit_test_framework

#  if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_TEST_DYN_LINK)
#    define BOOST_DYN_LINK
#  endif

#  include <boost/config/auto_link.hpp>

#endif  // auto-linking disabled

#include <iostream>

namespace boost::unit_test {

int BOOST_TEST_DECL unit_test_main(init_unit_test_func init_func, int argc, char* argv[]);

}
#endif

#ifndef AUTO_TEST_REGISTRATION
void all_tests_registrar();
#endif

std::vector<boost::unit_test::test_case*> * tests_ = nullptr;
bool master_test_suite_initialized_ = false;
void register_test(boost::unit_test::test_case* ptc)
{
    if (!tests_) {
        tests_ = new std::vector<boost::unit_test::test_case*>{ ptc };
    } else {
        tests_->push_back(ptc);
    }
}

bool init_unit_test()
{
    if (!master_test_suite_initialized_) {
#ifndef AUTO_TEST_REGISTRATION
        all_tests_registrar();
#endif
        using namespace boost::unit_test;
        if (tests_) {
            for (test_case* ptf : *tests_) {
                framework::master_test_suite().add(ptf);
            }
            delete tests_;
            tests_ = nullptr;
        }
        master_test_suite_initialized_ = true;
        return true;
    }
}

boost::unit_test::test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[])
{
    init_unit_test();
    return nullptr;
}

extern "C" {

int run_tests(int argc, char* argv[])
{
#ifdef BOOST_TEST_ALTERNATIVE_INIT_API
    return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
#else
    return ::boost::unit_test::unit_test_main(&init_unit_test_suite, argc, argv);
#endif
}

}

