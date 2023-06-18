//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "host_impl.hpp"

#include "sonia/utility/streaming/type_index.hpp"
#include "sonia/utility/streaming/array_view.hpp"
#include "thread_descriptor.hpp"

//#ifndef SONIA_MM_ITEM_POOL_SIZE
//#   define SONIA_MM_ITEM_POOL_SIZE 128
//#endif

namespace sonia::services {

host_impl::host_impl(std::string name)
    : name_(std::move(name))
//    : mm_item_pool_(SONIA_MM_ITEM_POOL_SIZE)
{

}

void host_impl::open(shared_ptr<service_registry> r, shared_ptr<service_factory> f)
{
    attach_to_current_thread();
    locator_ = make_shared<service_locator>(std::move(r), std::move(f));
}

void host_impl::close()
{
    locator_->shutdown();
}

host_impl::~host_impl()
{
    for (function<void()> & h : on_close_handlers_) {
        h();
    }
}

shared_ptr<service> host_impl::locate(std::string_view nm) const
{
    return locator_->get(nm);
}

shared_ptr<service> host_impl::locate(service::id id) const
{
    return locator_->get(id);
}

void host_impl::run(std::vector<std::string> const& servs)
{
    for (auto const& s : servs) {
        locate(s);
    }
}

void host_impl::register_on_close(function<void()> const& func)
{
    lock_guard guard(on_close_mtx_);
    on_close_handlers_.push_back(func);
}



void host_impl::attach_to_current_thread()
{
    (new thread_descriptor(shared_from_this()))->set();
}

void host_impl::dettach_from_current_thread()
{
    thread_descriptor::reset();
}

}
