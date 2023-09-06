//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "zlib.h"

namespace sonia::zlib_detail {

inline const char * err_to_str(int err)
{
    if (Z_ERRNO == err) return "Z_ERRNO";
    if (Z_STREAM_ERROR == err) return "Z_STREAM_ERROR";
    if (Z_DATA_ERROR == err) return "Z_DATA_ERROR";
    if (Z_MEM_ERROR == err) return "Z_MEM_ERROR";
    if (Z_BUF_ERROR == err) return "Z_BUF_ERROR";
    if (Z_VERSION_ERROR == err) return "Z_VERSION_ERROR";
    return "UNKNOWN";
}

}
