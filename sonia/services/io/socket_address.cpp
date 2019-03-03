//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "socket_address.hpp"

#include <boost/throw_exception.hpp>
#include "sonia/exceptions.hpp"

#ifdef BOOST_WINDOWS
#   include "sonia/utility/windows.hpp"
namespace winapi = sonia::windows;
#else
#   include <netinet/in.h>
#   include "sonia/utility/linux.hpp"
namespace linapi = sonia::linux;
#endif

namespace sonia { namespace io {

socket_address::socket_address()
{
    reset();
}

socket_address::socket_address(const void* p, size_t sz)
    : sz_(static_cast<uint8_t>(sz))
{
    if (sz >= sizeof(buffer_)) {
        BOOST_THROW_EXCEPTION(internal_error("can't store inet address, required size: %1%"_fmt % sz));
    }
    std::memcpy(buffer_, p, sz);
}

void socket_address::reset()
{
    sz_ = sizeof(buffer_);
    memset(buffer_, 0, sz_);
}

std::pair<const char*, size_t> socket_address::in_addr() const
{
    return std::pair(buffer_, sz_);
}

std::string socket_address::str() const
{
#ifdef BOOST_WINDOWS
    return winapi::inet_ntoa(reinterpret_cast<sockaddr const*>(buffer_), (DWORD)sz_, NULL);
#else
    return linapi::inet_ntoa(reinterpret_cast<sockaddr_in const*>(buffer_));
#endif
}

uint16_t socket_address::port() const
{
    return reinterpret_cast<sockaddr_in const*>(buffer_)->sin_port;
}

}}
