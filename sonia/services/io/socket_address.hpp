//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_IO_SOCKET_ADDRESS_HPP
#define SONIA_IO_SOCKET_ADDRESS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <vector>
#include "sonia/cstdint.hpp"
#include "sonia/string.hpp"

namespace sonia { namespace io {

class socket_address
{
public:
    socket_address();
    socket_address(const void*, size_t);

    void reset();
    
    std::string str() const;
    uint16_t port() const;

    std::pair<const char*, size_t> in_addr() const;
    template <typename T> T * addr() { return (T*)buffer_; }
    uint8_t size() const { return sz_; }

private:
    alignas(void*) char buffer_[31]; // to be able to store sockaddr_in6
    uint8_t sz_;
};

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

}}

#endif // SONIA_IO_SOCKET_ADDRESS_HPP
