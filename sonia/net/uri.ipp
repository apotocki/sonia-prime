//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_NET_URI_IPP
#define SONIA_NET_URI_IPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/net/uri.hpp"
#include "sonia/utility/parsers/utility.hpp"

namespace sonia::net {

namespace parsers {

using namespace sonia::parsers;

template <typename IteratorT>
inline bool unreserved(IteratorT & b)
{
    // unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
    return alpha(b) || digit(b) || character_from(b, string_view("-._~", 4));
}

template <typename IteratorT>
inline bool sub_delims(IteratorT & b)
{
    // sub-delims    = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
    return character_from(b, string_view("!$&'()*+,;=", 11));
}

template <typename IteratorT>
inline bool pct_encoded(IteratorT & b, IteratorT const& e)
{
    // pct-encoded   = "%" HEXDIG HEXDIG
    IteratorT pos = b;
    if (character(pos, '%') && valid(pos, e) && hexdig(pos) && valid(pos, e) && hexdig(pos)) {
        b = pos;
        return true;
    }
    return false;
}

template <typename IteratorT>
inline bool pchar(IteratorT & b, IteratorT const& e)
{
    // pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
    return unreserved(b) || pct_encoded(b, e) || sub_delims(b) || character_from(b, string_view(":@", 2));
}

template <typename IteratorT>
bool scheme(IteratorT & b, IteratorT const& e)
{
    // scheme        = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
    if (!alpha(b)) return false;
    star(b, e, [](auto & b, auto const& e) {
        return alpha(b) || digit(b) || character_from(b, string_view("+-.", 3));
    });
    return true;
}

template <typename IteratorT>
bool userinfo(IteratorT & b, IteratorT const& e)
{
    // *( unreserved / pct-encoded / sub-delims / ":" )
    return star(b, e, [](auto & b, auto const& e) {
        return unreserved(b) || pct_encoded(b, e) || sub_delims(b) || character(b, ':');
    });
}

template <typename IteratorT, class UriPpartsT>
bool ipv6address(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    // dummy
    return plus(b, e, [](auto & b, auto const& e) {
        return hexdig(b) || character(b, ':');
    });
}

template <typename IteratorT>
bool ipvfuture(IteratorT & b, IteratorT const& e)
{
    // IPvFuture     = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
    IteratorT pos = b;
    if (!character(pos, 'v')) return false;
    if (!plus(pos, e, [](auto & b, auto const& e) { return hexdig(b); })) return false;
    if (!character(pos, '.')) return false;
    if (!plus(pos, e, [](auto & b, auto const& e) { return unreserved(b) || sub_delims(b) || character(b, ':'); })) return false;
    b = pos;
    return true;
}

template <typename IteratorT, class UriPpartsT>
bool ip_literal(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    // IP-literal    = "[" ( IPv6address / IPvFuture  ) "]"
    IteratorT pos = b;
    if (character(pos, '[') && valid(pos, e) && (ipv6address(pos, e, result) || ipvfuture(pos, e)) && valid(pos, e) && character(pos, ']')) {
        if (result) result->host = {b + 1, pos - 1};
        b = pos;
        return true;
    }
    return false;
}

template <typename IteratorT, class UriPpartsT>
bool ipv4address(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    // dummy
    return false;
}

template <typename IteratorT, class UriPpartsT>
bool reg_name(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    // reg-name      = *( unreserved / pct-encoded / sub-delims )
    IteratorT host_b = b;
    star(b, e, [](auto & b, auto const& e) {
        return unreserved(b) || pct_encoded(b, e) || sub_delims(b);
    });
    if (result) result->host = {host_b, b};
    return true;
}

template <typename IteratorT>
bool segment(IteratorT & b, IteratorT const& e)
{
    // segment       = *pchar
    return star(b, e, [](auto & b, auto const& e) { return pchar(b, e); });
}

template <typename IteratorT>
bool segments(IteratorT & b, IteratorT const& e)
{
    // segments = *( "/" segment )
    return star(b, e, [](auto & b, auto const& e) {
        auto pos = b;
        if (!character(pos, '/')) return false;
        if (valid(pos, e) && !segment(pos, e)) return false;
        b = pos;
        return true;
    });
}

template <typename IteratorT, class UriPpartsT>
bool host(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    // host          = IP-literal / IPv4address / reg-name
    return ip_literal(b, e, result) || ipv4address(b, e, result) || reg_name(b, e, result);
}

template <typename IteratorT, class UriPpartsT>
bool authority(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    // authority     = [ userinfo "@" ] host [ ":" port ]
    IteratorT auth_b = b, pos = b;
    if (userinfo(pos, e) && valid(pos, e) && character(pos, '@')) {
        if (result) result->userinfo = {auth_b, pos - 1};
    } else {
        pos = b;
    }
    if (!valid(pos, e) || !host(pos, e, result)) return false;
    b = pos;
    uint16_t port = 0;
    if (valid(b, e) && character(b, ':')) {
        integer(b, e, 0, 5, port);
        if (result) result->port = port;
    }
    if (result) result->authority = {auth_b, b};
    return true;
}

template <typename IteratorT, class UriPpartsT>
bool path_abempty(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    // path-abempty  = *( "/" segment )
    IteratorT pos = b;
    segments(b, e); // always return true
    if (result) result->path = {pos, b};
    return true;
}

template <typename IteratorT, class UriPpartsT>
bool path_absolute(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    // path-absolute = "/" [ segment-nz *( "/" segment ) ]
    // segment-nz    = 1*pchar
    IteratorT pos = b;
    if (!character(b, '/')) return false;
    if (valid(b, e) && plus(b, e, [](auto & b, auto const& e) { return pchar(b, e); })) {
        segments(b, e);
    }
    if (result) result->path = {pos, b};
    return true;
}

template <typename IteratorT, class UriPpartsT>
bool path_rootless(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    // path-rootless = segment-nz *( "/" segment )
    // segment-nz    = 1*pchar
    IteratorT pos = b;
    if (plus(b, e, [](auto & b, auto const& e) { return pchar(b, e); })) {
        segments(b, e);
        if (result) result->path = {pos, b};
        return true;
    }
    return false;
}

template <typename IteratorT, class UriPpartsT>
bool path_noscheme(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    // path-noscheme = segment-nz-nc *( "/" segment )
    // segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" ) ; non-zero-length segment without any colon ":"
   
    IteratorT pos = b;
    if (plus(pos, e, [](auto & b, auto const& e){ return unreserved(b) || pct_encoded(b, e) || sub_delims(b) || character(b, '@'); })) {
        segments(pos, e);
        if (result) result->path = {b, pos};
        b = pos;
        return true;
    }
    return false;
}

template <typename IteratorT, class UriPpartsT>
bool hier_part(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    /*
        hier-part = "//" authority path-abempty
                 / path-absolute
                 / path-rootless
                 / path-empty
    
    */
    IteratorT pos = b;
    if (string(pos, e, string_view("//", 2)) && valid(pos, e) && authority(pos, e, result) && valid(pos, e) && path_abempty(pos, e, result)) {
        b = pos;
        return true;
    }
    if (!path_absolute(b, e, result)) {
        path_rootless(b, e, result);
    }
    return true;
}

template <typename IteratorT, class UriPpartsT>
bool relative_part(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    /*
        relative-part = "//" authority path-abempty
                 / path-absolute
                 / path-noscheme
                 / path-empty
    */
    IteratorT pos = b;
    if (string(pos, e, string_view("//", 2)) && valid(pos, e) && authority(pos, e, result) && valid(pos, e) && path_abempty(pos, e, result)) {
        b = pos;
        return true;
    }
    if (!path_absolute(b, e, result)) {
        path_noscheme(b, e, result);
    }
    return true;
}

template <typename IteratorT, class UriPpartsT>
bool query(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    IteratorT pos = b;
    star(b, e, [](auto & b, auto const& e) {
        auto pos = b;
        if (!pchar(pos, e) && !character(pos, '/') && !character(pos, '?')) return false;
        b = pos;
        return true;
    });
    if (result) result->query = {pos, b};
    return true;
}

template <typename IteratorT, class UriPpartsT>
bool fragment(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    IteratorT pos = b;
    star(b, e, [](auto & b, auto const& e) {
        auto pos = b;
        if (!pchar(pos, e) && !character(pos, '/') && !character(pos, '?')) return false;
        b = pos;
        return true;
    });
    if (result) result->fragment = {pos, b};
    return true;
}

template <typename IteratorT, class UriPpartsT>
bool uri(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    // URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

    IteratorT pos = b;
    if (!valid(b, e) || !parsers::scheme(b, e)) return false;
    if (result) result->scheme = {pos, b};
    if (!valid(b, e) || !character(b, ':')) return false;
    if (!valid(b, e) || !hier_part(b, e, result)) return false;
    if (valid(b, e) && character(b, '?')) {
        query(b, e, result);
    }
    if (valid(b, e) && character(b, '#')) {
        fragment(b, e, result);
    }
    if (result) result->is_absolute = true;
    return true;
}

template <typename IteratorT, class UriPpartsT>
bool relative_ref(IteratorT & b, IteratorT const& e, UriPpartsT* result)
{
    // relative-ref  = relative-part [ "?" query ] [ "#" fragment ]
    if (!relative_part(b, e, result)) return false;
    if (valid(b, e) && character(b, '?')) {
        query(b, e, result);
    }
    if (valid(b, e) && character(b, '#')) {
        fragment(b, e, result);
    }
    if (result) {
        result->scheme.reset();
        result->is_absolute = false;
    }
    return true;
}

}

template <typename IteratorT, class UriPpartsT>
bool parse_uri(IteratorT & b, IteratorT e, UriPpartsT* result)
{
    using namespace sonia::net::parsers;

    // URI-reference = URI / relative-ref
    return uri(b, e, result) || relative_ref(b, e, result);
}

template <typename OutputIteratorT>
void canonize_path(string_view path, OutputIteratorT oit)
{
    const char* uriit = path.begin(), *urieit = path.end();
    std::vector<string_view> parts;

    const char* part_begin = uriit;
    while (uriit != urieit) {
        if (*uriit == '/' || *uriit == '\\') {
            if (part_begin != uriit) {
                parts.push_back(string_view(part_begin, uriit - part_begin));
            }
            ++uriit;
            part_begin = uriit;
            continue;
        }
        if (uriit == part_begin && *uriit == '.') {
            ++uriit;
            if (uriit == urieit) break;
            if (*uriit == '/' || *uriit == '\\') {
                ++uriit;
                part_begin = uriit;
                continue;
            }
            if (*uriit != '.') {
                ++uriit;
                continue;
            }
            ++uriit;
            if (uriit == urieit) break;
            if (*uriit != '/' && *uriit != '\\') {
                ++uriit;
                continue;
            }
            if (!parts.empty()) {
                parts.pop_back();
            }
            ++uriit;
            part_begin = uriit;
            continue;
        }
        ++uriit;
    }
    if (part_begin != uriit) {
        parts.push_back(string_view(part_begin, uriit - part_begin));
    }

    if (!path.empty() && (path[0] == '/' || path[0] == '\\')) {
        *oit = '/';
        ++oit;
    }

    auto it = parts.begin(), eit = parts.end();
    if (it != eit) for (;;) {
        oit = std::copy(it->begin(), it->end(), oit);
        ++it;
        if (it == eit) break;
        *oit = '/';
        ++oit;
    }
    if (path.size() > 1 && !parts.empty() && (path.back() == '/' || path.back() == '\\')) {
        *oit = '/';
        ++oit;
    }
}

template <typename InputIteratorT, typename OutputIteratorT>
void decode_uri_component(InputIteratorT b, InputIteratorT e, OutputIteratorT oit)
{
    for (; b != e; ++b, ++oit) {
        char c = *b;
        if (c == '%') {
            ++b;
            uint8_t r;
            if (!parsers::hexinteger(b, e, 2, 2, r)) throw exception("wrong urlencoded sequence");
            *oit = r;
        } else if (c == '+') {
            *oit = ' ';
        } else {
            *oit = c;
        }
    }
}

template <typename InputIteratorT, typename FunctorT>
void decode_query(InputIteratorT b, InputIteratorT e, FunctorT const& ftor)
{
    for (;;) {
        InputIteratorT bnm = b;
        for (; b != e && *b != '='; ++b);
        if (b == e) return;
        InputIteratorT enm = b; ++b;
        InputIteratorT bv = b;
        for (; b != e && *b != '&'; ++b);
        std::string nm, v;
        decode_uri_component(bnm, enm, std::back_inserter(nm));
        decode_uri_component(bv, b, std::back_inserter(v));
        ftor(std::move(nm), std::move(v));
        if (b == e) return;
        ++b;
    }
}

}

#endif // SONIA_NET_URI_IPP
