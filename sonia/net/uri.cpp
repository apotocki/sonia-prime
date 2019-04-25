//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/net/uri.ipp"
#include "sonia/exceptions.hpp"

namespace sonia::net {

uri_parts parse_uri(string_view str)
{
    uri_parts result;
    auto it = str.begin();
    if (!parse_uri(it, str.end(), &result)) {
        throw exception("can't parse uri, stopped at: %1%"_fmt % string_view(it, str.end()));
    }
    return result;
}

uint16_t default_scheme_port(string_view scheme)
{
    if (scheme == "http") return 80;
    if (scheme == "https") return 443;
    throw exception("unknown scheme %1%"_fmt % scheme);
}

}
