//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_SCHEDULER_SERVICE_CONFIGURATION_HPP
#define SONIA_SERVICES_SCHEDULER_SERVICE_CONFIGURATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

namespace sonia::services {

struct scheduler_service_configuration
{
    uint32_t threads;
    uint32_t fibers;
};

}

#endif // SONIA_SERVICES_SCHEDULER_SERVICE_CONFIGURATION_HPP
