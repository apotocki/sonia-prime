//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "variant_lib.hpp"
#include "sonia/utility/invokation/invokation.hpp"
#include "sonia/utility/scope_exit.hpp"
#include <sstream>

#include <boost/multiprecision/cpp_int.hpp>

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h> 

}

#define VARIANT_METATABLE_NAME "sonia.variant"
#define VARIANT_LIB_NAME "variant"

namespace sonia::lua {

static const char context_key = '_';

int ext_index(lua_State* L)
{
    lua_pushlightuserdata(L, (void*)&context_key);
    lua_gettable(L, LUA_REGISTRYINDEX);
    language* lang = (language*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return lang->resolve_global();
}

int ext_newindex(lua_State* L)
{
    lua_pushlightuserdata(L, (void*)&context_key);
    lua_gettable(L, LUA_REGISTRYINDEX);
    language* lang = (language*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return lang->set_global();
}

void setup_ext(lua_State* L, language* inv)
{
    lua_pushlightuserdata(L, (void*)&context_key);
    lua_pushlightuserdata(L, inv);
    lua_settable(L, LUA_REGISTRYINDEX);

    luaL_Reg const metamethods[] = {
        { "__index", ext_index },
        { "__newindex", ext_newindex },
        { NULL, NULL }
    };

    lua_pushglobaltable(L);
    luaL_newlib(L, metamethods);
    lua_setmetatable(L, -2);
}

int ext_invoke(lua_State* L)
{
    auto fname_index = lua_upvalueindex(1);
    char const* strval = lua_tostring(L, fname_index);
    size_t sz = lua_rawlen(L, fname_index);
    string_view fname{ strval, sz };

    int argcount = lua_gettop(L);
    std::vector<blob_result> args;
    for (int i = 0; i < argcount; ++i) {
        args.push_back(to_blob(L, i + 1));
    }
    SCOPE_EXIT([&args] {
        for (auto arg : args) blob_result_unpin(&arg);
    });

    lua_pushlightuserdata(L, (void*)&context_key);
    lua_gettable(L, LUA_REGISTRYINDEX);
    language* lang = (language*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return lang->invoke_global(fname, args);
}

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

void push_from_blob(lua_State* L, blob_result const& b)
{
    if (b.type == blob_type::unspecified && !b.data) {
        lua_pushnil(L);
        return;
    }
    if (b.is_array || b.type == blob_type::unspecified) {
        push_variant(L, b);
        /*
        lua_newtable(L);
        blob_result_type_selector(b, [L, this](auto ident, blob_result b) {
            using type = typename decltype(ident)::type;
            using ftype = std::conditional_t<std::is_void_v<type>, uint8_t, type>;
            using fstype = std::conditional_t<std::is_same_v<ftype, bool>, uint8_t, ftype>;
            for (int32_t i = 0; i < b.size / sizeof(fstype); ++i) {
                lua_pushinteger(L, (lua_Integer)(i+1));
                if constexpr (is_same_v<fstype, blob_result>) {
                    push_from_blob(*(reinterpret_cast<blob_result const*>(b.data) + i));
                } else if constexpr (is_integral_v<fstype>) {
                    fstype const* pval = reinterpret_cast<fstype const*>(b.data) + i;
                    lua_pushinteger(L, (lua_Integer)*pval);
                } else {
                    fstype const* pval = reinterpret_cast<fstype const*>(b.data) + i;
                    lua_pushnumber(L, (lua_Number)*pval);
                }
                lua_settable(L, -3);
            }
        });
        */
        return;
    }
    switch (b.type) {
    case blob_type::boolean:
        lua_pushboolean(L, (int)b.ui8value);
        return;

    case blob_type::unspecified:
    case blob_type::i8:
        lua_pushinteger(L, (lua_Integer)b.i8value); return;
    case blob_type::ui8:
        lua_pushinteger(L, (lua_Integer)b.ui8value); return;
    case blob_type::i16:
        lua_pushinteger(L, (lua_Integer)b.i16value); return;
    case blob_type::ui16:
        lua_pushinteger(L, (lua_Integer)b.ui16value); return;
    case blob_type::i32:
        lua_pushinteger(L, (lua_Integer)b.i32value); return;
    case blob_type::ui32:
        lua_pushinteger(L, (lua_Integer)b.ui32value); return;
    case blob_type::i64:
        lua_pushinteger(L, (lua_Integer)b.i64value); return;
    case blob_type::ui64:
        if (b.ui64value > (uint64_t)(std::numeric_limits<int64_t>::max)()) {
            push_variant(L, b);
        } else {
            lua_pushinteger(L, (lua_Integer)b.ui64value);
        }
        return;
    case blob_type::flt:
        lua_pushnumber(L, (lua_Number)b.floatvalue); return;
    case blob_type::string:
        lua_pushlstring(L, (const char*)b.data, b.size); return;
        //case blob_type::blob:
    case blob_type::function:
        lua_pushlstring(L, (const char*)b.data, b.size);
        lua_pushcclosure(L, ext_invoke, 1);
        //lua_pushcfunction(L, ext_invoke); 
        return;
    default:
        lua_pushnil(L);
    }
}

int variant_index(lua_State* L)
{
    blob_result* br = luaL_check_variant_lib(L, 1);
    luaL_argcheck(L, !!br, 1, "`variant' expected");

    luaL_getmetatable(L, VARIANT_METATABLE_NAME);
    lua_pushvalue(L, 2);
    lua_rawget(L, -2);

    if (!lua_isnil(L, -1)) return 1;

    auto index = luaL_checkinteger(L, 2);
    if (!br->is_array) { // scalar
        if (index == 1) { // push itself
            return push_variant(L, *br);
        }
        lua_pushnil(L);
    }
    blob_result_type_selector(*br, [L, c_index = index - 1](auto ident, blob_result b) {
        using type = typename decltype(ident)::type;
        using ftype = std::conditional_t<std::is_void_v<type>, uint8_t, type>;
        using fstype = std::conditional_t<std::is_same_v<ftype, bool>, uint8_t, ftype>;

        if (c_index < 0 || b.size / sizeof(fstype) <= (size_t)c_index) {
            lua_pushnil(L);
        } else {
            if constexpr (is_same_v<fstype, blob_result>) {
                push_from_blob(L, *(reinterpret_cast<blob_result const*>(b.data) + c_index));
            } else if constexpr (is_integral_v<fstype>) {
                fstype const* pval = reinterpret_cast<fstype const*>(b.data) + c_index;
                lua_pushinteger(L, (lua_Integer)*pval);
            } else {
                fstype const* pval = reinterpret_cast<fstype const*>(b.data) + c_index;
                lua_pushnumber(L, (lua_Number)*pval);
            }
        }
    });
    return 1;
}

int variant_len(lua_State* L)
{
    blob_result* br = luaL_check_variant_lib(L, 1);
    luaL_argcheck(L, !!br, 1, "`variant' expected");

    if (br->is_array) {
        lua_Integer count = blob_result_type_selector(*br, [](auto ident, blob_result b) {
            using type = typename decltype(ident)::type;
            using ftype = std::conditional_t<std::is_void_v<type>, uint8_t, type>;
            using fstype = std::conditional_t<std::is_same_v<ftype, bool>, uint8_t, ftype>;
            return b.size / sizeof(fstype);
        });
        lua_pushinteger(L, count);
    } else {
        luaL_error(L, "is not an array");
    }
    return 1;
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

template <std::integral T>
int variant_array(lua_State* L)
{
    int argcount = lua_gettop(L);
    blob_result br{ nullptr, static_cast<int32_t>(argcount * sizeof(T)), 0, 1, blob_type_for<T>() };
    blob_result_allocate(&br);
    defer{ blob_result_unpin(&br); };
    T* arr_data = reinterpret_cast<T*>(const_cast<void*>(br.data));
    for (int i = 0; i < argcount; ++i) {
        auto elemval = luaL_checkinteger(L, i + 1);
        bool check = (std::numeric_limits<T>::min)() <= elemval && (std::numeric_limits<T>::max)() >= elemval;
        luaL_argcheck(L, check, i + 1, "invalid value");
        arr_data[i] = static_cast<T>(elemval);
    }
    return push_variant(L, br);
}

int variant_int(lua_State* L)
{
    if (lua_isinteger(L, 1)) {
        lua_Integer i = lua_tointeger(L, 1);
        return push_variant(L, i64_blob_result(i));
    } else if (lua_isstring(L, 1)) {
        char const* strval = lua_tostring(L, 1);
        size_t sz = lua_rawlen(L, 1);
        using integer_type = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<65, 0>>;
        integer_type ival(string_view{ strval, sz });
        if (ival >= 0 && ival <= (std::numeric_limits<uint64_t>::max)()) {
            return push_variant(L, ui64_blob_result(ival.convert_to<uint64_t>()));
        } else if (ival <= (std::numeric_limits<int64_t>::max)() && ival >= (std::numeric_limits<int64_t>::min)()) {
            return push_variant(L, i64_blob_result(ival.convert_to<int64_t>()));
        } else {
            return luaL_error(L, "invalid argument");
        }
    } else {
        return luaL_error(L, "invalid argument");
    }
}

int variant_type(lua_State* L)
{
    blob_result* br = luaL_check_variant_lib(L, 1);
    luaL_argcheck(L, !!br, 1, "`variant' expected");
    std::ostringstream s;
    print_type(s, *br);
    std::string result = s.str();
    lua_pushfstring(L, "%s", result.c_str());
    return 1;
}

const struct luaL_Reg variantlib[] = {
    {"ui8array", variant_array<uint8_t>},
    {"i8array", variant_array<int8_t>},
    {"int", variant_int},
    {NULL, NULL}
};

const struct luaL_Reg variantlib_m[] = {
    {"__tostring", variant_tostring},
    {"__index", variant_index},
    {"__len", variant_len},
    {"__gc", variant_gc},
    {NULL, NULL}
};

const struct luaL_Reg variantlib_f[] = {
    {"type", variant_type},
    {NULL, NULL}
};

int luaopen_variantlib(lua_State* L)
{
#ifdef MERGE_VARIANT_LIB // hasn't tested
    lua_getglobal(L, VARIANT_LIB_NAME);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        luaL_newmetatable(L, VARIANT_METATABLE_NAME);
        lua_newtable(L);
    }
#else
    luaL_newmetatable(L, VARIANT_METATABLE_NAME);
    luaL_setfuncs(L, variantlib_m, 0);
    luaL_setfuncs(L, variantlib_f, 0);
    lua_pop(L, 1);

    lua_newtable(L);
#endif
    luaL_setfuncs(L, variantlib, 0);
    lua_setglobal(L, VARIANT_LIB_NAME);

    return 1;
}

blob_result to_blob(lua_State* L, int index)
{
    int luavaltype = lua_type(L, index);
    switch (luavaltype) {
        case LUA_TNUMBER:
            if (lua_isinteger(L, index)) {
                lua_Integer i = lua_tointeger(L, index);
                return i64_blob_result(i);
            } else {
                lua_Number i = lua_tonumber(L, index);
                return float_blob_result((float) i);
            }

        case LUA_TBOOLEAN:
        {
            int bval = lua_toboolean(L, index);
            return bool_blob_result(!!bval);
        }

        case LUA_TSTRING:
        {
            char const* strval = lua_tostring(L, index);
            size_t sz = lua_rawlen(L, index);
            auto result = string_blob_result(string_view(strval, sz));
            blob_result_allocate(&result);
            return result;
        }

        case LUA_TTABLE:
        {
            lua_pushvalue(L, index);
            lua_pushnil(L);
            blob_type ktype = blob_type::unspecified, vtype = blob_type::unspecified;
            bool no_unique_k = false, no_unique_v = false;
            std::vector<blob_result> vals;
            std::vector<blob_result> keys;
            while (lua_next(L, -2) != 0) {
                keys.push_back(to_blob(L, -2));
                vals.push_back(to_blob(L, -1));
                lua_pop(L, 1);
                if (ktype == blob_type::unspecified) {
                    ktype = keys.back().type;
                } else if (ktype != keys.back().type) {
                    no_unique_k = true;
                }
                if (vtype == blob_type::unspecified) {
                    vtype = vals.back().type;
                } else if (vtype != vals.back().type) {
                    no_unique_v = true;
                }
            }
            lua_pop(L, 1);
            if (keys.empty()) return nil_blob_result();

            if (no_unique_v) vtype = blob_type::blob;
            smart_blob blob_values;
            if (vtype == blob_type::flt) {
                blob_values = blob_result{ nullptr, (int32_t)(vals.size() * sizeof(float)), 0, 1, vtype };
                blob_values.allocate();
                for (size_t i = 0; i < vals.size(); ++i) {
                    *(reinterpret_cast<float*>(const_cast<void*>(blob_values->data)) + i) = vals[i].floatvalue;
                }
            } else if (vtype == blob_type::i64) {
                blob_values = blob_result{ nullptr, (int32_t)(vals.size() * sizeof(int64_t)), 0, 1, vtype };
                blob_values.allocate();
                for (size_t i = 0; i < vals.size(); ++i) {
                    *(reinterpret_cast<int64_t*>(const_cast<void*>(blob_values->data)) + i) = vals[i].i64value;
                }
            } else if (vtype == blob_type::string || vtype == blob_type::blob) {
                blob_values = blob_result{ nullptr, (int32_t)(vals.size() * sizeof(blob_result)), 0, 1, vtype };
                blob_values.allocate();
                for (size_t i = 0; i < vals.size(); ++i) {
                    *(reinterpret_cast<blob_result*>(const_cast<void*>(blob_values->data)) + i) = vals[i];
                }
            } else {
                // not implemented for now
                for (blob_result v : vals) blob_result_unpin(&v);
            }

            // check if array
            if (!no_unique_k && ktype == blob_type::i64) {
                bool is_array = true;
                for (int i = 0; i < keys.size(); ++i) {
                    if (auto kval = keys[i].i64value; kval != (i+1)) {
                        //if (float fval = keys[i].i64value.floatvalue; fmod(fval, 1.0) > 0.00001 || ((int)fval) != (i+1)) {
                        is_array = false;
                        break;
                    }
                }
                if (is_array) {
                    return blob_values.detach();
                }
            }

            smart_blob result = blob_result{nullptr, (int32_t)(2 * sizeof(blob_result)), 0, 1, blob_type::blob};
            result.allocate();
            smart_blob blob_keys = blob_result{nullptr, (int32_t)(keys.size() * sizeof(blob_result)), 0, 1, blob_type::blob};
            blob_keys.allocate();

            for (size_t i = 0; i < keys.size(); ++i) {
                *(reinterpret_cast<blob_result*>(const_cast<void*>(blob_keys->data)) + i) = keys[i];
            }
            *reinterpret_cast<blob_result*>(const_cast<void*>(result->data)) = blob_keys.detach();
            *(reinterpret_cast<blob_result*>(const_cast<void*>(result->data)) + 1) = blob_values.detach();
            return result.detach();
        }

        case LUA_TUSERDATA:
            if (blob_result* br = luaL_check_variant_lib(L, index); br) {
                blob_result_pin(br);
                return *br;
            }

        case LUA_TNIL:
        default:
            return nil_blob_result();
    }
}

}
