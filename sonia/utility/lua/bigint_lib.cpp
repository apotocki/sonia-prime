//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "bigint_lib.hpp"

#include <boost/multiprecision/cpp_int.hpp>

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h> 

}

#define BIGINT_METATABLE_NAME "sonia.bigint"
#define BIGINT_LIB_NAME "bigint"

namespace sonia::lua {

struct biginit_header
{
    uint64_t size : 63;
    uint64_t sign : 1;
};

using integer_type = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<65, 0>>;
using limb_type = boost::multiprecision::limb_type;

biginit_header* luaL_check_bigint_lib(lua_State* L, int index)
{
    return (biginit_header*)luaL_checkudata(L, index, BIGINT_METATABLE_NAME);
}

biginit_header* luaL_test_bigint_lib(lua_State* L, int index)
{
    return (biginit_header*)luaL_testudata(L, index, BIGINT_METATABLE_NAME);
}

int bigint_index(lua_State* L)
{
    biginit_header* bh = luaL_test_bigint_lib(L, 1);
    luaL_argcheck(L, !!bh, 1, "`bigint' is expected");

    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_pushvalue(L, 2);
    lua_rawget(L, -2);

    return 1;
    /*
    if (!lua_isnil(L, -1)) return 1;

    auto index = luaL_checkinteger(L, 2);
    if (!is_array(*br)) { // scalar
        if (index == 1) { // push itself
            return push_variant(L, *br);
        }
        lua_pushnil(L);
    }
    */
}

int push_bigint(lua_State* L, integer_type const& value)
{
    size_t sz = value.backend().size();
    size_t datasz = sz * sizeof(limb_type);
    biginit_header* br = (biginit_header*)lua_newuserdata(L, sizeof(biginit_header) + datasz);
    br->size = sz;
    br->sign = value.backend().sign() ? 1 : 0;
    memcpy(br + 1, value.backend().limbs(), datasz);

    luaL_getmetatable(L, BIGINT_METATABLE_NAME);
    lua_setmetatable(L, -2);

    return 1;
}

int bigint_create(lua_State* L)
{
    try {
        if (lua_isinteger(L, 1)) {
            integer_type ival = lua_tointeger(L, 1);
            return push_bigint(L, ival);
        } else if (lua_isstring(L, 1)) {
            size_t sz;
            char const* strval = lua_tolstring(L, 1, &sz);
            integer_type ival(string_view{ strval, sz });
            return push_bigint(L, ival);
        }
        return luaL_argerror(L, 1, "invalid argument");
    } catch (std::exception const& e) {
        return luaL_argerror(L, 1, e.what());
    }
}

void restore_bigint(biginit_header* bh, integer_type & ival)
{
    ival.backend().resize(bh->size, bh->size);
    memcpy(ival.backend().limbs(), bh + 1, bh->size * sizeof(limb_type));
    if (bh->sign != ival.backend().sign()) ival.backend().negate();
}

int bigint_tostring(lua_State* L)
{
    biginit_header* bh = luaL_test_bigint_lib(L, 1);
    luaL_argcheck(L, !!bh, 1, "`bigint' is expected");
    integer_type ival;
    restore_bigint(bh, ival);
    
    std::ostringstream s;
    s << ival;
    lua_pushfstring(L, s.str().c_str());
    return 1;
}

int bigint_fancy_string(lua_State* L)
{
    biginit_header* bh = luaL_test_bigint_lib(L, 1);
    luaL_argcheck(L, !!bh, 1, "`bigint' is expected");
    integer_type ival;
    restore_bigint(bh, ival);

    lua_Integer radix = 10;
    lua_Integer groupSz = 0;
    string_view delim = " "sv;
    int argcount = lua_gettop(L);
    if (argcount > 1) {
        radix = luaL_checkinteger(L, 2);
        if (argcount > 2) {
            groupSz = luaL_checkinteger(L, 3);
            if (argcount > 3) {
                size_t sz;
                char const* strval = luaL_checklstring(L, 4, &sz);
                delim = string_view{ strval, sz };
            }
        }
    }

    // formatting
    std::ostringstream ss, tempss;
    if (radix == 16) {
        if (ival < 0) {
            ss << '-';
            ival.backend().negate();
        }
        ss << "0x"sv;
        tempss << std::hex << std::uppercase << ival;
    } else {
        tempss << ival;
    }

    std::string val = tempss.str();
    if (!groupSz) {
        ss << val;
    } else {
        for (size_t i = 0, sz = val.size(); i < sz; ++i) {
            ss << val[i];
            if (size_t pos = sz - i - 1; pos && 0 == pos % groupSz) {
                ss << delim;
            }
        }
    }

    lua_pushfstring(L, ss.str().c_str());
    return 1;
}

int bigint_to_integer(lua_State* L)
{
    integer_type val;
    biginit_header* bh = luaL_test_bigint_lib(L, 1);
    luaL_argcheck(L, !!bh, 1, "`bigint' is expected");
    restore_bigint(bh, val);
    luaL_argcheck(L, val >= (std::numeric_limits<lua_Integer>::min)() &&
        val <= (std::numeric_limits<lua_Integer>::max)(), 1, "out of bounds");
    lua_pushinteger(L, (lua_Integer)val);
    return 1;
}

void bigint_binary_operator(lua_State* L, integer_type & l_val, integer_type& r_val)
{
    biginit_header* blh = luaL_test_bigint_lib(L, 1);
    if (blh) { 
        restore_bigint(blh, l_val);
    } else if (lua_isinteger(L, 1)) {
        l_val = lua_tointeger(L, 1);
    } else {
        luaL_argerror(L, 1, "`bigint' or integer expected");
    }

    biginit_header* brh = luaL_test_bigint_lib(L, 2);
    if (brh) {
        restore_bigint(brh, r_val);
    } else if (lua_isinteger(L, 2)) {
        r_val = lua_tointeger(L, 2);
    } else {
        luaL_argerror(L, 2, "`bigint' or integer expected");
    }
}

int bigint_unary_minus(lua_State* L)
{
    integer_type val;
    biginit_header* bh = luaL_test_bigint_lib(L, 1);
    luaL_argcheck(L, !!bh, 1, "`bigint' is expected");
    restore_bigint(bh, val);
    return push_bigint(L, -val);
}

int bigint_equal(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    lua_pushboolean(L, l_val == r_val);

    return 1;
}

int bigint_lt(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    lua_pushboolean(L, l_val < r_val);

    return 1;
}

int bigint_le(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    lua_pushboolean(L, l_val <= r_val);

    return 1;
}

int bigint_add(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    return push_bigint(L, l_val + r_val);
}

int bigint_sub(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    return push_bigint(L, l_val - r_val);
}

int bigint_mul(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    return push_bigint(L, l_val * r_val);
}

int bigint_div(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    return push_bigint(L, l_val / r_val);
}

int bigint_mod(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    return push_bigint(L, l_val % r_val);
}

int bigint_pow(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    luaL_argcheck(L, r_val >= (std::numeric_limits<unsigned int>::min)() &&
        r_val <= (std::numeric_limits<unsigned int>::max)(), 2, "out of bounds");

    return push_bigint(L, pow(l_val, (unsigned int)r_val));
}

int bigint_band(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    return push_bigint(L, l_val & r_val);
}

int bigint_bor(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    return push_bigint(L, l_val | r_val);
}

int bigint_bxor(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    return push_bigint(L, l_val ^ r_val);
}

int bigint_unary_not(lua_State* L)
{
    integer_type val;
    biginit_header* bh = luaL_test_bigint_lib(L, 1);
    luaL_argcheck(L, !!bh, 1, "`bigint' is expected");
    restore_bigint(bh, val);
    return push_bigint(L, ~val);
}

int bigint_shl(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    luaL_argcheck(L, r_val >= (std::numeric_limits<unsigned int>::min)() &&
        r_val <= (std::numeric_limits<unsigned int>::max)(), 2, "out of bounds");

    return push_bigint(L, l_val << (unsigned int)r_val);
}

int bigint_shr(lua_State* L)
{
    integer_type l_val, r_val;
    bigint_binary_operator(L, l_val, r_val);

    luaL_argcheck(L, r_val >= (std::numeric_limits<unsigned int>::min)() &&
        r_val <= (std::numeric_limits<unsigned int>::max)(), 2, "out of bounds");

    return push_bigint(L, l_val >> (unsigned int)r_val);
}

const struct luaL_Reg bigintlib[] = {
    {"create", bigint_create},
    /*
    {"to_datetime_string", variant_datetime_string},
    {"encode", variant_encode},
    */
    {NULL, NULL}
};

const struct luaL_Reg bigintlib_m[] = {
    {"__tostring", bigint_tostring},
    {"__index", bigint_index},
    {"__eq", bigint_equal},
    {"__lt", bigint_lt},
    {"__le", bigint_le},
    {"__add", bigint_add},
    {"__sub", bigint_sub},
    {"__mul", bigint_mul},
    {"__div", bigint_div},
    {"__mod", bigint_mod},
    {"__pow", bigint_pow},
    {"__unm", bigint_unary_minus},
    {"__band", bigint_band},
    {"__bor", bigint_bor},
    {"__bxor", bigint_bxor},
    {"__bnot", bigint_unary_not},
    {"__shl", bigint_shl},
    {"__shr", bigint_shr},
    /*
    {"__len", variant_len},
    
    {"__gc", variant_gc},
    */
    {NULL, NULL}
};

const struct luaL_Reg bigintlib_f[] = {
    {"to_fancy_string", bigint_fancy_string},
    {"to_integer", bigint_to_integer},
    /*
    {"type", variant_type},
    {"decode", variant_decode},
    */
    {NULL, NULL}
};

int luaopen_bigintlib(lua_State* L)
{
    luaL_newmetatable(L, BIGINT_METATABLE_NAME);
    luaL_setfuncs(L, bigintlib_m, 0);
    luaL_setfuncs(L, bigintlib_f, 0);
    lua_pop(L, 1);

    lua_newtable(L);

    luaL_setfuncs(L, bigintlib, 0);
    lua_setglobal(L, BIGINT_LIB_NAME);

    return 1;
}

}
