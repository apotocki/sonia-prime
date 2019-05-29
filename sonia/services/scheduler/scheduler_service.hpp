//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_SCHEDULER_SERVICE_HPP
#define SONIA_SERVICES_SCHEDULER_SERVICE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/services/service.hpp"
#include "sonia/services/scheduler/basic_scheduler.hpp"
#include "scheduler_service_configuration.hpp"
#include "sonia/services.hpp"

namespace sonia::services {

class scheduler_service 
    : public service
    , public basic_scheduler
{
public:
    explicit scheduler_service(scheduler_service_configuration const& cfg);

    void open() override;
    void close() noexcept override;

    void on_new_thread() override;

private:
    shared_ptr<host> host_;
};

}

#endif // SONIA_SERVICES_SCHEDULER_SERVICE_HPP
