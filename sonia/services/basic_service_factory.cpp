//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "service_factory.hpp"



#include "sonia/exceptions.hpp"

namespace sonia { namespace services {

shared_ptr<service> basic_service_factory::create(string_view nm) const 
{
    unique_lock<mutex> lock(named_factories_mtx_);
    auto it = named_factories_.find(nm, string_hasher(), string_equal_to());
    if (it != named_factories_.cend()) {
        function<shared_ptr<service>()> func = it->second;
        lock.unlock();
        return func();
    }

    throw not_implemented_error(fmt("can't create service '%1%'") % nm);
}

void basic_service_factory::register_service_factory(string_view nm, function<shared_ptr<service>()> const& fm)
{
    lock_guard<mutex> guard(named_factories_mtx_);
    auto it = named_factories_.find(nm, string_hasher(), string_equal_to());
    if (it == named_factories_.end()) {
        named_factories_.insert(it, factories_type::value_type(to_string(nm), fm));
    } else {
        throw internal_error(fmt("factory for the service '%1%' is already registered") % nm);
    }
}

}}
