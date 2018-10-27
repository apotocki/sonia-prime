//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "scheduler_service.hpp"

namespace sonia { namespace services {

scheduler_service::scheduler_service(scheduler_service_configuration const& cfg)
    : basic_scheduler(cfg.threads, cfg.fibers)
{
    set_attribute("Type", "scheduler");
}

void scheduler_service::open() {
    start();
}

void scheduler_service::close() noexcept {
    stop();
}

}}
