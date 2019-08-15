//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "bookkeeper_service.hpp"
#include "sonia/exceptions.hpp"

namespace sonia::services {

bookkeeper_service::bookkeeper_service(bookkeeper_service_configuration const& cfg)
    : cfg_(cfg)
{
    set_log_attribute("Type", "file based bookkeeper");
}

void bookkeeper_service::open()
{
    statable_t::open();
    restore();
}

void bookkeeper_service::set(string_view key, json_value const&)
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

json_value const* bookkeeper_service::get(string_view key)
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

void bookkeeper_service::erase(string_view key)
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

bool bookkeeper_service::compare_and_change(string_view key, json_value const* expected, json_value const* newval)
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

}
