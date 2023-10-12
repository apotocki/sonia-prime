//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/cstdint.hpp"

namespace sonia::xmlbuilder {

enum class func_type : uint8_t {
    unspecified = 0,
    no_return = 2,
    is_functor = 4
};

inline func_type operator | (func_type l, func_type r)
{
    return (func_type)((uint8_t)l | (uint8_t)r);
}

inline bool operator * (func_type l, func_type r)
{
    return !!((uint8_t)l & (uint8_t)r);
}

}
