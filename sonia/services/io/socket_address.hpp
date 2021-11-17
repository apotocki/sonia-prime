//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <tuple>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"
#include "sockets.hpp"

#include "sonia/sal/net.hpp"

namespace sonia::io {

enum class socket_type : uint8_t
{
    TCP,
    TCP6,
    UDP,
    UDP6
};

#define SONIA_IO_PROTOCOL_TYPE_SEQ  \
    ((TCP, tcp))                    \
    ((TCP6, tcp6))                  \
    ((UDP, udp))                    \
    ((UDP6, udp6))                  \
    ((SSL, ssl))                    \

#define SONIA_PRINT_IO_PROTOCOL_TYPE_ENUM(r, data, i, elem) BOOST_PP_TUPLE_ELEM(2, 0, elem), 

enum class protocol_type : uint8_t
{
    BOOST_PP_SEQ_FOR_EACH_I(SONIA_PRINT_IO_PROTOCOL_TYPE_ENUM, _, SONIA_IO_PROTOCOL_TYPE_SEQ)
    UNKNOWN
};

/**
 *  examples: tcp://localhost:8080 udp6://[fe80::156a:c70:636e:4bc1]:49000
 */
std::tuple<protocol_type, std::string, uint16_t> parse_address(string_view);

using sonia::sal::socket_address;

//class socket_addresses
//{
//public:
//    explicit socket_address(std::pair<const char*, size_t> iaddr);
//    socket_address(cstring_view, uint16_t);
//    
//    std::pair<const char*, size_t> in_addr() const;
//    cstring_view c_str() const;
//    uint16_t port() const { return port_; }
//
//private:
//    mutable shared_optimized_array<char, 8, uint16_t> addr_;
//    mutable shared_optimized_array<char, 16, uint16_t> str_;
//    uint16_t port_;
//};

}
