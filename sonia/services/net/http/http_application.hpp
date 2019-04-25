//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HTTP_APPLICATION_HPP
#define SONIA_SERVICES_HTTP_APPLICATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/net/http/message.hpp"

namespace sonia::services {

class http_application
{
public:
    virtual ~http_application() = default;

    virtual void handle(http::request & req, http::response & resp) = 0;
};

}

#endif // SONIA_SERVICES_HTTP_APPLICATION_HPP
