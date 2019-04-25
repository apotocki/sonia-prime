//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/build_config.hpp"
#include "sonia/exceptions/internal_errors.hpp"

#include "bundle.hpp"

namespace sonia::services {

bundle::bundle()
    : libhandle_(nullptr)
{
    set_log_attribute("Type", "bundle");
}

void bundle::open()
{
    if (std::string(BUILD_ID) != build_id()) {
        LOG_WARN(logger()) << "different bundle id: " << build_id() << ", expected: " << BUILD_ID;
    }
    init();
}

shared_ptr<service> bundle::build(service_configuration const& cfg)
{
    json_value const* jnm = cfg.parameters["name"];
    if (!jnm || jnm->type() != json_value_type::string) {
        THROW_INTERNAL_ERROR("bundle factory error: No factory name was found in '%1%'"_fmt % to_string(cfg.parameters));
    }

    string_view name = jnm->get_string();
    auto it = builders_.find(name, hasher(), string_equal_to());
    if (it == builders_.end()) {
        THROW_INTERNAL_ERROR("The '%1%' bundle has no a definition of '%2%' factory"_fmt % typeid(*this).name() % name);
    }
    
    shared_ptr<service> r = it->second(cfg.parameters);
    service_access::set_layer(*r, cfg.layer);
    return r;
}

}
