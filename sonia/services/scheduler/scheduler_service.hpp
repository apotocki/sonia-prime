//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_SCHEDULER_SERVICE_HPP
#define SONIA_SERVICES_SCHEDULER_SERVICE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/services/service.hpp"
#include "sonia/scheduler/basic_scheduler.hpp"
#include "scheduler_configuration.hpp"

namespace sonia { namespace services {

class scheduler_service 
    : public service
    , public basic_scheduler
{
public:
    scheduler_service(scheduler_configuration const& cfg);

    void open() override;
    void close() noexcept override;
};

}}

#endif // SONIA_SERVICES_SCHEDULER_SERVICE_HPP
