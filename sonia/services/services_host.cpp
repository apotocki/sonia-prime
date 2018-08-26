//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "host.hpp"

#include <mutex>

namespace sonia { namespace services {

//std::once_flag application_host_init_flag;
//po::options_description application_host_options;
// "id": "127.0.0.0:11111",
// "services": [ "async.serv", "net.serv" ]

host::host(shared_ptr<service_registry> r, shared_ptr<service_factory> f)
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

shared_ptr<service> host::locate(string_view nm) const {
    return locator_->get(nm);
}

shared_ptr<service> host::locate(service::id id) const {
    return locator_->get(id);
}

}}
