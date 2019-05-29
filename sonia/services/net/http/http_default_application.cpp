//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/utility/algorithm/copy.hpp"
#include "http_default_application.hpp"

namespace sonia::services {

http_default_application::http_default_application(http_default_application_configuration cfg)
    : cfg_(std::move(cfg))
{
    set_log_attribute("Type", "http-default-application");
}

void http_default_application::handle(http::request & req, http::response & resp)
{
    resp.set_header(http::header::CONTENT_TYPE, "text/html; charset=UTF-8");
    resp.status_code = cfg_.response_code;

    std::ostringstream res;

    res << "<html>";
    res << "<head><title>" << cfg_.title << "</title></head>";
    res << "<body>";
    res << "<center><h2>" << cfg_.title << "</h2></center>";
    if (cfg_.image) {
        res << "<center><img src=\"" << *cfg_.image << "\"></center>";
    }
    if (cfg_.show_headers) {
        res << "<center><h2>Headers</h2></center>";
        res << "<center><table><tr><td>Name</td><td>Value</td></tr>";
        for (auto const& [hname, hvals] : req.headers) {
            for (auto const& hval : hvals) {
                res << "<tr><td>" << http::message::header_name(hname) << "</td>";
                res << "<td>" << hval << "</td></tr>";
            }
        }
        res << "</table></center>";
    }
    res << "</body>";
    res << "</html>";

    std::string body = res.str();
    //resp.set_header(http::header::CONTENT_LENGTH, std::to_string(body.size()));
    resp.meet_request(req);
    resp.set_header(http::header::TRANSFER_ENCODING, "chunked");
    resp.content_writer = [cnt = std::move(body)](http::message::range_write_input_iterator it) {
        copy_range(string_view(cnt), it);
    };
}

}
