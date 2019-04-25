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
#include "sonia/utility/iterators/wrapper_iterator.hpp"

#include "http.hpp"

namespace sonia::http {
    
class message
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

    using any_header_t = boost::variant<header, std::string>;
    using any_header_param_t = boost::variant<header, string_view>;
    using header_value_param_t = boost::variant<string_view, std::string>;
    using headers_t = boost::unordered_map<any_header_t, std::vector<std::string>, hasher>;

    int version{11}; // http protocol version * 10. e.g. 1.1 -> 11

    headers_t headers;

    array_view<const std::string> get_header(any_header_param_t) const;
    void tokenize_header(any_header_param_t, function<bool(string_view, string_view, char)> const& handler) const;

    void set_header(any_header_param_t, header_value_param_t);
    void add_header(any_header_param_t, header_value_param_t);
    void remove_header(any_header_param_t);
       
    static string_view header_name(any_header_t const&);

    function<void(range_write_input_iterator)> content_writer;
};

class request : public message
{
public:
    request() {}

    explicit request(string_view requri);

    method_verb method{method_verb::GET};
    std::string host;
    uint16_t port;
    
    cstring_view get_relative_uri() const;
    void set_uri(string_view, bool ignore_abs_parts);

    using parameter_arg_t = boost::variant<string_view, std::string>;

    void add_parameter(parameter_arg_t name, parameter_arg_t value);

private:
    using parameter_value_t = std::vector<std::string>;
    using parameters_t = boost::unordered_map<std::string, parameter_value_t, hasher>;

    parameters_t parameters_;
    std::vector<char> relative_uri_;
};

class response : public message
{
public:
    explicit response(status code = status::OK, optional<std::string> status_str = nullopt);

    void meet_request(request const&);

    status status_code;
    optional<std::string> status_string;
};

}

#endif // SONIA_HTTP_MESSAGE_HPP
