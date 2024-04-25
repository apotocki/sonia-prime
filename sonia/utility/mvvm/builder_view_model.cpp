//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "builder_view_model.hpp"

namespace sonia {

void builder_view_model::do_registration(registrar_type& mr)
{
    mr.register_method<&builder_view_model::create>("create"sv);
}

shared_ptr<invocation::invocable> builder_view_model::create(string_view type)
{
    return factory_.create(type);
}

}
