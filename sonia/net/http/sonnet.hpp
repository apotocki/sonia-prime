//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/unordered_map.hpp>

#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"

#include "application.hpp"

namespace sonia::http {

class sonnet : public application
{
public:
    sonnet();

    void handle(request & req, response & resp) override;

    struct request_parameters
    {
        bool is_urlencoded{false};
        bool has_form_data{false};
        string_view form_data_boundary;
    };

protected:
    using method_handler_type = function<void(request &, request_parameters const&, response &)>;
    void bind_handler(string_view, method_handler_type const&);
    void handle_json_callback(request & req, response & resp, function<void(message::range_write_input_iterator)> writer);

    virtual void handle_unhandled(request & req, request_parameters const&, response & resp);

    boost::unordered_map<std::string, method_handler_type, hasher> handlers_;
    std::string path_base_;
    std::string cached_server_header_;
};

class sonnet_exception : public exception
{
public:
    template <typename ... ArgsT>
    explicit sonnet_exception(status sval, ArgsT&& ... args) 
        : exception(std::forward<ArgsT>(args) ...)
        , s{ sval }
    {}
    status s;
};

}
