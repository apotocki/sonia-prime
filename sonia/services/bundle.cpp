//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/build_config.hpp"
#include "sonia/exceptions.hpp"

#include "bundle.hpp"

namespace sonia { namespace services {

bundle::bundle()
    : libhandle_(nullptr)
{
    set_attribute("Type", "bundle");
}

void bundle::open() {
    if (std::string(BUILD_ID) != build_id()) {
        LOG_WARN(logger()) << "different bundle id: " << build_id() << ", expected: " << BUILD_ID;
    }
    init();
}

shared_ptr<service> bundle::build(json_object const& jo) {
    json_value const* jnm = jo["name"];
    if (!jnm || jnm->type() != json_value_type::string) {
        throw internal_error("bundle factory error: No factory name was found in '%1%'"_fmt % to_string(jo));
    }

    string_view name = jnm->get_string();
    auto it = builders_.find(name, hasher(), string_equal_to());
    if (it == builders_.end()) {
        throw internal_error("The '%1%' bundle has no a definition of '%2%' factory"_fmt % typeid(*this).name() % name);
    }
    
    return it->second(jo);
}

}}
