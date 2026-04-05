//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <iosfwd>

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include "sonia/string.hpp"

namespace sonia::http {
    
#define SONIA_HTTP_STATUS_CODES_SEQ                                               \
    ((CONTINUE, "Continue"sv, 100))                                               \
    ((SWITCHING_PROTOCOLS, "Switching Protocols"sv, 101))                         \
    ((OK, "OK"sv, 200))                                                           \
    ((CREATED, "Created"sv, 201))                                                 \
    ((ACCEPTED, "Accepted"sv, 202))                                               \
    ((NON_AUTHORIRATIVE_INFORMATION, "Non-Authoritative Information"sv, 203))     \
    ((NO_CONTENT, "No Content"sv, 204))                                           \
    ((RESET_CONTENT, "Reset Content"sv, 205))                                     \
    ((PARTIAL_CONTENT, "Partial Content"sv, 206))                                 \
    ((MULTIPLE_CHOICES, "Multiple Choices"sv, 300))                               \
    ((MOVED_PERMANENTLY, "Moved Permanently"sv, 301))                             \
    ((FOUND, "Moved Temporarily"sv, 302))                                         \
    ((SEE_OTHER, "See Other"sv, 303))                                             \
    ((NOT_MODIFIED, "Not Modified"sv, 304))                                       \
    ((USE_PROXY, "Use Proxy"sv, 305))                                             \
    ((TEMPORARY_REDIRECT, "Temporary Redirect"sv, 307))                           \
    ((BAD_REQUEST, "Bad Request"sv, 400))                                         \
    ((UNAUTHORIZED, "Unauthorized"sv, 401))                                       \
    ((PAYMENT_REQUIRED, "Payment Required"sv, 402))                               \
    ((FORBIDDEN, "Forbidden"sv, 403))                                             \
    ((NOT_FOUND, "Not Found"sv, 404))                                             \
    ((METHOD_NOT_ALLOWED, "Method Not Allowed"sv, 405))                           \
    ((NOT_ACCEPTABLE, "Not Acceptable"sv, 406))                                   \
    ((PROXY_AUTHENTICATION_REQUIRED, "Proxy Authentication Required"sv, 407))     \
    ((REQUEST_TIMEOUT, "Request Timeout"sv, 408))                                 \
    ((CONFLICT, "Conflict"sv, 409))                                               \
    ((GONE, "Gone"sv, 410))                                                       \
    ((LENGTH_REQUIRED, "Length Required"sv, 411))                                 \
    ((PRECONDITION_FAILED, "Precondition Failed"sv, 412))                         \
    ((REQUEST_ENTITY_TOO_LARGE, "Request Entity Too Large"sv, 413))               \
    ((REQUEST_URI_TOO_LONG, "Request-URI Too Long"sv, 414))                       \
    ((UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type"sv, 415))                   \
    ((REQUESTED_RANGE_NOT_SATISFIABLE, "Requested Range Not Satisfiable"sv, 416)) \
    ((EXPECTATION_FAILED, "Expectation Failed"sv, 417))                           \
    ((INTERNAL_SERVER_ERROR, "Internal Server Error"sv, 500))                     \
    ((NOT_IMPLEMENTED, "Not Implemented"sv, 501))                                 \
    ((BAD_GATEWAY, "Bad Gateway"sv, 502))                                         \
    ((SERVICE_UNAVAILABLE, "Service Unavailable"sv, 503))                         \
    ((GATEWAY_TIMEOUT, "Gateway Timeout"sv, 504))                                 \
    ((HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported"sv, 505))           \
    ((CONNECTION_TIMED_OUT, "Connection Timed Out"sv, 522))                       \
    ((A_TIMEOUT_OCCURRED, "A Timeout Occurred"sv, 524)) 

#define SONIA_PRINT_TYPE_ENUM(r, data, i, elem) BOOST_PP_TUPLE_ELEM(3, 0, elem) = BOOST_PP_TUPLE_ELEM(3, 2, elem),

enum class status
{
    BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_TYPE_ENUM, _, SONIA_HTTP_STATUS_CODES_SEQ)
};

std::string_view to_string_view(status);

template <typename CharT, class TraitsT>
inline std::basic_ostream<CharT, TraitsT> & operator<< (std::basic_ostream<CharT, TraitsT> & os, status s)
{
    return os << to_string_view(s);
}

#undef SONIA_PRINT_TYPE_ENUM

}
