//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

extern "C" {

#include <lua.h>

}

#include "lua.hpp"

namespace sonia::lua {

int luaopen_bigintlib(lua_State*);

struct biginit_header;

biginit_header* luaL_check_bigint_lib(lua_State*, int index);
biginit_header* luaL_test_bigint_lib(lua_State* L, int index);

}
