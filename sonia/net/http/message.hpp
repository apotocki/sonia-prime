//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <vector>

#include <unordered_map>

#include "sonia/optional.hpp"
#include "sonia/variant.hpp"
#include "sonia/function.hpp"
#include "sonia/span.hpp"
#include "sonia/utility/json/value.hpp"
#include "sonia/utility/iterators/wrapper_iterator.hpp"
#include "sonia/utility/iterators/chain_iterator.hpp"
#include "sonia/utility/iterators/http_form_data_read_iterator.hpp"

#include "http.hpp"
#include "status_codes.hpp"
#include "verb.hpp"
#include "headers.hpp"

namespace sonia::http {

using any_header_t = variant<header, std::string>;
using any_header_param_t = variant<header, std::string_view>;
using header_value_param_t = variant<std::string_view, std::string>;

struct header_param_converter
{
    any_header_t operator()(header h) const { return h; }
    any_header_t operator()(std::string_view h) const { return std::string(h); }
};

struct header_equal_to
{
    using is_transparent = void;
    
    template <typename LT, typename RT>
    bool operator()(LT const& l, RT const& r) const
    {
        return std::visit([](auto const& lh, auto const& rh) -> bool {
            if constexpr (!(std::is_same_v<std::decay_t<decltype(lh)>, header> ^ std::is_same_v<std::decay_t<decltype(rh)>, header>)) {
                return lh == rh;
            } else {
                return false;
            }}, l, r);
    }
};

std::string_view to_string_view(any_header_t const&);

using headers_t = std::unordered_map<any_header_t, std::vector<std::string>, hasher, header_equal_to>;

class header_collection
{
public:
    headers_t headers;

    span<const std::string> get_header(any_header_param_t) const;

    void tokenize_header(any_header_param_t, function<bool(std::string_view, std::string_view, char)> const& handler) const;

    void set_header(any_header_param_t, header_value_param_t);
    void add_header(any_header_param_t, header_value_param_t);
    void remove_header(any_header_param_t);
       
    template <size_t N> void set_header(any_header_param_t h, const char(&hv)[N]) { set_header(h, std::string_view{hv, hv[N - 1] ? N : N -1}); }
    template <size_t N> void add_header(any_header_param_t h, const char(&hv)[N]) { add_header(h, std::string_view{hv, hv[N - 1] ? N : N -1}); }
};

class message : public header_collection
{
public:
    virtual ~message() = default;

    using range_write_impl_type = proxying_iterator_polymorphic<std::span<char>>;
    using range_write_input_iterator = wrapper_iterator<
        range_write_impl_type*,
        std::span<char>,
        std::input_iterator_tag,
        span<char>
    >;

    using content_read_iterator_t = chain_linkable_iterator<std::span<const char>, std::input_iterator_tag>;

    int version{11}; // http protocol version * 10. e.g. 1.1 -> 11
    optional<bool> keep_alive;

    function<void(range_write_input_iterator)> content_writer;
    content_read_iterator_t input;

    template <typename ReadIteratorT>
    void build_input_iterator(ReadIteratorT & ii);

    std::string get_body_as_string();
    size_t eat_body();
};

struct form_data_item : header_collection
{
    std::string_view name, filename;
    string_view boundary;
    mutable http_form_data_read_iterator<message::content_read_iterator_t> input;
};

class request : public message
{
public:
    request() = default;

    explicit request(string_view requri);

    verb method{ verb::GET };
    std::string host;
    uint16_t port;
    
    cstring_view get_relative_uri() const;
    void set_uri(string_view, bool ignore_abs_parts);

    using parameter_arg_t = variant<std::string_view, std::string>;

    void add_parameter(parameter_arg_t name, parameter_arg_t value);
    std::span<const std::string> get_parameter(std::string_view name) const;

    void set_property(std::string_view name, json_value value);
    json_value const* get_property(std::string_view name) const;

    using parameter_value_t = std::vector<std::string>;
    using parameters_t = std::unordered_map<std::string, parameter_value_t, hasher, string_equal_to>;

    parameters_t parameters;

    using properties_t = std::unordered_map<std::string, json_value, hasher, string_equal_to>;
    properties_t properties;

    void parse_body_as_x_www_form_urlencoded();
    void parse_body_as_multipart_form_data(std::string_view boundary, function<void(form_data_item const&)> const& handler);

private:
    std::vector<char> relative_uri_;
};

class response : public message
{
public:
    explicit response(status code = status::OK, optional<std::string> status_str = nullopt);

    void meet_request(request const&);
    void meet_keep_alive(request const&);

    void make401(std::string_view auth_type, std::string_view realm, std::string_view opaque, std::string_view nonce);
    void make404(optional<std::string> = nullopt);
    void make_custom(status code, std::string_view ct, std::string_view body);
    void make_moved_temporarily_302(std::string_view location);

    status status_code;
    optional<std::string> status_string;
};

}
