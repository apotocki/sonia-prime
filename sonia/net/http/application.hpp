//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HTTP_APPLICATION_HPP
#define SONIA_SERVICES_HTTP_APPLICATION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/net/http/message.hpp"

namespace sonia::http {

class application
{
public:
    virtual ~application() = default;

    virtual void handle(request & req, response & resp) = 0;
};

}

#endif // SONIA_SERVICES_HTTP_APPLICATION_HPP
