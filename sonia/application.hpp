//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_APPLICATION_HPP
#define SONIA_APPLICATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "prime_config.hpp"

namespace sonia {

struct application_configuration {
    bool verbose;
};

SONIA_PRIME_API application_configuration & get_application_configuration();

}

#endif // SONIA_APPLICATION_HPP
