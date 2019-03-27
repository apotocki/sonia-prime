//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HOST_IMPL_HPP
#define SONIA_SERVICES_HOST_IMPL_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <vector>

#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/function.hpp"
#include "sonia/concurrency.hpp"
#include "sonia/services/service.hpp"
#include "sonia/services/service_locator.hpp"

#include "host.hpp"
#include "multimethod_registry.hpp"

namespace sonia { namespace services {

class host_impl 
    : public host
    , public multimethod_registry
    , public enable_shared_from_this<host_impl>
{
public:
    explicit host_impl(std::string name);
    ~host_impl() override;

    void open(shared_ptr<service_registry> r, shared_ptr<service_factory> f);
    void close() override;

    shared_ptr<service> locate(string_view) const;
    shared_ptr<service> locate(service::id) const;

    void register_on_close(function<void()> const& func);

    void run(std::vector<std::string> const& servs);

    string_view get_name() const override final { return name_; }
    void attach_to_current_thread() override final;
    void dettach_from_current_thread() override final;

private:
    std::string name_;
    shared_ptr<service_registry> registry_;
    shared_ptr<service_locator> locator_;

    spin_mutex on_close_mtx_;
    std::vector<function<void()>> on_close_handlers_;
};

}}

#endif // SONIA_SERVICES_HOST_HPP
