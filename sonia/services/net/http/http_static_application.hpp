//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/string.hpp"
#include "sonia/filesystem.hpp"
#include "sonia/services/service.hpp"
#include "sonia/net/http/application.hpp"
#include "http_static_application_configuration.hpp"
#include "mime_mapping.hpp"

namespace sonia::services {

class http_static_application
    : public service
    , public http::application
{
public:
    explicit http_static_application(http_static_application_configuration cfg);

    void handle(http::request & req, http::response & resp) override;

    void handle404(http::request & req, http::response & resp);

private:
    mime_mapping mime_map_;
    std::string www_path_;
    std::string cache_contol_header;
    fs::path sys_path_;
    std::vector<std::pair<boost::regex, std::string>> forwards_;
    shared_ptr<http::application> app404_;
};

}
