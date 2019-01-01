//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "local_service_registry.hpp"

namespace sonia {

local_service_registry::local_service_registry(shared_ptr<persister> sp)
    : base_t(std::move(sp))
{
    base_t::restore();
}

service::id local_service_registry::get_id(string_view name) {
    return base_t::get_id(name, "");
}

string_view local_service_registry::get_name(service::id id) const {
    return base_t::get_name(id);
}

}
