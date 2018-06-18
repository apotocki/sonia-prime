//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HPP
#define SONIA_SERVICES_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "prime_config.hpp"
#include "sonia/service/service.hpp"

namespace sonia { namespace services {

SONIA_PRIME_API shared_ptr<service> locate(string_view);
SONIA_PRIME_API shared_ptr<service> locate(service::id);

}}

#endif // SONIA_SERVICES_HPP
