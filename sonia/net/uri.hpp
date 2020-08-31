//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_NET_URI_HPP
#define SONIA_NET_URI_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "sonia/string.hpp"
#include "sonia/optional.hpp"

namespace sonia::net {

// based on rfc3986 with updates: rfc6874 and rfc7320

struct uri_parts
{
    string_view scheme;
    string_view authority;
        string_view userinfo;
        string_view host;
        optional<uint16_t> port;
    string_view path;
    string_view query;
    string_view fragment;
    bool is_absolute;
};

uri_parts parse_uri(string_view);

template <typename InputIteratorT, class UriPpartsT>
bool parse_uri(InputIteratorT & b, InputIteratorT e, UriPpartsT* result = nullptr);

template <typename OutputIteratorT>
void canonize_path(string_view path, OutputIteratorT oit);

template <typename InputIteratorT, typename OutputIteratorT>
void decode_uri_component(InputIteratorT b, InputIteratorT e, OutputIteratorT oit);

template <typename InputIteratorT, typename FunctorT>
void decode_query(InputIteratorT b, InputIteratorT e, FunctorT const& ftor);

template <typename InputIteratorT, typename OutputIteratorT>
void encode_uri_component(InputIteratorT b, InputIteratorT e, OutputIteratorT oit);

uint16_t default_scheme_port(string_view);

}

#endif // SONIA_NET_URI_HPP
