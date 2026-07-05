//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <iosfwd>
#include <time.h>

#include "sonia/string.hpp"

namespace sonia::http {

std::string http_date(tm* tm);

}
