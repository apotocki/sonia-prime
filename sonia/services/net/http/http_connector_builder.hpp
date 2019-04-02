//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HTTP_CONNECTOR_BUILDER_HPP
#define SONIA_SERVICES_HTTP_CONNECTOR_BUILDER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/services/service.hpp"
#include "sonia/services/builder.hpp"
#include "sonia/utility/parameters/parameters.hpp"
#include "http_connector_configuration.hpp"

namespace sonia::services {

//DECLARE_PARTICULAR_BUILDER(http_connector)

class http_connector_builder 
    : public service
    , public builder
{
public:
    http_connector_builder();
    shared_ptr<service> build(json_object const& parameters) override;

private:
    sonia::parameters::parameters_description<http_connector_configuration> parameters_;
};

}

#endif // SONIA_SERVICES_HTTP_CONNECTOR_BUILDER_HPP
