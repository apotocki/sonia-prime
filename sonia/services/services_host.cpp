//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "host.hpp"

#include <mutex>

#ifndef SONIA_MM_ITEM_POOL_SIZE
#   define SONIA_MM_ITEM_POOL_SIZE 128
#endif

namespace sonia { namespace services {

//std::once_flag application_host_init_flag;
//po::options_description application_host_options;
// "id": "127.0.0.0:11111",
// "services": [ "async.serv", "net.serv" ]

host::host(shared_ptr<service_registry> r, shared_ptr<service_factory> f)
//    : mm_item_pool_(SONIA_MM_ITEM_POOL_SIZE)
{
    locator_ = make_shared<service_locator>(std::move(r), std::move(f));
    /*
    std::call_once(application_host_init_flag, []() {
        application_host_options.add_options()
            ("id", po::value<std::string>()->required(), "host identifier")
            ("services", po::value<std::vector<std::string> >()->composing(), "comma separated list of services")
        ;
    });
    */
}

host::~host() {
    for (function<void()> & h : on_close_handlers_) {
        h();
    }
}

shared_ptr<service> host::locate(string_view nm) const {
    return locator_->get(nm);
}

shared_ptr<service> host::locate(service::id id) const {
    return locator_->get(id);
}

void host::run(std::vector<std::string> const& servs) {
    for (auto const& s : servs) {
        locate(s);
    }
}

void host::register_on_close(function<void()> const& func) {
    auto guard = make_lock_guard(on_close_mtx_);
    on_close_handlers_.push_back(func);
}

void host::register_multimethod(multimethod && mm, type_info const& id, array_view<const type_info> mmid) {
    auto slguard = make_lock_guard(mm_item_mtx_);
    auto it = mm_set_.find(id, hash<type_info>(), std::equal_to<type_info>());
    if (it == mm_set_.end()) {
        mm_set_.insert(it, {mmholder_t(std::move(mm)), id, std::vector<type_info>(mmid.begin(), mmid.end())});
    } else {
        throw internal_error("multimethod with signature %1% is already registered"_fmt % id.name());
    }
}

multimethod const* host::get_multimethod(type_info const& id) const {
    
    auto slguard = make_shared_lock_guard(mm_item_mtx_);
    auto it = mm_set_.find(id, hash<type_info>(), std::equal_to<type_info>());
    return (it != mm_set_.cend()) ? it->mm.get_pointer() : nullptr;
}

}}
