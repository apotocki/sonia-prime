//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/utility/linux.hpp"
#include "sonia/exceptions.hpp"

#include <sys/utsname.h>

#include <string>
#include <cstdlib>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace sonia { namespace linux {

std::tuple<int, int, int> kernel_version()
{
    utsname nm;
    if (uname(&nm)) {
        int err = errno;
        throw exception("can't retrieve the information about current kernel, error : %1%"_fmt % strerror(err));
    }

    std::string kname(nm.release);

    std::vector<std::string> svec;
    boost::split(svec, kname, boost::is_any_of("."), boost::token_compress_on);

    std::tuple<int, int, int> result{0, 0, 0};
    if (svec.size() > 0) std::get<0>(result) = atoi(svec[0].c_str());
    if (svec.size() > 1) std::get<1>(result) = atoi(svec[1].c_str());
    if (svec.size() > 2) std::get<2>(result) = atoi(svec[2].c_str());
    return result;
}

}}
