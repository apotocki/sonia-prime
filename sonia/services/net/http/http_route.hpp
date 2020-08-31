//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERVICES_HTTP_ROUTE_HPP
#define SONIA_SERVICES_HTTP_ROUTE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <string>
#include <boost/regex.hpp>
#include "sonia/shared_ptr.hpp"
#include "sonia/net/http/application.hpp"

namespace sonia::services {

struct http_route
{
    std::string application_name;
    boost::regex pathre{""};
    mutable shared_ptr<http::application> application;
};

}

#endif // SONIA_SERVICES_HTTP_ROUTE_HPP
