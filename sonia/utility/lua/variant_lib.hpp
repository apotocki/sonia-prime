//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/utility/invokation/invokation.hpp"

extern "C" {

#include <lua.h>

}

namespace sonia::lua {

int luaopen_variantlib(lua_State* L);
    
blob_result* luaL_check_variant_lib(lua_State* L, int index);

int push_variant(lua_State* L, blob_result const&);

}
