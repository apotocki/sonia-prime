//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "lua.hpp"

#include "sonia/exceptions.hpp"
#include "sonia/utility/scope_exit.hpp"
//#include "sonia/singleton.hpp"
//#include "sonia/services.hpp"

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h> 

}

namespace sonia::lua {

blob_result to_blob(lua_State* L, int index);

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

int ext_invoke(lua_State* L)
{
    auto fname_index = lua_upvalueindex(1);
    char const* strval = lua_tostring(L, fname_index);
    size_t sz = lua_rawlen(L, fname_index);
    string_view fname{strval, sz};

    int argcount = lua_gettop(L);
    std::vector<blob_result> args;
    for (int i = 0; i < argcount; ++i) {
        args.push_back(to_blob(L, i + 1));
    }
    SCOPE_EXIT([&args]{
        for (auto arg : args) blob_result_unpin(&arg);
    });

    lua_pushlightuserdata(L, (void*)&context_key);
    lua_gettable(L, LUA_REGISTRYINDEX);
    language* lang = (language*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return lang->invoke_global(fname, args);
}

language::language()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L); /* Load Lua libraries */

    lua_pushlightuserdata(L, (void*)&context_key);
    lua_pushlightuserdata(L, this);
    lua_settable(L, LUA_REGISTRYINDEX);

    luaL_Reg const metamethods[] = {
        { "__index", ext_index },
        { "__newindex", ext_newindex },
        { NULL, NULL }
    };

    lua_pushglobaltable(L);
    luaL_newlib(L, metamethods);
    lua_setmetatable(L, -2);

    L_ = L;
}

language::~language()
{
    lua_close(reinterpret_cast<lua_State*>(L_));
}

void language::append_code(std::string code)
{
    lua_State* L = reinterpret_cast<lua_State*>(L_);
    //auto it = codes_.find(code, hasher{}, string_equal_to{});
    //if (it == codes_.end()) {
        if (luaL_loadbuffer(L, code.data(), code.size(), code.c_str())) {
            std::string err = lua_tostring(L, -1);
            lua_pop(L, 1);
            throw exception("lua loading script error: %1%, script: %2%"_fmt % err % code);
        }
        if (lua_pcall(L, 0, 0, 0)) {
            std::string err = lua_tostring(L, -1);
            lua_pop(L, 1);
            throw exception("lua applying script error: %1%, script: %2%"_fmt % err % code);
        }
        //codes_.insert(it, std::move(code));
    //}
}

cstring_view language::append_inplace(string_view code, bool no_return)
{
    lua_State* L = reinterpret_cast<lua_State*>(L_);
    auto it = inplace_fns_.find(code, hasher{}, string_equal_to{});
    if (it == inplace_fns_.end()) {
        std::ostringstream fnnamess;
        fnnamess << "inplace_" << inplace_fns_.size();
        std::string fnname = fnnamess.str();

        std::ostringstream fncodess;
        fncodess << "function " << fnname << "(...)\n local args = {...}\n";
        if (!no_return) {
            fncodess << "return (" << code << ") end";
        } else {
            fncodess << code << "\n end";
        }
        std::string codestr = fncodess.str();

        std::string origcodestr{code};
        if (luaL_loadbuffer(L, codestr.data(), codestr.size(), origcodestr.c_str())) {
            std::string err = lua_tostring(L, -1);
            lua_pop(L, 1);
            throw exception("lua loading script error: %1%, script: %2%"_fmt % err % codestr);
        }
        if (lua_pcall(L, 0, 0, 0)) {
            std::string err = lua_tostring(L, -1);
            lua_pop(L, 1);
            throw exception("lua applying script error: %1%, script: %2%"_fmt % err % codestr);
        }
        it = inplace_fns_.insert(std::pair{ std::move(origcodestr), std::move(fnname) }).first;
    }
    return it->second;
}

blob_result language::get_global_property(cstring_view name) const
{
    lua_State* L = reinterpret_cast<lua_State*>(L_);
    lua_getglobal(L, name.c_str());
    blob_result result = to_blob(-1);
    lua_pop(L, 1);
    return result;
}

void language::set_global_property(cstring_view name, blob_result const& val)
{
    lua_State* L = reinterpret_cast<lua_State*>(L_);
    push_from_blob(val);
    lua_setglobal(L, name.c_str());
}

blob_result language::eval_inplace(cstring_view fn, std::span<const blob_result> args, resolver * r)
{
    resolvers_.push_back(r);
    defer{ resolvers_.pop_back(); };

    lua_State* L = reinterpret_cast<lua_State*>(L_);
    lua_getglobal(L, fn.c_str());
    for (blob_result arg : args) {
        push_from_blob(arg);
    }
    if (int res = lua_pcall(L, (int)args.size(), 1, 0); !!res) {
        std::string err = lua_tostring(L, -1);
        lua_pop(L, 1);
        throw exception("lua evaluation '%2%' script error (%3%): %1%"_fmt % err % fn % res);
    }
    blob_result result = to_blob(-1);
    lua_pop(L, 1);
    return result;
}

blob_result language::eval(string_view code, resolver * r)
{
    cstring_view fn = append_inplace(code, false);
    return eval_inplace(fn, {}, r);
}

int language::resolve_global()
{
    lua_State* L = reinterpret_cast<lua_State*>(L_);
    char const* key = lua_tostring(L, 2);
    //GLOBAL_LOG_INFO() << "looking for global key: " << (key ? key : "<nil>");
    if (!key) {
        lua_pushnil(L);
        //GLOBAL_LOG_INFO() << "no global key found";
        return 1;
    }
    
    blob_result val = (!resolvers_.empty() && resolvers_.back())  ? resolvers_.back()->index(key) : nil_blob_result();
    SCOPE_EXIT([&val] { blob_result_unpin(&val); });
    push_from_blob(val);
    //GLOBAL_LOG_INFO() << "global key found, value: " << val;
    return 1;
}

void language::push_from_blob(blob_result const& b)
{
    lua_State* L = reinterpret_cast<lua_State*>(L_);
    if (b.type == blob_type::unspecified && !b.data) {
        lua_pushnil(L);
        return;
    }
    if (b.is_array || b.type == blob_type::unspecified) {
        lua_newtable(L);
        blob_result_type_selector(b, [L, this](auto ident, blob_result b) {
            using type = typename decltype(ident)::type;
            using ftype = std::conditional_t<std::is_void_v<type>, uint8_t, type>;
            using fstype = std::conditional_t<std::is_same_v<ftype, bool>, uint8_t, ftype>;
            for (int32_t i = 0; i < b.size / sizeof(fstype); ++i) {
                lua_pushnumber(L, (lua_Number)(i+1));
                if constexpr (is_same_v<fstype, string_view> || is_same_v<fstype, blob_result>) {
                    push_from_blob(*(reinterpret_cast<blob_result const*>(b.data) + i));
                } else {
                    fstype const* pval = reinterpret_cast<fstype const*>(b.data) + i;
                    lua_pushnumber(L, (lua_Number)*pval);
                }
                lua_settable(L, -3);
            }
        });
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
                lua_pushnumber(L, (lua_Number) b.ui64value);
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
            break;

        case LUA_TSTRING:
        {
            char const* strval = lua_tostring(L, index);
            size_t sz = lua_rawlen(L, index);
            auto result = string_blob_result(string_view(strval, sz));
            blob_result_allocate(&result);
            return result;
        }
            break;

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
                if (is_array) return blob_values.detach();
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

        case LUA_TNIL:
        default:
            return nil_blob_result();
    }
}

blob_result language::to_blob(int index) const
{
    lua_State* L = reinterpret_cast<lua_State*>(L_);
    return sonia::lua::to_blob(L, index);
}

int language::set_global()
{
    lua_State* L = reinterpret_cast<lua_State*>(L_);
    char const* key = lua_tostring(L, 2);
    if (!key) {
        return 0;
    }
    if (resolvers_.empty() || !resolvers_.back() || !resolvers_.back()->newindex(key, to_blob(3))) {
        lua_rawset(L, 1);
    }
    return 0;
}

int language::invoke_global(string_view fname, std::span<blob_result> args)
{
    if (resolvers_.empty() || !resolvers_.back()) return 0;

    lua_State* L = reinterpret_cast<lua_State*>(L_);

    /*
    lua_Debug ar;
    lua_getstack(L, 0, &ar);
    lua_getinfo(L, "n", &ar);

    if (ar.name) {
        GLOBAL_LOG_INFO() << "METHOD NAME: '" << ar.name << "'";
    } else {
        GLOBAL_LOG_INFO() << "METHOD NAME: NIL";
    }
    */
    try {
        //GLOBAL_LOG_INFO() << "lua global invoking '" << fname << "', args: '" << args << "'";
        smart_blob res = resolvers_.back()->invoke(fname, args);
        //GLOBAL_LOG_INFO() << "lua global invoked '" << fname << "', result: '" << *res << "'";
        if (res->is_array && res->type == blob_type::blob) {
            size_t cnt = res->size / sizeof(blob_type);
            for (blob_result r : std::span{ reinterpret_cast<const blob_result*>(res->data), cnt }) {
                push_from_blob(r);
            }
            return static_cast<int>(cnt);
        }
        push_from_blob(*res);
        return 1;
    } catch (std::exception const& e) {
        //GLOBAL_LOG_INFO() << "lua global invoked '" << fname << "', error: '" << e.what() << "'";
        return luaL_error(L, e.what());
    }
}

}
