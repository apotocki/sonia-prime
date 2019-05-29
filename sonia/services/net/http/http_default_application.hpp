//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HTTP_DEFAULT_APPLICATION_HPP
#define SONIA_SERVICES_HTTP_DEFAULT_APPLICATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/services/service.hpp"
#include "sonia/net/http/application.hpp"
#include "http_default_application_configuration.hpp"

namespace sonia::services {

class http_default_application
    : public service
    , public http::application
{
public:
    explicit http_default_application(http_default_application_configuration cfg);

    void handle(http::request & req, http::response & resp) override;

    http_default_application_configuration cfg_;
};

}

#endif // SONIA_SERVICES_HTTP_DEFAULT_APPLICATION_HPP
