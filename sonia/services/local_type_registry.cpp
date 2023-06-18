//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "local_type_registry.hpp"

namespace sonia {

local_type_registry::local_type_registry(shared_ptr<persister> sp)
    : state_persister_{std::move(sp)}
{
    base_t::restore();
}

uint32_t local_type_registry::get_type_id(std::string_view name, std::string_view meta)
{
    return base_t::get_id(name, meta);
}

std::pair<std::string_view, std::string_view> local_type_registry::get_type_description(uint32_t id) const
{
    return base_t::get_data(id);
}

}
