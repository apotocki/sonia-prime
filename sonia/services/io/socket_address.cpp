//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#ifdef BOOST_WINDOWS
#   include "sonia/utility/windows.hpp"
namespace winapi = sonia::windows;
#else
#   include <netinet/in.h>
#   include "sonia/utility/linux.hpp"
namespace linapi = sonia::linux;
#endif

#include "socket_address.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/phoenix/stl/container.hpp>
#include <boost/phoenix/object/construct.hpp>

#include "sonia/exceptions/internal_errors.hpp"

namespace sonia { namespace io {

#define SONIA_PRINT_IO_PROTOCOL_TYPE_CASE(r, data, i, elem) \
    if (lcasestr == BOOST_STRINGIZE(BOOST_PP_TUPLE_ELEM(2, 1, elem))) { pt = protocol_type::BOOST_PP_TUPLE_ELEM(2, 0, elem); } else 

std::tuple<protocol_type, std::string, uint16_t> parse_address(string_view sv)
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;

    std::vector<char> protocol_str;
    std::vector<char> address_str;
    uint16_t port;

    auto it = sv.begin();
    if (bool r = qi::phrase_parse(it, sv.end(),
          qi::lexeme[+(qi::char_ - ':')][phoenix::ref(protocol_str) = qi::_1] >> "://" >>
          ((
            qi::lexeme[+(qi::char_ - ':')][phoenix::ref(address_str) = qi::_1]
          ) ||
          (
            '[' >> qi::lexeme[+(qi::char_ - ']')][phoenix::ref(address_str) = qi::_1] >> ']'
          )) >> ':' >>
          qi::ushort_[phoenix::ref(port) = qi::_1]
        , ascii::space); !r || it != sv.end())
    {
        throw exception("can't parse address string: '%1%', sopped at: '%2%'"_fmt % sv % string_view(it, sv.end()));
    }

    for (char & c : protocol_str) {
        if (c >= 'A' && c <= 'Z') c += ('a' - 'A');
    }

    protocol_type pt = protocol_type::UNKNOWN;
    string_view lcasestr = to_string_view(protocol_str);
    BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_IO_PROTOCOL_TYPE_CASE, _, SONIA_IO_PROTOCOL_TYPE_SEQ)
    {
        throw exception("unknown protocol name: '%1%'"_fmt % lcasestr);
    }

    return {pt, to_string(address_str), port};
}

}}
