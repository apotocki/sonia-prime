//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/type_traits.hpp"
#include "serialization.hpp"
#include "http_message.hpp"

namespace sonia::serialization {

template <typename TagT>
class coder<TagT, http::request> : coder<TagT, http::message>
{
    using base_type = coder<TagT, http::message>;

public:
    template <typename OutputIteratorT>
    OutputIteratorT encode(http::request const& r, OutputIteratorT oi) const
    {
        std::string hello_line = to_string("%1% %2% HTTP/%3%.%4%\r\n"_fmt % to_string(r.method) % r.get_relative_uri() % (r.version / 10) % (r.version % 10));
        oi = std::copy(hello_line.begin(), hello_line.end(), std::move(oi));

        oi = base_type::encode(r, std::move(oi));
        
        return std::move(oi);
    }

    template <typename InputIteratorT>
    InputIteratorT decode(InputIteratorT ii, http::request & req) const
    {
        char tmpbuff[8];
        
        // parse verb
        char * bit = tmpbuff, * beit = tmpbuff + sizeof(tmpbuff);
        for (; bit != beit; ++bit) {
            *bit = *ii;
            ++ii;
            if (*bit == ' ') break;
        }
        req.method = http::to_method(string_view(tmpbuff, bit));
        
        if (http::method_verb::UNKNOWN == req.method) {
            throw exception("unknown verb: %1%"_fmt % string_view(tmpbuff, bit));
        }

        // get url
        std::string url;
        for (;;) {
            char c = *ii; ++ii;
            if (c == ' ') break;
            url.push_back(c);
        }
        req.set_uri(url, true); // ignore abs part
        
        ii = base_type::decode_protocol(std::move(ii), req);
        char c = *ii; ++ii;
        char c1 = *ii; ++ii;
        if (c != '\r' || c1 != '\n') {
            throw exception("wrong protocol version");
        }
        
        // decode headers
        ii = base_type::decode(std::move(ii), req);
        if (!req.keep_alive) {
            auto hvals = req.get_header(http::header::CONNECTION);
            if (req.version == 10) {
                req.keep_alive = false;
            } else {
                req.keep_alive = !(hvals.size() == 1 && boost::iequals(hvals[0], "close"));
            }
        }
        return std::move(ii);
    }
};

}
