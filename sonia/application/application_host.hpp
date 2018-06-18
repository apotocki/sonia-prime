//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_APPLICATION_HOST_HPP
#define SONIA_APPLICATION_HOST_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/application/configuration.hpp"

namespace sonia {

class application_host {
public:
    application_host(config_node const& cn);
};

}

#endif // SONIA_APPLICATION_HOST_HPP
