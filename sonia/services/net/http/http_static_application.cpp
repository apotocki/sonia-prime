//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/utility/algorithm/copy.hpp"
#include "http_static_application.hpp"

#include <filesystem>
#include <boost/algorithm/string/trim.hpp>

#include <fstream>

#include "sonia/net/uri.hpp"
#include "sonia/net/uri.ipp"
#include "sonia/services.hpp"

#include "sonia/utility/iterators/file_region_iterator.hpp"

namespace sonia::services {

http_static_application::http_static_application(http_static_application_configuration cfg)
    : www_path_(std::move(cfg.www_path)), forwards_(std::move(cfg.forward_mapping))
{
    set_log_attribute("Type", "http-static-application");

    sys_path_ = std::filesystem::canonical(cfg.sys_path);

    auto mmpath = std::filesystem::canonical(cfg.mime_mapping_file_path);
    std::ifstream mmfile(mmpath.string());
    if (!mmfile.is_open()) {
        throw exception("can't open the mime-mapping file '%1%'"_fmt % mmpath);
    }
    
    while (!mmfile.eof()) {
        char line[256];
        mmfile.getline(line, sizeof(line));
        std::string rule(line);
        if (!rule.empty() && rule[0] == '#') continue;
        auto delit = std::find(rule.begin(), rule.end(), ',');
        if (delit == rule.end()) {
            continue;
        }
        std::string ext = boost::trim_copy(std::string{rule.begin(), delit});
        std::string mt = boost::trim_copy(std::string{delit + 1, rule.end()});
        mime_map_[ext] = mt;
    }

    if (cfg.page404app) {
        locate(*cfg.page404app, app404_);
    }
}

void http_static_application::handle404(http::request & req, http::response & resp)
{
    if (app404_) {
        app404_->handle(req, resp);
    } else {
        resp.make404();
    }
}

void http_static_application::handle(http::request & req, http::response & resp)
{
    cstring_view uri = req.get_relative_uri();
    auto uriparts = net::parse_uri(uri);

    std::vector<char> canuri;
    canuri.reserve(uriparts.path.size());
    net::canonize_path(uriparts.path, std::back_inserter(canuri));

    if (canuri.size() >= www_path_.size() && std::equal(www_path_.begin(), www_path_.end(), canuri.begin())) {
        canuri.push_back('\0');
        cstring_view relpath = to_array_view(canuri);
        relpath.advance_front(www_path_.size());

        // forwarding
        for (auto const& rule : forwards_) {
            if (boost::regex_match(relpath.c_str(), rule.first, boost::regex_constants::match_perl)) {
                relpath = rule.second;
                break;
            }
        }

        auto extdelimit = std::find(relpath.rbegin(), relpath.rend(), '.');
        string_view ext{extdelimit.base() == relpath.begin() ? relpath.end() : extdelimit.base(), relpath.end()};
        auto extit = mime_map_.find(ext, hasher(), string_equal_to());
        if (extit == mime_map_.end()) {
            LOG_WARN(logger()) << "undefined mime type for extension: '" << ext << "', path: '" << uri << "'";
            resp.set_header(http::header::CONTENT_TYPE, "text/html; charset=UTF-8");
        } else {
            resp.set_header(http::header::CONTENT_TYPE, extit->second + std::string("; charset=UTF-8"));
        }

        try {
            auto abspath = std::filesystem::canonical(sys_path_ / relpath.c_str());
            resp.set_header(http::header::TRANSFER_ENCODING, "chunked");
            //resp.set_header(http::header::CONTENT_LENGTH, std::to_string(std::filesystem::file_size(abspath)));
            file_region_iterator<const char> fit{abspath};

            resp.status_code = http::status::OK;
            resp.meet_request(req);

            // skip request body if exists
            for (; !req.input.empty(); ++req.input);

            resp.content_writer = [fit = std::move(fit)](http::message::range_write_input_iterator it) mutable {
                for (; !fit.empty(); ++fit) {
                    it = copy_range(array_view<const char>(*fit), it);
                }
            };
        } catch (...) {
            handle404(req, resp);
            return;
        }
    }
}

}
