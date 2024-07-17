//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "lua_view_model.hpp"
#include "sonia/utility/scope_exit.hpp"

#include "sonia/filesystem.hpp"
#include <fstream>

namespace sonia {

void lua_view_model::do_registration(registrar_type& mr)
{
    mr.register_method<&lua_view_model::load_lua>("load_lua"sv);
    mr.register_method<&lua_view_model::load_lua>("load_code"sv);
    mr.register_method<&lua_view_model::load_file>("load_file"sv);

    using eval_lua_t = smart_blob(lua_view_model::*)(string_view);
    mr.register_method<(eval_lua_t)&lua_view_model::eval_lua>("eval_lua"sv);

    mr.register_method<&lua_view_model::append_inplace>("append_inplace"sv);
}

class vm_lua_resolver : public lua::language::resolver
{
    view_model& ctx_;

public:
    explicit vm_lua_resolver(view_model& ctx) : ctx_{ ctx } {}

    blob_result index(string_view key) override
    {
        if (ctx_.has_method(key)) {
            return function_blob_result(key);
        }
        smart_blob result;
        if (!ctx_.view_model::try_get_property(key, result)) {
            blob_result args[] = { string_blob_result(key) };
            result = ctx_.do_call_method("hasMethod", std::span{args});
            if (result->type == blob_type::boolean && result.as<bool>()) {
                return function_blob_result(key);
            }
            result = ctx_.do_call_method("hasProperty", std::span{args});
            if (result->type != blob_type::boolean || !result.as<bool>()) {
                // GLOBAL_LOG_INFO() << "no property '" << key << "' was found";
                return nil_blob_result();
            }
            result = ctx_.do_call_method("getProperty", std::span{args});
        }
        return result.detach();
        //return ctx_.view_model::get_property(key).detach();
        /*
        smart_blob result;
        if (ctx_.try_get_property(key, result)) {
            return result.detach();
        }
        return nil_blob_result();
        */
    }

    bool newindex(string_view key, blob_result&& value) override
    {
        SCOPE_EXIT([&value](){ blob_result_unpin(&value); });
        if (!ctx_.view_model::try_set_property(key, value)) {
            blob_result args[] = { string_blob_result(key), value };
            smart_blob result = ctx_.do_call_method("setProperty", std::span{args});
            if (result->type != blob_type::boolean) return false;
            return result.as<bool>();
        }
        return true;
    }

    blob_result invoke(string_view name, std::span<const blob_result> args) override
    {
        smart_blob result;
        if (!ctx_.view_model::try_invoke(name, args, result)) {
            //GLOBAL_LOG_INFO() << "callback invoking: " << name;
            result = ctx_.do_call_method(name, args);
        }
        return result.detach();
    }
};

bool lua_view_model::try_invoke(string_view methodname, span<const blob_result> args, smart_blob& result) noexcept
{
    if (view_model::try_invoke(methodname, args, result)) return true;
    //GLOBAL_LOG_INFO() << "invoking lua: " << methodname;
    vm_lua_resolver rslv{ *this };
    result = as_cstring<32>(methodname, [args, &rslv, this](cstring_view methodname_cstr) {
        return lua::language::eval_inplace(methodname_cstr, args, &rslv);
    });
    //GLOBAL_LOG_INFO() << "lua result: " << *result;
    return true;
}

/*
smart_blob lua_view_model::invoke(string_view methodname, span<const blob_result> args)
{
    smart_blob result;
    if (!try_invoke(methodname, args, result)) {
        //GLOBAL_LOG_INFO() << "invoking lua: " << methodname;
        vm_lua_resolver rslv{ *this };
        result = as_cstring<32>(methodname, [args, &rslv, this](cstring_view methodname_cstr) {
            return lua::language::eval_inplace(methodname_cstr, args, &rslv);
        });
        //GLOBAL_LOG_INFO() << "lua result: " << *result;
    }
    return result;
}
*/

bool lua_view_model::try_get_property(string_view propname, smart_blob& result) const
{
    if (view_model::try_get_property(propname, result)) return true;
    result = as_cstring<32>(propname, [this](cstring_view propname_cstr) {
        return lua::language::get_global_property(propname_cstr);
    });
    return true;
}

bool lua_view_model::try_set_property(string_view propname, blob_result const& val)
{
    if (view_model::try_set_property(propname, val)) return true;
    as_cstring<32>(propname, [&val, this](cstring_view propname_cstr) {
        lua::language::set_global_property(propname_cstr, val);
    });
    on_property_change(propname);
    return true;
}

/*
smart_blob lua_view_model::get_property(string_view propname) const
{
    smart_blob result;
    if (!try_get_property(propname, result)) {
        result = as_cstring<32>(propname, [this](cstring_view propname_cstr) {
            return lua::language::get_global_property(propname_cstr);
        });
    }
    return result;
}

void lua_view_model::set_property(string_view propname, blob_result const& val)
{
    //GLOBAL_LOG_INFO() << "lua_view_model:set_property '" << propname << "' with value: " << val;
    if (!try_set_property(propname, val)) {
        as_cstring<32>(propname, [&val, this](cstring_view propname_cstr) {
            lua::language::set_global_property(propname_cstr, val);
        });
        on_property_change(propname);
    }
}
*/

void lua_view_model::load_file(string_view pathstr)
{
    auto path = std::string{pathstr};
    std::string code;
    code.reserve(fs::file_size(path));
    std::ifstream file{ path.c_str(), std::ios::binary };
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{}, std::back_inserter(code));
    load_lua(std::move(code));
}

void lua_view_model::load_lua(std::string code)
{
    lua::language::append_code(std::move(code));
}

cstring_view lua_view_model::append_inplace(string_view code, bool no_return)
{
    return lua::language::append_inplace(code, no_return);
}

smart_blob lua_view_model::eval_lua(string_view code)
{
    vm_lua_resolver rslv{ *this };
    return eval_lua(code, &rslv);
}

smart_blob lua_view_model::eval_lua(string_view code, resolver* r)
{
    return lua::language::eval(code, r);
}

}
