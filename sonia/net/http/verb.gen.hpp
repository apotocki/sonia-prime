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

inline std::string_view to_string_view(verb value) noexcept {
    using namespace std::string_view_literals;
    switch (value) {
    case verb::UNKNOWN: return "UNKNOWN"sv;
    case verb::GET: return "GET"sv;
    case verb::HEAD: return "HEAD"sv;
    case verb::POST: return "POST"sv;
    case verb::PUT: return "PUT"sv;
    case verb::DELETE_: return "DELETE"sv;
    case verb::CONNECT: return "CONNECT"sv;
    case verb::OPTIONS: return "OPTIONS"sv;
    case verb::TRACE_: return "TRACE"sv;
    case verb::PATCH: return "PATCH"sv;
    default: return ""sv;
    }
}

inline std::ostream& operator<<(std::ostream& os, verb value) {
    return os << to_string_view(value);
}

inline bool is_valid(verb value) noexcept {
    switch (value) {
    case verb::UNKNOWN:
    case verb::GET:
    case verb::HEAD:
    case verb::POST:
    case verb::PUT:
    case verb::DELETE_:
    case verb::CONNECT:
    case verb::OPTIONS:
    case verb::TRACE_:
    case verb::PATCH:
        return true;
    default:
        return false;
    }
}

inline verb parse_verb(std::string_view str) {
    using namespace std::string_view_literals;
    std::string_view key = str;
    static constexpr std::pair<std::string_view, verb> table[] = {
        { "CONNECT"sv, verb::CONNECT },
        { "DELETE"sv, verb::DELETE_ },
        { "GET"sv, verb::GET },
        { "HEAD"sv, verb::HEAD },
        { "OPTIONS"sv, verb::OPTIONS },
        { "PATCH"sv, verb::PATCH },
        { "POST"sv, verb::POST },
        { "PUT"sv, verb::PUT },
        { "TRACE"sv, verb::TRACE_ },
        { "UNKNOWN"sv, verb::UNKNOWN },
    };
    auto it = std::lower_bound(std::begin(table), std::end(table), key,
        [](const auto& p, std::string_view s) { return p.first < s; });
    if (it != std::end(table) && it->first == key)
        return it->second;
    throw std::invalid_argument("Unknown verb value: " + std::string(str));
}

}
