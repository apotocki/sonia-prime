//
//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
//
#include "sonia/config.hpp"
#include "tests_external.hpp"

#ifdef BOOST_WINDOWS
#   pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    const char* argv_[] = { "tests", "--no_color_output", "--log_level=test_suite" };
    return run_tests(3, (char**)argv_);
    //return run_tests(argc, argv);
}
