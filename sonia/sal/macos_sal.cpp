//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "sonia/sal.hpp"
#include "sys/statvfs.h"

namespace sonia::sal {

void get_disk_info(u8string_view path, disk_info& di)
{
    struct statvfs s;
    if (-1 == statvfs((const char*)path.data(), &s)) {
        int err = errno;
        throw exception("get file size error : ", strerror(err));
    }

    di.total_size = (int64_t)s.f_frsize * (int64_t)s.f_blocks;
    di.free_size = (int64_t)s.f_frsize * (int64_t)s.f_bfree;
}

std::u8string reencode_system_message(string_view message)
{
    return std::u8string((const char8_t*)message.data(), message.size());
}

}
