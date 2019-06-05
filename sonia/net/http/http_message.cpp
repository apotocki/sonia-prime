//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/exceptions.hpp"
#include "message.hpp"
#include "sonia/net/uri.hpp"
#include "sonia/net/uri.ipp"
#include "sonia/utility/functional/hash/variant.hpp"
#include "sonia/utility/iterators/range_dereferencing_iterator.hpp"


namespace sonia::http {

struct header_param_converter : boost::static_visitor<any_header_t>
{
    any_header_t operator()(header h) const { return h; }
    any_header_t operator()(string_view h) const { return to_string(h); }
};

struct header_equal_to
{
    bool operator()(any_header_param_t const& l, any_header_t const& r) const
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

std::string header_string(header_value_param_t & v)
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

std::string message::get_body_as_string()
{
    std::string result;
    while (!input.empty()) {
        array_view<const char> rng = *input; ++input;
        result.insert(result.end(), boost::begin(rng), boost::end(rng));
    }
    return result;
}

template <typename InputIterator>
char uri_char_decoder(InputIterator & it)
{
    char c = *it; ++it;
    if (c == '+') return ' ';
    if (c == '%') {
        uint8_t result = 0;
        unsigned int dc = 0;
        for (; !it.empty() && dc < 2; ++it, ++dc) {
            const char c0 = *it;
            uint8_t v = parsers::get_hexdigit(c0);
            if (v != 0xff) {
                result = result * 16 + v;
            } else {
                break;
            }
        }
        if (dc < 2) {
            throw exception("wrong urlencoded sequence");
        }
        return result;
    }
    return c;
}

void request::parse_body_as_x_www_form_urlencoded()
{
    range_dereferencing_iterator b{std::move(input)};
    do {
        std::string paramname;
        while (!b.empty()) {
            if ('=' == *b) { ++b; break; }
            char c = uri_char_decoder(b);
            paramname.push_back(c);
        }
        if (b.empty()) return;
        std::string paramval;
        while (!b.empty()) {
            if ('&' == *b) { ++b; break; }
            char c = uri_char_decoder(b);
            paramval.push_back(c);
        }
        add_parameter(paramname, paramval);
    } while (!b.empty());
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
    auto it = parameters.find(pname, hasher(), string_equal_to());
    if (it == parameters.end()) {
        parameters.insert(it, std::pair(
            boost::apply_visitor(to_rvstring_visitor(), name),
            parameter_value_t{boost::apply_visitor(to_rvstring_visitor(), value)})
        );
    } else {
        it->second.push_back(boost::apply_visitor(to_rvstring_visitor(), value));
    }
}

array_view<std::string const> request::get_parameter(string_view name) const
{
    auto it = parameters.find(name, hasher(), string_equal_to());
    if (it == parameters.end() || it->second.empty()) return array_view<std::string const>();
    return to_array_view(it->second);
}

void request::set_property(string_view name, json_value value)
{
    auto it = properties.find(name, hasher(), string_equal_to());
    if (it == properties.end()) {
        properties.insert(it, std::pair(to_string(name), value));
    } else {
        it->second = value;
    }
}

json_value const* request::get_property(string_view name) const
{
    auto it = properties.find(name, hasher(), string_equal_to());
    if (it == properties.end()) return nullptr;
    return &it->second;
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

void response::make401(string_view realm, string_view opaque, string_view nonce)
{
    std::ostringstream authval;
    authval << "Digest realm=\"" << realm << "\"";
    authval << ",qop=\"auth\"";
    authval << ",opaque=\"" << opaque << "\"";
    authval << ",nonce=\"" << nonce << "\"";

    set_header(header::WWW_AUTHENTICATE, authval.str());
    make_custom(status::UNAUTHORIZED, "text/html", "<h1>401 Unauthorized.</h1>");
}

void response::make404()
{
    set_header(header::CONTENT_TYPE, "text/html; charset=UTF-8");
    set_header(http::header::TRANSFER_ENCODING, "chunked");
    status_code = status::NOT_FOUND;
    content_writer = [](http::message::range_write_input_iterator it) {
        copy_range(string_view("<h1>404 Not Found.</h1>"), it);
    };
}

void response::make_custom(status code, string_view ct, string_view body)
{
    status_code = code;
    set_header(header::CONTENT_TYPE, ct);
    if (!body.empty()) {
        set_header(http::header::CONTENT_LENGTH, std::to_string(body.size()));
        content_writer = [body = to_string(body)](http::message::range_write_input_iterator it) {
            copy_range(body, it);
        };
    }
}

void response::make_moved_temporarily_302(string_view location)
{
    status_code = status::FOUND;
    set_header(http::header::CONTENT_LENGTH, "0");
    set_header(header::LOCATION, location);
}

}
