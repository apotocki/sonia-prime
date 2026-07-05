//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

// Auto-generated file. Do not edit manually.
#pragma once


#include <string_view>
#include <stdexcept>
#include <ostream>
#include <algorithm>
#include <utility>

namespace sonia::http {

inline std::string_view to_string_view(status value) noexcept {
    using namespace std::string_view_literals;
    switch (value) {
    case status::CONTINUE: return "Continue"sv;
    case status::SWITCHING_PROTOCOLS: return "Switching Protocols"sv;
    case status::OK: return "OK"sv;
    case status::CREATED: return "Created"sv;
    case status::ACCEPTED: return "Accepted"sv;
    case status::NON_AUTHORIRATIVE_INFORMATION: return "Non-Authoritative Information"sv;
    case status::NO_CONTENT: return "No Content"sv;
    case status::RESET_CONTENT: return "Reset Content"sv;
    case status::PARTIAL_CONTENT: return "Partial Content"sv;
    case status::MULTIPLE_CHOICES: return "Multiple Choices"sv;
    case status::MOVED_PERMANENTLY: return "Moved Permanently"sv;
    case status::FOUND: return "Moved Temporarily"sv;
    case status::SEE_OTHER: return "See Other"sv;
    case status::NOT_MODIFIED: return "Not Modified"sv;
    case status::USE_PROXY: return "Use Proxy"sv;
    case status::TEMPORARY_REDIRECT: return "Temporary Redirect"sv;
    case status::BAD_REQUEST: return "Bad Request"sv;
    case status::UNAUTHORIZED: return "Unauthorized"sv;
    case status::PAYMENT_REQUIRED: return "Payment Required"sv;
    case status::FORBIDDEN: return "Forbidden"sv;
    case status::NOT_FOUND: return "Not Found"sv;
    case status::METHOD_NOT_ALLOWED: return "Method Not Allowed"sv;
    case status::NOT_ACCEPTABLE: return "Not Acceptable"sv;
    case status::PROXY_AUTHENTICATION_REQUIRED: return "Proxy Authentication Required"sv;
    case status::REQUEST_TIMEOUT: return "Request Timeout"sv;
    case status::CONFLICT: return "Conflict"sv;
    case status::GONE: return "Gone"sv;
    case status::LENGTH_REQUIRED: return "Length Required"sv;
    case status::PRECONDITION_FAILED: return "Precondition Failed"sv;
    case status::REQUEST_ENTITY_TOO_LARGE: return "Request Entity Too Large"sv;
    case status::REQUEST_URI_TOO_LONG: return "Request-URI Too Long"sv;
    case status::UNSUPPORTED_MEDIA_TYPE: return "Unsupported Media Type"sv;
    case status::REQUESTED_RANGE_NOT_SATISFIABLE: return "Requested Range Not Satisfiable"sv;
    case status::EXPECTATION_FAILED: return "Expectation Failed"sv;
    case status::INTERNAL_SERVER_ERROR: return "Internal Server Error"sv;
    case status::NOT_IMPLEMENTED: return "Not Implemented"sv;
    case status::BAD_GATEWAY: return "Bad Gateway"sv;
    case status::SERVICE_UNAVAILABLE: return "Service Unavailable"sv;
    case status::GATEWAY_TIMEOUT: return "Gateway Timeout"sv;
    case status::HTTP_VERSION_NOT_SUPPORTED: return "HTTP Version Not Supported"sv;
    case status::CONNECTION_TIMED_OUT: return "Connection Timed Out"sv;
    case status::A_TIMEOUT_OCCURRED: return "A Timeout Occurred"sv;
    default: return ""sv;
    }
}

inline std::ostream& operator<<(std::ostream& os, status value) {
    return os << to_string_view(value);
}

inline bool is_valid(status value) noexcept {
    switch (value) {
    case status::CONTINUE:
    case status::SWITCHING_PROTOCOLS:
    case status::OK:
    case status::CREATED:
    case status::ACCEPTED:
    case status::NON_AUTHORIRATIVE_INFORMATION:
    case status::NO_CONTENT:
    case status::RESET_CONTENT:
    case status::PARTIAL_CONTENT:
    case status::MULTIPLE_CHOICES:
    case status::MOVED_PERMANENTLY:
    case status::FOUND:
    case status::SEE_OTHER:
    case status::NOT_MODIFIED:
    case status::USE_PROXY:
    case status::TEMPORARY_REDIRECT:
    case status::BAD_REQUEST:
    case status::UNAUTHORIZED:
    case status::PAYMENT_REQUIRED:
    case status::FORBIDDEN:
    case status::NOT_FOUND:
    case status::METHOD_NOT_ALLOWED:
    case status::NOT_ACCEPTABLE:
    case status::PROXY_AUTHENTICATION_REQUIRED:
    case status::REQUEST_TIMEOUT:
    case status::CONFLICT:
    case status::GONE:
    case status::LENGTH_REQUIRED:
    case status::PRECONDITION_FAILED:
    case status::REQUEST_ENTITY_TOO_LARGE:
    case status::REQUEST_URI_TOO_LONG:
    case status::UNSUPPORTED_MEDIA_TYPE:
    case status::REQUESTED_RANGE_NOT_SATISFIABLE:
    case status::EXPECTATION_FAILED:
    case status::INTERNAL_SERVER_ERROR:
    case status::NOT_IMPLEMENTED:
    case status::BAD_GATEWAY:
    case status::SERVICE_UNAVAILABLE:
    case status::GATEWAY_TIMEOUT:
    case status::HTTP_VERSION_NOT_SUPPORTED:
    case status::CONNECTION_TIMED_OUT:
    case status::A_TIMEOUT_OCCURRED:
        return true;
    default:
        return false;
    }
}

inline status parse_status(std::string_view str) {
    using namespace std::string_view_literals;
    std::string_view key = str;
    static constexpr std::pair<std::string_view, status> table[] = {
        { "A Timeout Occurred"sv, status::A_TIMEOUT_OCCURRED },
        { "Accepted"sv, status::ACCEPTED },
        { "Bad Gateway"sv, status::BAD_GATEWAY },
        { "Bad Request"sv, status::BAD_REQUEST },
        { "Conflict"sv, status::CONFLICT },
        { "Connection Timed Out"sv, status::CONNECTION_TIMED_OUT },
        { "Continue"sv, status::CONTINUE },
        { "Created"sv, status::CREATED },
        { "Expectation Failed"sv, status::EXPECTATION_FAILED },
        { "Forbidden"sv, status::FORBIDDEN },
        { "Gateway Timeout"sv, status::GATEWAY_TIMEOUT },
        { "Gone"sv, status::GONE },
        { "HTTP Version Not Supported"sv, status::HTTP_VERSION_NOT_SUPPORTED },
        { "Internal Server Error"sv, status::INTERNAL_SERVER_ERROR },
        { "Length Required"sv, status::LENGTH_REQUIRED },
        { "Method Not Allowed"sv, status::METHOD_NOT_ALLOWED },
        { "Moved Permanently"sv, status::MOVED_PERMANENTLY },
        { "Moved Temporarily"sv, status::FOUND },
        { "Multiple Choices"sv, status::MULTIPLE_CHOICES },
        { "No Content"sv, status::NO_CONTENT },
        { "Non-Authoritative Information"sv, status::NON_AUTHORIRATIVE_INFORMATION },
        { "Not Acceptable"sv, status::NOT_ACCEPTABLE },
        { "Not Found"sv, status::NOT_FOUND },
        { "Not Implemented"sv, status::NOT_IMPLEMENTED },
        { "Not Modified"sv, status::NOT_MODIFIED },
        { "OK"sv, status::OK },
        { "Partial Content"sv, status::PARTIAL_CONTENT },
        { "Payment Required"sv, status::PAYMENT_REQUIRED },
        { "Precondition Failed"sv, status::PRECONDITION_FAILED },
        { "Proxy Authentication Required"sv, status::PROXY_AUTHENTICATION_REQUIRED },
        { "Request Entity Too Large"sv, status::REQUEST_ENTITY_TOO_LARGE },
        { "Request Timeout"sv, status::REQUEST_TIMEOUT },
        { "Request-URI Too Long"sv, status::REQUEST_URI_TOO_LONG },
        { "Requested Range Not Satisfiable"sv, status::REQUESTED_RANGE_NOT_SATISFIABLE },
        { "Reset Content"sv, status::RESET_CONTENT },
        { "See Other"sv, status::SEE_OTHER },
        { "Service Unavailable"sv, status::SERVICE_UNAVAILABLE },
        { "Switching Protocols"sv, status::SWITCHING_PROTOCOLS },
        { "Temporary Redirect"sv, status::TEMPORARY_REDIRECT },
        { "Unauthorized"sv, status::UNAUTHORIZED },
        { "Unsupported Media Type"sv, status::UNSUPPORTED_MEDIA_TYPE },
        { "Use Proxy"sv, status::USE_PROXY },
    };
    auto it = std::lower_bound(std::begin(table), std::end(table), key,
        [](const auto& p, std::string_view s) { return p.first < s; });
    if (it != std::end(table) && it->first == key)
        return it->second;
    throw std::invalid_argument("Unknown status value: " + std::string(str));
}

}
