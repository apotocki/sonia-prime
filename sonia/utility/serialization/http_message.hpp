//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/type_traits.hpp"
#include "serialization.hpp"
#include "sonia/net/http/message.hpp"

namespace sonia::serialization {

template <typename TagT>
class coder<TagT, http::message>
{
public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(http::message const& m, OutputIteratorT oi) const
    {
        for (auto const& [hname, hval] : m.headers) {
            http::header const* h = boost::get<http::header>(&hname);
            auto const& hnamestr = h ? to_string(*h) : boost::get<std::string>(hname);
            for (auto const& hvalitm : hval) {
                oi = std::copy(hnamestr.begin(), hnamestr.end(), std::move(oi));
                *oi = ':'; ++oi;
                *oi = ' '; ++oi;
                oi = std::copy(hvalitm.begin(), hvalitm.end(), std::move(oi));
                *oi = '\r'; ++oi;
                *oi = '\n'; ++oi;
            }
        }

        *oi = '\r'; ++oi;
        *oi = '\n'; ++oi;
        return std::move(oi);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, http::message & m) const
    {
        return decode_headers(std::move(ii), m.headers);
    }

    template <typename InputIteratorT>
    static InputIteratorT decode_headers(InputIteratorT ii, http::headers_t & headers)
    {
        using namespace sonia::http;

        char tmpbuff[64];

        for (;;) {
            char * bit = tmpbuff, * beit = tmpbuff + sizeof(tmpbuff);
        
            *bit = *ii; ++ii;
            if (*bit == '\r') {
                char c = *ii; ++ii;
                if (c == '\n') break;
                throw exception("wrong header");
            }
            if (*bit == ':') {
                throw exception("wrong header");
            }
            ++bit;
            for (;;) {
                *bit = *ii;
                ++ii;
                if (*bit == ':') break;
                ++bit;
                if (bit == beit) {
                    throw exception("header name is too long");
                }
            }
            
            std::string_view hname{tmpbuff, bit};
            header h = to_header(hname);
        
            for (char c = *ii; c == ' '; ++ii, c = *ii);
        
            std::string val;
            for (;;) {
                char c = *ii; ++ii;
                if (c == '\r') {
                    c = *ii; ++ii;
                    if (c == '\n') break;
                    val.push_back('\r');
                }
                val.push_back(c);
            }

            any_header_param_t hkey = (h == header::UNKNOWN) ? any_header_param_t(hname) : any_header_param_t(h);

            auto it = headers.find(hkey, hasher(), header_equal_to());
            if (it == headers.end()) {
                headers.insert(it, std::pair(boost::apply_visitor(header_param_converter(), hkey), std::vector{std::move(val)}));
            } else {
                it->second.push_back(std::move(val));
            }
        }

        return std::move(ii);
    }

    template <typename InputIteratorT>
    InputIteratorT decode_protocol(InputIteratorT ii, http::message & m) const
    {
        char tmpbuff[8];
        
        // parse protocol name
        char * bit = tmpbuff;
        for (char * beit = tmpbuff + sizeof(tmpbuff); bit != beit; ++bit) {
            *bit = *ii;
            ++ii;
            if (*bit == '/') break;
        }
        if (array_view(tmpbuff, bit) != array_view("HTTP", 4)) {
            throw exception("unsupported protocol: %1%"_fmt % string_view(tmpbuff, bit));
        }
        
        // parse protocol version
        m.version = 0;
        for (;; ++ii) {
            char c = *ii;
            if (c == '.') continue;
            if (c >= '0' && c <= '9') {
                m.version = m.version * 10 + (c - '0');
            } else {
                break;
            }
        }
        return std::move(ii);
    }
};

}
