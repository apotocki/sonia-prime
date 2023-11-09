//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "variant_lib.hpp"
#include "sonia/utility/invokation/invokation.hpp"
#include <sstream>

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h> 

}

#define VARIANT_METATABLE_NAME "sonia.variant"
#define VARIANT_LIB_NAME "variant"

namespace sonia::lua {

blob_result* luaL_check_variant_lib(lua_State* L, int index)
{
    return (blob_result*)luaL_checkudata(L, index, VARIANT_METATABLE_NAME);
}

int variant_gc(lua_State* L)
{
    blob_result* br = (blob_result*)lua_touserdata(L, 1);
    if (br) {
        blob_result_unpin(br);
    }
    return 0;
}

int push_variant(lua_State* L, blob_result const& value)
{
    blob_result* br = (blob_result*)lua_newuserdata(L, sizeof(blob_result));
    *br = value;
    blob_result_pin(br);

    luaL_getmetatable(L, VARIANT_METATABLE_NAME);
    lua_setmetatable(L, -2);

    return 1;
}

int variant_at_index(lua_State* L)
{
    blob_result* br = luaL_check_variant_lib(L, 1);
    luaL_argcheck(L, !!br, 1, "`variant' expected");
}

int variant_tostring(lua_State* L)
{
    blob_result* br = luaL_check_variant_lib(L, 1);
    luaL_argcheck(L, !!br, 1, "`variant' expected");
    std::ostringstream s;
    s << *br;
    std::string result = s.str();
    lua_pushfstring(L, "variant(%s)", result.c_str());
    return 1;
}

const struct luaL_Reg variantlib[] = {
    //{"new", newvariant},
    {NULL, NULL}
};

const struct luaL_Reg variantlib_m[] = {
    {"__tostring", variant_tostring},
    {NULL, NULL}
};

int luaopen_variantlib(lua_State* L)
{
#ifdef MERGE_VARIANT_LIB
    lua_getglobal(L, VARIANT_LIB_NAME);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        luaL_newmetatable(L, VARIANT_METATABLE_NAME);
        lua_newtable(L);
    }
#else
    luaL_newmetatable(L, VARIANT_METATABLE_NAME);
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, variant_gc);
    lua_settable(L, -3);

    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);  /* pushes the metatable */
    lua_settable(L, -3);  /* metatable.__index = metatable */
    luaL_setfuncs(L, variantlib_m, 0);

    lua_newtable(L);
#endif
    luaL_setfuncs(L, variantlib, 0);
    lua_setglobal(L, VARIANT_LIB_NAME);

    return 1;
}

}
