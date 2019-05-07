//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/exceptions.hpp"
#include "message.hpp"
#include "sonia/net/uri.hpp"
#include "sonia/net/uri.ipp"
#include "sonia/utility/functional/hash/variant.hpp"

namespace sonia::http {

struct header_param_converter : boost::static_visitor<message::any_header_t>
{
    message::any_header_t operator()(header h) const { return h; }
    message::any_header_t operator()(string_view h) const { return to_string(h); }
};

struct header_equal_to
{
    bool operator()(message::any_header_param_t const& l, message::any_header_t const& r) const
    {
        if (l.which() != r.which()) return false;
        header const* lh = boost::get<header>(&l);
        if (lh) return *lh == boost::get<header>(r);
        return boost::get<string_view>(l) == boost::get<std::string>(r);
    }
};

struct to_rvstring_visitor : boost::static_visitor<std::string>
{
    std::string operator()(string_view sv) const { return to_string(sv); }
    std::string operator()(std::string& s) const { return std::move(s); }
};

struct to_string_view_visitor : boost::static_visitor<string_view>
{
    string_view operator()(string_view sv) const { return sv; }
    string_view operator()(std::string const& s) const { return s; }
};

std::string header_string(message::header_value_param_t & v)
{
    return boost::apply_visitor(to_rvstring_visitor(), v);
}

string_view message::header_name(any_header_t const& hn)
{
    header const* h = boost::get<header>(&hn);
    if (h) return to_string(*h);
    return boost::get<std::string>(hn);
}

void message::set_header(any_header_param_t h, header_value_param_t v)
{
    auto it = headers.find(h, hasher(), header_equal_to());
    if (it == headers.end()) {
        headers.insert(it, std::pair(boost::apply_visitor(header_param_converter(), h), std::vector{header_string(v)}));
    } else {
        it->second.clear();
        it->second.push_back(header_string(v));
    }
}

void message::add_header(any_header_param_t h, header_value_param_t v)
{
    auto it = headers.find(h, hasher(), header_equal_to());
    if (it == headers.end()) {
        headers.insert(it, std::pair(boost::apply_visitor(header_param_converter(), h), std::vector{header_string(v)}));
    } else {
        it->second.push_back(header_string(v));
    }
}

void message::remove_header(any_header_param_t h)
{
    auto it = headers.find(h, hasher(), header_equal_to());
    if (it != headers.end()) {
        headers.erase(it);
    }
}

array_view<const std::string> message::get_header(any_header_param_t h) const
{
    auto it = headers.find(h, hasher(), header_equal_to());
    if (it != headers.end()) {
        return to_array_view(it->second);
    }
    return array_view<const std::string>{};
}

void message::tokenize_header(any_header_param_t h, function<bool(string_view, string_view, char)> const& handler) const
{
    auto hvals = get_header(h);
    for (std::string const& hval : hvals) {
        string_view hvalvw{hval};
        auto start = hvalvw.begin(), it = hvalvw.begin(), eit = hvalvw.end();
        while (it != eit) {
            if (*it == ' ') {
                ++it;
                start = it;
                continue;
            }
            if (*it == ';' || *it == ',') {
                if (!handler(string_view{start, it}, string_view{}, *it)) return;
                ++it;
                start = it;
                continue;
            }
            if (*it != '=') {
                ++it;
                continue;
            }
            auto valstart = it;
            ++valstart;
            auto valit = valstart;
            for (; valit != eit && *valit != ';' && *valit != ','; ++valit);
            if (!handler(string_view{start, it}, string_view{valstart, valit}, valit != eit ? *valit : '\0')) return;
            if (valit == eit) {
                it = start = eit;
                break;
            }
            ++valit;
            it = start = valit;
        }
        if (start != it) {
            if (!handler(string_view{start, it}, string_view{}, it != eit ? *it : '\0')) return;
        }
    }
}

request::request(string_view requri)
{
    set_uri(requri, false);
}

cstring_view request::get_relative_uri() const
{
    if (relative_uri_.empty()) {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    return {&relative_uri_.front(), relative_uri_.size() - 1};
}

void request::set_uri(string_view uri, bool ignore_abs_parts)
{
    auto parts = net::parse_uri(uri);
    if (parts.is_absolute && !ignore_abs_parts) {
        host = to_string(parts.host);
        port = parts.port.value_or(net::default_scheme_port(parts.scheme));
        set_header(header::HOST, host);
    }
    relative_uri_.clear();
    net::canonize_path(parts.path, std::back_inserter(relative_uri_));
    if (parts.query) {
        relative_uri_.push_back('?');
        std::copy(parts.query.begin(), parts.query.end(), std::back_inserter(relative_uri_));
        net::decode_query(parts.query.begin(), parts.query.end(), [this](std::string nm, std::string val) {
            add_parameter(std::move(nm), std::move(val));
        });
    }
    relative_uri_.push_back('\0');
}

void request::add_parameter(parameter_arg_t name, parameter_arg_t value)
{
    string_view pname = boost::apply_visitor(to_string_view_visitor(), name);
    auto it = parameters_.find(pname, hasher(), string_equal_to());
    if (it == parameters_.end()) {
        parameters_.insert(it, std::pair(
            boost::apply_visitor(to_rvstring_visitor(), name),
            parameter_value_t{boost::apply_visitor(to_rvstring_visitor(), value)})
        );
    } else {
        it->second.push_back(boost::apply_visitor(to_rvstring_visitor(), value));
    }
}

response::response(status code, optional<std::string> status_str)
    : status_code(code), status_string(std::move(status_str))
{

}

void response::meet_request(request const& r)
{
    r.tokenize_header(header::ACCEPT_ENCODING, [this](string_view n, string_view v, char d) {
        if (!v.empty()) return true;
        if (n == "gzip") {
            set_header(header::CONTENT_ENCODING, string_view("gzip"));
            return false;
        }
        if (n == "deflate") {
            set_header(header::CONTENT_ENCODING, string_view("deflate"));
            return false;
        }
        return true;
    });
}

}
