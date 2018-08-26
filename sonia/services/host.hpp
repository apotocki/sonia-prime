//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HOST_HPP
#define SONIA_SERVICES_HOST_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/services/service.hpp"
#include "sonia/services/service_locator.hpp"

namespace sonia { namespace services {

class host {
public:
    host(shared_ptr<service_registry>, shared_ptr<service_factory>);

    shared_ptr<service> locate(string_view) const;
    shared_ptr<service> locate(service::id) const;

private:
    shared_ptr<service_registry> registry_;
    shared_ptr<service_locator> locator_;
};

}}

#endif // SONIA_SERVICES_HOST_HPP
