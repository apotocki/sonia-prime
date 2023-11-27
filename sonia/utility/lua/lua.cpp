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

#include "variant_lib.hpp"

namespace sonia::lua {

language::language()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L); /* Load Lua libraries */

    luaopen_variantlib(L);

    setup_ext(L, this);

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
    blob_result result = to_blob(L, -1);
    lua_pop(L, 1);
    return result;
}

void language::set_global_property(cstring_view name, blob_result const& val)
{
    lua_State* L = reinterpret_cast<lua_State*>(L_);
    push_from_blob(L, val);
    lua_setglobal(L, name.c_str());
}

blob_result language::eval_inplace(cstring_view fn, std::span<const blob_result> args, resolver * r)
{
    resolvers_.push_back(r);
    defer{ resolvers_.pop_back(); };

    lua_State* L = reinterpret_cast<lua_State*>(L_);
    lua_getglobal(L, fn.c_str());
    for (blob_result arg : args) {
        push_from_blob(L, arg);
    }
    if (int res = lua_pcall(L, (int)args.size(), 1, 0); !!res) {
        std::string err = lua_tostring(L, -1);
        lua_pop(L, 1);
        throw exception("lua evaluation '%2%' script error (%3%): %1%"_fmt % err % fn % res);
    }
    blob_result result = to_blob(L, -1);
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
    push_from_blob(L, val);
    //GLOBAL_LOG_INFO() << "global key found, value: " << val;
    return 1;
}

int language::set_global()
{
    lua_State* L = reinterpret_cast<lua_State*>(L_);
    char const* key = lua_tostring(L, 2);
    if (!key) {
        return 0;
    }
    if (resolvers_.empty() || !resolvers_.back() || !resolvers_.back()->newindex(key, to_blob(L, 3))) {
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
        if (res->type == blob_type::tuple) {
            size_t cnt = array_size_of<blob_result>(*res);
            for (blob_result r : std::span{ data_of<blob_result>(*res), cnt }) {
                push_from_blob(L, r);
            }
            return static_cast<int>(cnt);
        }
        push_from_blob(L, *res);
        return 1;
    } catch (std::exception const& e) {
        //GLOBAL_LOG_INFO() << "lua global invoked '" << fname << "', error: '" << e.what() << "'";
        return luaL_error(L, e.what());
    }
}

}
