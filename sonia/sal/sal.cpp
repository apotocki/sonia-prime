//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"

#include "sonia/sal.hpp"
#include "sonia/sal/net.hpp"

#include "sonia/exceptions.hpp"

namespace sonia::sal {
    
socket_address::socket_address()
{
    reset();
}

socket_address::socket_address(const void* p, size_t sz)
    : sz_(static_cast<uint8_t>(sz))
{
    if (sz >= sizeof(buffer_)) {
        THROW_INTERNAL_ERROR("can't store inet address, required size: %1%"_fmt % sz);
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

}
