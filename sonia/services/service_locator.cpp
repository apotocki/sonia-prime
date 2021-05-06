//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/services.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "service_locator.hpp"

namespace sonia {

service_locator::service_locator(shared_ptr<service_registry> sr, shared_ptr<service_factory> sf)
    : singleton_locator{null}, sr_{std::move(sr)}, sf_{std::move(sf)}
{
    set_log_attribute("Host", services::get_host()->get_name());
    set_log_attribute("Type", "service_locator");
}

service_locator::~service_locator() noexcept
{
    shutdown();
}

shared_ptr<service> service_locator::get(string_view name)
{
    return get(sr_->get_id(name), name);
}

shared_ptr<service> service_locator::get(service::id id)
{
    return get(id, string_view());
}

shared_ptr<service> service_locator::get(service::id id, string_view name)
{
    try {
        return static_pointer_cast<service>(get(id, [this, name](singleton::id id) mutable {
            if (!name) name = sr_->get_name(id);
                
            auto creature = sf_->create(name);
            creature->set_log_attribute("Name", name);
            creature->set_log_attribute("Host", services::get_host()->get_name());
            service_access::set_name(*creature, to_string(name));
            singleton_access::set_id(*creature, id);
            creature->open();
            LOG_TRACE(creature->logger()) << "started";
            return creature;
        }));
    } catch (circular_dependency_error const&) {
        BOOST_THROW_EXCEPTION(circular_dependency_error("found a circular dependency for the service: '%1%' (%2%)"_fmt
            % (name ? name : sr_->get_name(id)) % id));
    }
}

}
