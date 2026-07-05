//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/config.hpp"
#include "sonia/enum_proto.hpp"
#include "sonia/string.hpp"

namespace sonia::http {

DEFINE_ENUM_PROTO_EX(status, uint16_t, "ostream",
    CONTINUE                        ENUM_VALUE_STR("Continue")                        = 100,
    SWITCHING_PROTOCOLS             ENUM_VALUE_STR("Switching Protocols")             = 101,
    OK                              ENUM_VALUE_STR("OK")                              = 200,
    CREATED                         ENUM_VALUE_STR("Created")                         = 201,
    ACCEPTED                        ENUM_VALUE_STR("Accepted")                        = 202,
    NON_AUTHORIRATIVE_INFORMATION   ENUM_VALUE_STR("Non-Authoritative Information")   = 203,
    NO_CONTENT                      ENUM_VALUE_STR("No Content")                      = 204,
    RESET_CONTENT                   ENUM_VALUE_STR("Reset Content")                   = 205,
    PARTIAL_CONTENT                 ENUM_VALUE_STR("Partial Content")                 = 206,
    MULTIPLE_CHOICES                ENUM_VALUE_STR("Multiple Choices")                = 300,
    MOVED_PERMANENTLY               ENUM_VALUE_STR("Moved Permanently")               = 301,
    FOUND                           ENUM_VALUE_STR("Moved Temporarily")               = 302,
    SEE_OTHER                       ENUM_VALUE_STR("See Other")                       = 303,
    NOT_MODIFIED                    ENUM_VALUE_STR("Not Modified")                    = 304,
    USE_PROXY                       ENUM_VALUE_STR("Use Proxy")                       = 305,
    TEMPORARY_REDIRECT              ENUM_VALUE_STR("Temporary Redirect")              = 307,
    BAD_REQUEST                     ENUM_VALUE_STR("Bad Request")                     = 400,
    UNAUTHORIZED                    ENUM_VALUE_STR("Unauthorized")                    = 401,
    PAYMENT_REQUIRED                ENUM_VALUE_STR("Payment Required")                = 402,
    FORBIDDEN                       ENUM_VALUE_STR("Forbidden")                       = 403,
    NOT_FOUND                       ENUM_VALUE_STR("Not Found")                       = 404,
    METHOD_NOT_ALLOWED              ENUM_VALUE_STR("Method Not Allowed")              = 405,
    NOT_ACCEPTABLE                  ENUM_VALUE_STR("Not Acceptable")                  = 406,
    PROXY_AUTHENTICATION_REQUIRED   ENUM_VALUE_STR("Proxy Authentication Required")   = 407,
    REQUEST_TIMEOUT                 ENUM_VALUE_STR("Request Timeout")                 = 408,
    CONFLICT                        ENUM_VALUE_STR("Conflict")                        = 409,
    GONE                            ENUM_VALUE_STR("Gone")                            = 410,
    LENGTH_REQUIRED                 ENUM_VALUE_STR("Length Required")                 = 411,
    PRECONDITION_FAILED             ENUM_VALUE_STR("Precondition Failed")             = 412,
    REQUEST_ENTITY_TOO_LARGE        ENUM_VALUE_STR("Request Entity Too Large")        = 413,
    REQUEST_URI_TOO_LONG            ENUM_VALUE_STR("Request-URI Too Long")            = 414,
    UNSUPPORTED_MEDIA_TYPE          ENUM_VALUE_STR("Unsupported Media Type")          = 415,
    REQUESTED_RANGE_NOT_SATISFIABLE ENUM_VALUE_STR("Requested Range Not Satisfiable") = 416,
    EXPECTATION_FAILED              ENUM_VALUE_STR("Expectation Failed")              = 417,
    INTERNAL_SERVER_ERROR           ENUM_VALUE_STR("Internal Server Error")           = 500,
    NOT_IMPLEMENTED                 ENUM_VALUE_STR("Not Implemented")                 = 501,
    BAD_GATEWAY                     ENUM_VALUE_STR("Bad Gateway")                     = 502,
    SERVICE_UNAVAILABLE             ENUM_VALUE_STR("Service Unavailable")             = 503,
    GATEWAY_TIMEOUT                 ENUM_VALUE_STR("Gateway Timeout")                 = 504,
    HTTP_VERSION_NOT_SUPPORTED      ENUM_VALUE_STR("HTTP Version Not Supported")      = 505,
    CONNECTION_TIMED_OUT            ENUM_VALUE_STR("Connection Timed Out")            = 522,
    A_TIMEOUT_OCCURRED              ENUM_VALUE_STR("A Timeout Occurred")              = 524)

}

#ifndef SONIA_DO_NOT_INCLUDE_GEN
#   include "status_codes.gen.hpp"
#endif
