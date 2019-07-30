//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "registry_service.hpp"

namespace sonia::services {

registry_service::registry_service(registry_service_configuration const& cfg)
    : cfg_(cfg)
{
    set_log_attribute("Type", "file based registry");
}

void registry_service::open()
{
    statable_t::open();
    restore();
}

}
