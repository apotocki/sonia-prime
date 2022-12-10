//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/regex.hpp>
#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/net/http/application.hpp"

namespace sonia::services {

struct http_route
{
    optional<std::string> id;
    std::string application_name;
    boost::regex pathre{""};
    mutable shared_ptr<http::application> application;
    mutable bool enabled;
};

class route_selector
{
public:
    virtual ~route_selector() = default;

    virtual void enable_route(string_view routeid, bool enable_val) = 0;
};

}
