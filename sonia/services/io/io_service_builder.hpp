//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_IO_SERVICE_BUILDER_HPP
#define SONIA_SERVICES_IO_SERVICE_BUILDER_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/services/service.hpp"
#include "sonia/services/builder.hpp"
#include "sonia/utility/parameters/parameters.hpp"
#include "io_service_configuration.hpp"

namespace sonia::services {

class io_service_builder 
    : public service
    , public builder
{
public:
    io_service_builder();
    shared_ptr<service> build(json_object const& parameters) override;

private:
    sonia::parameters::parameters_description<io_service_configuration> parameters_;
};

}

#endif // SONIA_SERVICES_IO_SERVICE_BUILDER_HPP
