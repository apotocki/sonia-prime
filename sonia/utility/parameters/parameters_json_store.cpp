//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "json_store.hpp"

#include "sonia/exceptions.hpp"

namespace sonia { namespace parameters {

void store(parameters_description const& params, shared_ptr<json_value> jv, variables & vars)
{
    /*
    if (!jv->is_object()) {
        throw exception("json value is not an object");
    }
    params.for_each([&jv, &vars](value_descriptor const& vd) {
        shared_ptr<json_value> val = (*jv)[vd.name()];
        if (val) {
            //vars.set_variable(vd.name(), );
        }
    });
    */
}

}}
