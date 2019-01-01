//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/services.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "service_locator.hpp"

namespace sonia {

struct service_layer_comparer {
    inline bool operator()(int val, service_locator::cached_service_descriptor const& csd) const { return val < csd.layer(); }
    inline bool operator()(service_locator::cached_service_descriptor const& csd, int val) const { return csd.layer() < val; }
};

service_locator::service_locator(shared_ptr<service_registry> sr, shared_ptr<service_factory> sf)
    : sr_(std::move(sr)), sf_(std::move(sf))
{
    set_attribute("Host", services::get_host()->get_name());
}

service_locator::~service_locator()
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
    auto cache_lock = make_unique_lock(cache_mtx_);

    auto & tpl = cache_.try_emplace(id, std::in_place).first->second;
    cache_lock.unlock();

    lock_guard<mutex> object_guard(tpl.mtx);
    if (!tpl.object()) {
        this_thread::interruption_point();

        if (tpl.fid == this_fiber::get_id()) {
            BOOST_THROW_EXCEPTION(internal_error("found a circular dependency for the service: '%1%' (%2%)"_fmt
                % (name ? name : sr_->get_name(id)) % id));
        }

        tpl.fid = this_fiber::get_id();
        SCOPE_EXIT([&tpl] { tpl.fid = fiber::id(); });

        if (name) name = sr_->get_name(id);
        auto creature = sf_->create(name);
        service_access::set(*creature.serv, id, to_string(name));
        creature.serv->set_attribute("Name", name);
        creature.serv->set_attribute("Host", services::get_host()->get_name());
        creature.serv->open();
        LOG_TRACE(logger()) << "service " << name << "(id: " << id << ") is started";
        {
            auto layers_guard = make_lock_guard(layers_mtx_);
            layers_.insert(tpl);
        }

        tpl.descr = std::move(creature);
    }

    return tpl.object();
}

void service_locator::shutdown(shared_ptr<service> serv)
{
    auto cache_lock = make_unique_lock(cache_mtx_);
    
    auto it = cache_.find(serv->get_id());
    BOOST_ASSERT(it != cache_.end());
    auto & tpl = it->second;
    cache_lock.unlock();

    auto object_guard = make_lock_guard(tpl.mtx);

    if (tpl.object().get() != serv.get()) {
        return; // already shutdowned or on its way to it by another thread
    }

    {
        auto layers_guard = make_lock_guard(layers_mtx_);
        layers_.erase(tpl);
    }
    
    tpl.object().reset();

    serv->close();
}

void service_locator::shutdown(int down_to_layer)
{
    std::vector<shared_ptr<service>> services_to_shutdown;
    {
        auto layers_guard = make_lock_guard(layers_mtx_);
        for (auto it = layers_.end(), bit = layers_.begin(); it != bit;) {
            --it;
            if (it->layer() < down_to_layer) break;
            services_to_shutdown.push_back(it->object());
        }
    }
    while (!services_to_shutdown.empty()) {
        auto serv = std::move(services_to_shutdown.back());
        services_to_shutdown.pop_back();
        shutdown(std::move(serv));
    }
}

}
