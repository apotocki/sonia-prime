//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_TRANSCEIVER_CONFIGURATION_HPP
#define SONIA_SERVICES_TRANSCEIVER_CONFIGURATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

namespace sonia { namespace services {

struct transceiver_service_configuration
{
    std::string socket_factory;
};

}}

#endif // SONIA_SERVICES_TRANSCEIVER_CONFIGURATION_HPP
