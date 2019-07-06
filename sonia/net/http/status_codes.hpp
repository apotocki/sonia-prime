//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_HTTP_STATUS_CODES_HPP
#define SONIA_HTTP_STATUS_CODES_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <iosfwd>

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include "sonia/string.hpp"

namespace sonia::http {
    
#define SONIA_HTTP_STATUS_CODES_SEQ                                             \
    ((CONTINUE, "Continue", 100))                                               \
    ((SWITCHING_PROTOCOLS, "Switching Protocols", 101))                         \
    ((OK, "OK", 200))                                                           \
    ((CREATED, "Created", 201))                                                 \
    ((ACCEPTED, "Accepted", 202))                                               \
    ((NON_AUTHORIRATIVE_INFORMATION, "Non-Authoritative Information", 203))     \
    ((NO_CONTENT, "No Content", 204))                                           \
    ((RESET_CONTENT, "Reset Content", 205))                                     \
    ((PARTIAL_CONTENT, "Partial Content", 206))                                 \
    ((MULTIPLE_CHOICES, "Multiple Choices", 300))                               \
    ((MOVED_PERMANENTLY, "Moved Permanently", 301))                             \
    ((FOUND, "Moved Temporarily", 302))                                         \
    ((SEE_OTHER, "See Other", 303))                                             \
    ((NOT_MODIFIED, "Not Modified", 304))                                       \
    ((USE_PROXY, "Use Proxy", 305))                                             \
    ((TEMPORARY_REDIRECT, "Temporary Redirect", 307))                           \
    ((BAD_REQUEST, "Bad Request", 400))                                         \
    ((UNAUTHORIZED, "Unauthorized", 401))                                       \
    ((PAYMENT_REQUIRED, "Payment Required", 402))                               \
    ((FORBIDDEN, "Forbidden", 403))                                             \
    ((NOT_FOUND, "Not Found", 404))                                             \
    ((METHOD_NOT_ALLOWED, "Method Not Allowed", 405))                           \
    ((NOT_ACCEPTABLE, "Not Acceptable", 406))                                   \
    ((PROXY_AUTHENTICATION_REQUIRED, "Proxy Authentication Required", 407))     \
    ((REQUEST_TIMEOUT, "Request Timeout", 408))                                 \
    ((CONFLICT, "Conflict", 409))                                               \
    ((GONE, "Gone", 410))                                                       \
    ((LENGTH_REQUIRED, "Length Required", 411))                                 \
    ((PRECONDITION_FAILED, "Precondition Failed", 412))                         \
    ((REQUEST_ENTITY_TOO_LARGE, "Request Entity Too Large", 413))               \
    ((REQUEST_URI_TOO_LONG, "Request-URI Too Long", 414))                       \
    ((UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type", 415))                   \
    ((REQUESTED_RANGE_NOT_SATISFIABLE, "Requested Range Not Satisfiable", 416)) \
    ((EXPECTATION_FAILED, "Expectation Failed", 417))                           \
    ((INTERNAL_SERVER_ERROR, "Internal Server Error", 500))                     \
    ((NOT_IMPLEMENTED, "Not Implemented", 501))                                 \
    ((BAD_GATEWAY, "Bad Gateway", 502))                                         \
    ((SERVICE_UNAVAILABLE, "Service Unavailable", 503))                         \
    ((GATEWAY_TIMEOUT, "Gateway Timeout", 504))                                 \
    ((HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported", 505))           \
    ((CONNECTION_TIMED_OUT, "Connection Timed Out", 522))                       \
    ((A_TIMEOUT_OCCURRED, "A Timeout Occurred", 524)) 

#define SONIA_PRINT_TYPE_ENUM(r, data, i, elem) BOOST_PP_TUPLE_ELEM(3, 0, elem) = BOOST_PP_TUPLE_ELEM(3, 2, elem),

enum class status
{
    BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_TYPE_ENUM, _, SONIA_HTTP_STATUS_CODES_SEQ)
};

std::string const& to_string(status);

template <typename CharT, class TraitsT>
std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, status s)
{
    return os << to_string(s);
}

#undef SONIA_PRINT_TYPE_ENUM

}

#endif // SONIA_HTTP_STATUS_CODES_HPP
