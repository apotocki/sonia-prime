//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sal.hpp"

namespace sonia::sal {

std::u8string reencode_system_message(string_view message)
{
    return std::u8string((const char8_t*)message.data(), message.size());
}

}
