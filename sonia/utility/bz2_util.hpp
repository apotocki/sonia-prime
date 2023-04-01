//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "bzlib.h"

namespace sonia::bz2_detail {

inline const char * err_to_str(int err)
{
    if (BZ_SEQUENCE_ERROR == err) return "BZ_SEQUENCE_ERROR";
    if (BZ_PARAM_ERROR == err) return "BZ_PARAM_ERROR";
    if (BZ_MEM_ERROR == err) return "BZ_MEM_ERROR";
    if (BZ_DATA_ERROR == err) return "BZ_DATA_ERROR";
    if (BZ_DATA_ERROR_MAGIC == err) return "BZ_DATA_ERROR_MAGIC";
    if (BZ_IO_ERROR == err) return "BZ_IO_ERROR";
    if (BZ_UNEXPECTED_EOF == err) return "BZ_UNEXPECTED_EOF";
    if (BZ_OUTBUFF_FULL == err) return "BZ_OUTBUFF_FULL";
    if (BZ_CONFIG_ERROR == err) return "BZ_CONFIG_ERROR";
    return "UNKNOWN";
}

}
