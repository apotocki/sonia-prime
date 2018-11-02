//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_NET_SERVICE_BUILDER_HPP
#define SONIA_SERVICES_NET_SERVICE_BUILDER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/services/service.hpp"
#include "sonia/services/builder.hpp"
#include "sonia/utility/parameters/parameters.hpp"
#include "net_service_configuration.hpp"

namespace sonia { namespace services {

//DECLARE_PARTICULAR_BUILDER(net_service)

class net_service_builder 
    : public service
    , public builder
{
public:
    net_service_builder();
    shared_ptr<service> build(json_object const& parameters) override;

private:
    sonia::parameters::parameters_description<net_service_configuration> parameters_;
};

}}

#endif // SONIA_SERVICES_NET_SERVICE_BUILDER_HPP
