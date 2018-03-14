//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "service_cache.hpp"

namespace sonia {

service_cache::service_cache(shared_ptr<service_registry> sr, shared_ptr<service_factory> sf)
    : sr_(std::move(sr)), sf_(std::move(sf))
{

}

shared_ptr<service> service_cache::get(string_view name)
{
    throw 1;
}

shared_ptr<service> service_cache::get(service::id_type id)
{
    throw 1;
}

}
