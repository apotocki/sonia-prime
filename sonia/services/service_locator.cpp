//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/services.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "service_locator.hpp"

namespace sonia {

struct service_layer_comparer
{
    inline bool operator()(int val, service_locator::cached_service_descriptor const& csd) const noexcept { return val < csd.descr.layer; }
    inline bool operator()(service_locator::cached_service_descriptor const& csd, int val) const noexcept { return csd.descr.layer < val; }
};

service_locator::service_locator(shared_ptr<service_registry> sr, shared_ptr<service_factory> sf)
    : sr_(std::move(sr)), sf_(std::move(sf))
{
    set_log_attribute("Host", services::get_host()->get_name());
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
    unique_lock cache_lock(cache_mtx_);

    auto & tpl = cache_.try_emplace(id, in_place).first->second;
    cache_lock.unlock();

    lock_guard<mutex> object_guard(tpl.mtx);
    if (!tpl.object()) {
        if (tpl.fid == this_fiber::get_id()) {
            BOOST_THROW_EXCEPTION(internal_error("found a circular dependency for the service: '%1%' (%2%)"_fmt
                % (name ? name : sr_->get_name(id)) % id));
        }

        tpl.fid = this_fiber::get_id();
        SCOPE_EXIT([&tpl] { tpl.fid = fiber::id(); });

        if (name) name = sr_->get_name(id);
        auto creature = sf_->create(name);
        service_access::set(*creature.serv, id, to_string(name));
        creature.serv->set_log_attribute("Name", name);
        creature.serv->set_log_attribute("Host", services::get_host()->get_name());
        creature.serv->open();
        tpl.descr = std::move(creature);
        LOG_TRACE(logger()) << "service " << name << "(id: " << id << ") is started";
        {
            lock_guard layers_guard(layers_mtx_);
            layers_.insert(tpl);
        }
    }

    return tpl.object();
}

void service_locator::shutdown(shared_ptr<service> serv)
{
    //GLOBAL_LOG_TRACE() << "service_locator::shutdown: " << serv->get_name();
    unique_lock cache_lock(cache_mtx_);
    
    auto it = cache_.find(serv->get_id());
    BOOST_ASSERT(it != cache_.end());
    auto & tpl = it->second;
    cache_lock.unlock();

    lock_guard object_guard(tpl.mtx);

    if (tpl.object().get() != serv.get()) {
        return; // already shutdowned or on its way to it by another thread
    }

    {
        lock_guard layers_guard(layers_mtx_);
        layers_.erase(tpl);
    }
    
    tpl.object().reset();

    serv->close();
}

void service_locator::shutdown()
{
    int layer = (std::numeric_limits<int>::max)();
    std::vector<fiber> fibers;

    for (;;)
    {
        if (lock_guard layers_guard(layers_mtx_); !layers_.empty()) {
            for (auto it = layers_.end(), bit = layers_.begin(); it != bit;) {
                --it;
                if (it->descr.layer < layer) {
                    layer = it->descr.layer;
                    break;
                }
                fibers.emplace_back([this, serv = it->object()]() mutable { shutdown(std::move(serv)); });
            }
        } else {
            break;
        }

        //GLOBAL_LOG_TRACE() << "service_locator: scheduled " << fibers.size() << " shutdowns, layer: " << layer << " join...";

        for (fiber & f : fibers) {
            f.join();
        }
        fibers.clear();
    }
}

}
