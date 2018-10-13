//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_AIO_CONFIGURATION_HPP
#define SONIA_SERVICES_AIO_CONFIGURATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/cstdint.hpp"

namespace sonia { namespace services {

class aio_configuration {
public:
    uint32_t threads;
};

}}

#endif // SONIA_SERVICES_AIO_CONFIGURATION_HPP
