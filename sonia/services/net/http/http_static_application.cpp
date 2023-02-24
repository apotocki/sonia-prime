//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/utility/algorithm/copy.hpp"
#include "http_static_application.hpp"

#include <fstream>

#include "sonia/net/uri.hpp"
#include "sonia/net/uri.ipp"
#include "sonia/services.hpp"

#include "sonia/utility/iterators/file_region_iterator.hpp"

namespace sonia::services {

http_static_application::http_static_application(http_static_application_configuration cfg)
    : www_path_(std::move(cfg.www_path))
    , forwards_(std::move(cfg.forward_mapping))
    , cache_contol_header(std::move(cfg.cache_contol_header))
    , mime_map_{ fs::canonical(cfg.mime_mapping_file_path) }
{
    set_log_attribute("Type", "http-static-application");

    sys_path_ = fs::canonical(cfg.sys_path);

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
        cstring_view relpath = array_view(canuri);
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
        
        if (auto const* ptype = mime_map_.find(ext); ptype) {
            resp.set_header(http::header::CONTENT_TYPE, *ptype + std::string("; charset=UTF-8"));
        } else {
            LOG_WARN(logger()) << "undefined mime type for extension: '" << ext << "', path: '" << uri << "'";
            resp.set_header(http::header::CONTENT_TYPE, "text/html; charset=UTF-8");
        }

        try {
            auto abspath = fs::canonical(sys_path_ / relpath.c_str());

            if (!fs::exists(abspath) || !fs::is_regular_file(abspath)) {
                handle404(req, resp);
                return;
            }

            /*
            time_t now = time(NULL);
            if (now != -1) {
                if (tm* ptm = gmtime(&now); ptm) {
                    resp.set_header(http::header::DATE, sonia::http::http_date(ptm));
                }
            }
            
            int64_t dt = 0; // 100 nanoseconds intervals since Jan 1 1970 UTC
            fs::file_time_type ft = fs::last_write_time(abspath);
#ifdef BOOST_WINDOWS
            dt = clock_cast<std::chrono::utc_clock>(ft).time_since_epoch().count();
#else
            //dt = clock_cast<std::chrono::utc_clock>(ft).time_since_epoch().count();
            dt = ft.time_since_epoch().count() / 100;
#endif
            
            dt /= 10000000; // to seconds
            if (tm* ptm = gmtime(&dt); ptm) {
                resp.set_header(http::header::LAST_MODIFIED, sonia::http::http_date(ptm));
            }
            */
            if (!cache_contol_header.empty()) {
                resp.set_header(http::header::CACHE_CONTROL, cache_contol_header);
            }

            resp.set_header(http::header::TRANSFER_ENCODING, "chunked");
            
            
            

            //Last - Modified: Tue, 22 Feb 2022 22 : 00 : 00 GMT
            //Cache - Control : max - age = 3600

            //resp.set_header(http::header::CONTENT_LENGTH, std::to_string(fs::file_size(abspath)));

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
