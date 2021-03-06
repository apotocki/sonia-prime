//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_AUTH_SERVICE_BUILDER_HPP
#define SONIA_AUTH_SERVICE_BUILDER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/services/builder.hpp"
#include "auth_service.hpp"

namespace sonia::services {

DECLARE_PARTICULAR_BUILDER(auth_service)

}

#endif // SONIA_AUTH_SERVICE_BUILDER_HPP
