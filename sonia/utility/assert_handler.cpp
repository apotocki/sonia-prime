//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/exceptions.hpp"
#include <boost/stacktrace.hpp>

namespace boost {

using namespace sonia;

void assertion_failed(char const * expr, char const * function, char const * file, long line) {
    throw internal_error("BOOST_ASSERT: %1%\n%2%\n%3% at line: %4%\nbacktrace:\n%5%"_fmt % expr % function % file % line % boost::stacktrace::stacktrace());
}

void assertion_failed_msg(char const * expr, char const * msg, char const * function, char const * file, long line) {
    throw internal_error("BOOST_ASSERT [Message: %1%]: %2%\n%3%\n%4% at line: %5%\nbacktrace:\n%6%"_fmt % msg % expr % function % file % line % boost::stacktrace::stacktrace());
}

}
