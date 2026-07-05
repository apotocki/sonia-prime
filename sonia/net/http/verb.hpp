//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/config.hpp"
#include "sonia/enum_proto.hpp"
#include "sonia/string.hpp"

namespace sonia::http {

DEFINE_ENUM_PROTO_EX(verb, uint8_t, "ostream",
    UNKNOWN = 0,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE_ ENUM_VALUE_STR("DELETE"),
    CONNECT,
    OPTIONS,
    TRACE_ ENUM_VALUE_STR("TRACE"),
    PATCH,
    UPPER_BOUND_VALUE ENUM_VALUE_SKIP)

}

#ifndef SONIA_DO_NOT_INCLUDE_GEN
#   include "verb.gen.hpp"
#endif
