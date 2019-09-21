//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_HTTP_MESSAGE_HPP
#define SONIA_HTTP_MESSAGE_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <vector>

#include <boost/variant.hpp>
#include <boost/unordered_map.hpp>

#include "sonia/optional.hpp"
#include "sonia/array_view.hpp"
#include "sonia/function.hpp"
#include "sonia/utility/json/value.hpp"
#include "sonia/utility/iterators/wrapper_iterator.hpp"
#include "sonia/utility/iterators/chain_iterator.hpp"
#include "sonia/utility/iterators/http_form_data_read_iterator.hpp"
#include "sonia/utility/functional/hash/variant.hpp"

#include "http.hpp"

namespace sonia::http {

using any_header_t = boost::variant<header, std::string>;
using any_header_param_t = boost::variant<header, string_view>;
using header_value_param_t = boost::variant<string_view, std::string>;
using headers_t = boost::unordered_map<any_header_t, std::vector<std::string>, hasher>;

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

class header_collection
{
public:
    headers_t headers;

    array_view<const std::string> get_header(any_header_param_t) const;

    void tokenize_header(any_header_param_t, function<bool(string_view, string_view, char)> const& handler) const;

    void set_header(any_header_param_t, header_value_param_t);
    void add_header(any_header_param_t, header_value_param_t);
    void remove_header(any_header_param_t);
       
    template <size_t N> void set_header(any_header_param_t h, const char(&hv)[N]) { set_header(h, string_view{hv, hv[N - 1] ? N : N -1}); }
    template <size_t N> void add_header(any_header_param_t h, const char(&hv)[N]) { add_header(h, string_view{hv, hv[N - 1] ? N : N -1}); }

    static string_view header_name(any_header_t const&);
};

class message : public header_collection
{
public:
    virtual ~message() = default;

    using range_write_impl_type = proxying_iterator_polymorphic<array_view<char>>;
    using range_write_input_iterator = wrapper_iterator<
        range_write_impl_type*,
        array_view<char>,
        std::input_iterator_tag,
        array_view<char>
    >;

    using range_read_impl_type = proxying_iterator_polymorphic<array_view<const char>>;
    using range_read_input_iterator = wrapper_iterator<
        range_read_impl_type*,
        array_view<const char>,
        std::input_iterator_tag,
        array_view<const char>
    >;

    using content_read_iterator_t = chain_linkable_iterator<array_view<const char>, std::input_iterator_tag>;

    int version{11}; // http protocol version * 10. e.g. 1.1 -> 11

    function<void(range_write_input_iterator)> content_writer;
    content_read_iterator_t input;

    template <typename ReadIteratorT>
    void build_input_iterator(ReadIteratorT & ii);

    std::string get_body_as_string();
};

struct form_data_item : header_collection
{
    string_view name, filename;
    string_view boundary;
    mutable http_form_data_read_iterator<message::content_read_iterator_t> input;
};

class request : public message
{
public:
    request() = default;

    explicit request(string_view requri);

    method_verb method{method_verb::GET};
    std::string host;
    uint16_t port;
    
    cstring_view get_relative_uri() const;
    void set_uri(string_view, bool ignore_abs_parts);

    using parameter_arg_t = boost::variant<string_view, std::string>;

    void add_parameter(parameter_arg_t name, parameter_arg_t value);
    array_view<std::string const> get_parameter(string_view name) const;

    void set_property(string_view name, json_value value);
    json_value const* get_property(string_view name) const;

    using parameter_value_t = std::vector<std::string>;
    using parameters_t = boost::unordered_map<std::string, parameter_value_t, hasher>;

    parameters_t parameters;

    using properties_t = boost::unordered_map<std::string, json_value, hasher>;

    properties_t properties;

    void parse_body_as_x_www_form_urlencoded();
    void parse_body_as_multipart_form_data(string_view boundary, function<void(form_data_item const&)> const& handler);

private:
    std::vector<char> relative_uri_;
};

class response : public message
{
public:
    explicit response(status code = status::OK, optional<std::string> status_str = nullopt);

    void meet_request(request const&);

    void make401(string_view auth_type, string_view realm, string_view opaque, string_view nonce);
    void make404(optional<std::string> = nullopt);
    void make_custom(status code, string_view ct, string_view body);
    void make_moved_temporarily_302(string_view location);

    status status_code;
    optional<std::string> status_string;
};

}

#endif // SONIA_HTTP_MESSAGE_HPP
