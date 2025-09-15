//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/cstdint.hpp"

extern "C" {

#include <lua.h>

}

#include <numetron/integer_view.hpp>

namespace sonia::lua {

int luaopen_bigintlib(lua_State*);

struct bigint_header
{
    uint64_t size : 63;
    uint64_t sign : 1; // 0 - positive, 1 - negative
};

bigint_header* luaL_check_bigint_lib(lua_State*, int index);
bigint_header* luaL_test_bigint_lib(lua_State*, int index);

using limb_type = uint64_t;
constexpr size_t limbs_per_lua_integer = sizeof(lua_Integer) / sizeof(limb_type);
//using integer_type = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<65, 0>>;

int push_bigint(lua_State*, uint64_t value);
int push_bigint(lua_State*, numetron::basic_integer_view<limb_type> value);
numetron::basic_integer_view<limb_type> restore_bigint(bigint_header * bh);

}
