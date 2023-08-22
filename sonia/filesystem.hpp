//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <filesystem>

/*
#ifdef BOOST_WINDOWS
#   include "sonia/sys/windows/path.hpp"
#endif
*/

#include "string.hpp"

namespace sonia::fs {

using namespace std::filesystem;

}

namespace sonia {

inline fs::path operator / (fs::path const& p, experimental::basic_string_view<char8_t> l)
{
    return p / std::u8string_view(l.data(), l.size());
}

}
