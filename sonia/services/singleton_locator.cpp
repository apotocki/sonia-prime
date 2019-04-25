//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "singleton_locator.hpp"
#include "sonia/exceptions/internal_errors.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia {

//struct service_layer_comparer
//{
//    inline bool operator()(int val, singleton_locator::singleton_descriptor const& csd) const noexcept { return val < csd.object->get_layer(); }
//    inline bool operator()(singleton_locator::singleton_descriptor const& csd, int val) const noexcept { return csd.object.get_layer() < val; }
//};


shared_ptr<singleton> singleton_locator::get(singleton::id id, function<shared_ptr<singleton>()> const& factory)
{
    unique_lock cache_lock(cache_mtx_);
    
    auto [descr_it, inserted] = cache_.try_emplace(id, in_place);
    auto & descr = descr_it->second;
    cache_lock.unlock();

    lock_guard object_guard(descr.mtx);
    if (!descr.object) {
        if (BOOST_UNLIKELY(descr.fid == this_fiber::get_id())) {
            THROW_ERROR(circular_dependency_error());
        }
        descr.fid = this_fiber::get_id();
        SCOPE_EXIT([&descr] { descr.fid = fiber::id(); });
        descr.object = factory();
        singleton_access::set_id(*descr.object, id);
        lock_guard layers_guard(layers_mtx_);
        layers_.insert(descr);
    }
    return descr.object;
}

void singleton_locator::shutdown()
{
    int layer = (std::numeric_limits<int>::max)();
    std::vector<fiber> fibers;

    for (;;)
    {
        if (lock_guard layers_guard(layers_mtx_); !layers_.empty()) {
            for (auto it = layers_.end(), bit = layers_.begin(); it != bit;) {
                --it;
                if (it->object->get_layer() < layer) {
                    layer = it->object->get_layer();
                    break;
                }
                fibers.emplace_back([this, s = it->object]() mutable { shutdown(std::move(s)); });
            }
        } else {
            break;
        }

        //GLOBAL_LOG_TRACE() << "singleton_locator: scheduled " << fibers.size() << " shutdowns, layer: " << layer << " join...";

        for (fiber & f : fibers) {
            f.join();
        }
        fibers.clear();
    }
}

void singleton_locator::shutdown(shared_ptr<singleton> s)
{
    //GLOBAL_LOG_TRACE() << "singleton_locator::shutdown: " << serv->get_name();
    unique_lock cache_lock(cache_mtx_);
    
    auto it = cache_.find(s->get_id());
    BOOST_ASSERT(it != cache_.end());
    auto & descr = it->second;
    cache_lock.unlock();

    lock_guard object_guard(descr.mtx);

    if (descr.object.get() != s.get()) {
        return; // already shutdowned or on its way to it by another thread
    }

    {
        lock_guard layers_guard(layers_mtx_);
        layers_.erase(descr);
    }
    
    descr.object.reset();

    s->close();
}

}
