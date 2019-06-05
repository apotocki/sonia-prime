//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_NET_HTTP_SONNET_HPP
#define SONIA_NET_HTTP_SONNET_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/unordered_map.hpp>

#include "sonia/string.hpp"
#include "sonia/exceptions.hpp"

#include "application.hpp"

namespace sonia::http {

class sonnet : public application
{
public:
    void handle(request & req, response & resp) override;

protected:
    using method_handler_type = function<void(request &, response &)>;
    void bind_handler(string_view, method_handler_type const&);
    void handle_json_callback(request & req, response & resp, function<void(message::range_write_input_iterator)> const& writer);

    virtual void handle_unhandled(request & req, response & resp);


    boost::unordered_map<std::string, method_handler_type, hasher> handlers_;
    std::string path_base_;
};

class sonnet_exception : public exception
{
public:
    template <typename ... ArgsT>
    explicit sonnet_exception(status s, ArgsT&& ... args) : exception(std::forward<ArgsT>(args) ...) {}

    status s;
};

}

#endif // SONIA_NET_HTTP_SONNET_HPP
