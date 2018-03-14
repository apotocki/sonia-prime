//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICE_LOCAL_SERVICE_REGISTRY_HPP
#define SONIA_SERVICE_LOCAL_SERVICE_REGISTRY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "service.hpp"

namespace sonia {

class local_service_registry : public service_registry {
public:
    service::id_type get_id(string_view) const override;
    string const* get_name(service::id_type) const override;
};

}

#endif // SONIA_SERVICE_LOCAL_SERVICE_REGISTRY_HPP
