//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "service_factory.hpp"

#include "sonia/exceptions.hpp"

namespace sonia { namespace services {

service_descriptor basic_service_factory::create(string_view nm) const 
{
    unique_lock lock(named_factories_mtx_);
    auto it = named_factories_.find(nm, hasher(), string_equal_to());
    if (it != named_factories_.cend()) {
        function<service_descriptor()> func = it->second;
        lock.unlock();
        try {
            return func();
        } catch (std::exception const& e) {
            throw internal_error("Error occurred during building '%1%' service\n%2%"_fmt % nm % e.what());
        }
    }

    throw internal_error("can't create unknown service '%1%'"_fmt % nm);
}

void basic_service_factory::register_service_factory(string_view nm, function<service_descriptor()> const& fm)
{
    lock_guard guard(named_factories_mtx_);
    auto it = named_factories_.find(nm, hasher(), string_equal_to());
    if (it == named_factories_.end()) {
        named_factories_.insert(it, factories_type::value_type(to_string(nm), fm));
    } else {
        throw internal_error("factory for the service '%1%' is already registered"_fmt % nm);
    }
}

}}
