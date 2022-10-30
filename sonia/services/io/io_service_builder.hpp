//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/services/builder.hpp"
#include "io_service.hpp"
#include "io_ssl_service.hpp"

namespace sonia::services {

DECLARE_PARTICULAR_BUILDER(io_service)
DECLARE_PARTICULAR_BUILDER(io_ssl_service)

}
