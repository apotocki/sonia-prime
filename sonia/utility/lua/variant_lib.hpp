//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

extern "C" {

#include <lua.h>

}

#include "lua.hpp"

namespace sonia::lua {

int luaopen_variantlib(lua_State*);

void setup_ext(lua_State*, language*);

blob_result* luaL_check_variant_lib(lua_State*, int index);

void push_from_blob(lua_State*, blob_result const&);
blob_result to_blob(lua_State*, int index);

int push_variant(lua_State*, blob_result const&);

}
