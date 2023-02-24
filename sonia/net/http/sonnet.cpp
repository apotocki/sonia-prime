//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonnet.hpp"
#include "sonia/exceptions.hpp"

#include "sonia/net/uri.hpp"
#include "sonia/net/uri.ipp"

#include "sonia/logger/loggable.hpp"

namespace sonia::http {

void sonnet::handle(request & req, response & resp)
{
    resp.set_header(header::ACCESS_CONTROL_ALLOW_ORIGIN, "*");
    resp.set_header(header::ACCESS_CONTROL_ALLOW_HEADERS, "Pragma");

    if (req.method == method_verb::OPTIONS) {
        resp.set_header(header::ALLOW, cstring_view("GET,POST,OPTIONS"));
        resp.status_code = status::OK;
        return;
    }

    cstring_view uri = req.get_relative_uri();
    auto uriparts = net::parse_uri(uri);

    method_handler_type const* handler = nullptr;

    if (uriparts.path.size() >= path_base_.size() && std::equal(path_base_.begin(), path_base_.end(), uriparts.path.begin())) {
        string_view path = uriparts.path;
        path.advance_front(path_base_.size());

        auto it = handlers_.find(path, hasher(), string_equal_to());
        if (it != handlers_.end()) {
            handler = &it->second;
        }
    }

    request_parameters rparams;

    // handle post/form params
    if (req.method == method_verb::POST) {
        req.tokenize_header(header::CONTENT_TYPE, [this, &rparams](string_view nm, string_view val, char d) {
            if (nm == "multipart/form-data") rparams.has_form_data = true;
            else if (nm == "application/x-www-form-urlencoded") rparams.is_urlencoded = true;
            else if (nm == "boundary") rparams.form_data_boundary = val;
            return true;
        });
        if (rparams.has_form_data && rparams.is_urlencoded) {
            throw exception("wrong content type header value");
        }
        if (rparams.is_urlencoded) {
            req.parse_body_as_x_www_form_urlencoded();
        }
    }
    //resp.meet_request(req);
    
    optional<sonnet_exception> excpt;
    try {
        if (handler) {
            (*handler)(req, rparams, resp);
        } else {
            handle_unhandled(req, rparams, resp);
        }
    } catch (sonnet_exception const& e) {
        excpt = e;
    } catch (...) {
        req.keep_alive = false;
        auto dinfo = boost::current_exception_diagnostic_information();
        if (loggable* lp = dynamic_cast<loggable*>(this); lp) {
            LOG_TRACE(lp->logger()) << dinfo;
        } else {
            GLOBAL_LOG_TRACE() << dinfo;
        }
        excpt = sonnet_exception(status::INTERNAL_SERVER_ERROR, dinfo);
    }

    if (req.keep_alive.value_or(false)) {
        resp.set_header(header::CONNECTION, "close");
        req.keep_alive = false;
    }
    // can't handle exception in catch due to possible fiber stack unwind issues
    if (excpt) {
        req.eat_body();
        resp.make_custom(excpt->s, "text/html", excpt->what());
    }
}

void sonnet::handle_unhandled(request &, request_parameters const&, response & resp)
{
    resp.make404();
}

void sonnet::bind_handler(string_view path, method_handler_type const& h)
{
    auto rpair = handlers_.insert(std::pair(to_string(path), h));
    if (!rpair.second) {
        THROW_INTERNAL_ERROR("path '%1%' is already bound"_fmt % path);
    }
}

void sonnet::handle_json_callback(request & req, response & resp, function<void(message::range_write_input_iterator)> writer)
{
    auto callback = req.get_parameter("callback");
    if (callback.size() > 1) {
        return resp.make_custom(http::status::BAD_REQUEST, "text/html", "bad callback parameter");
    }

    resp.status_code = status::OK;
    resp.set_header(header::CONTENT_TYPE, "application/json");

    resp.content_writer = [callback, writer = std::move(writer)](message::range_write_input_iterator it) {
        if (!callback.empty()) {
            copy_range(callback[0], it);
            copy_range(string_view("("), it);
        }
        writer(it);
        if (!callback.empty()) {
            copy_range(string_view(")"), it);
        }
    };
}

}
