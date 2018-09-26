//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICE_LOCAL_SERVICE_REGISTRY_HPP
#define SONIA_SERVICE_LOCAL_SERVICE_REGISTRY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "service.hpp"
#include "sonia/utility/basic_local_registry.hpp"

namespace sonia {

class local_service_registry 
    : public service_registry
    , utility::basic_local_registry<local_service_registry, service::id>
{
    typedef utility::basic_local_registry<local_service_registry, service::id> base_t;
    friend base_t;

public:
    explicit local_service_registry(shared_ptr<persister> sp);

    service::id get_id(string_view) override;
    string_view get_name(service::id) const override;
};

}

#endif // SONIA_SERVICE_LOCAL_SERVICE_REGISTRY_HPP
