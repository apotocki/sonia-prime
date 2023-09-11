//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "lua_view_model.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia {

void lua_view_model::do_registration(registrar_type& mr)
{
    mr.register_method<&lua_view_model::load_lua>("load_lua");
    mr.register_method<&lua_view_model::load_lua>("load_code");

    using eval_lua_t = smart_blob(lua_view_model::*)(string_view);
    mr.register_method<(eval_lua_t)&lua_view_model::eval_lua>("eval_lua");

    mr.register_method<&lua_view_model::append_inplace>("append_inplace");
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
        return ctx_.view_model::get_property(key).detach();
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
        SCOPE_EXIT([&value](){  blob_result_unpin(&value); });
        return ctx_.try_set_property(key, value);
    }

    blob_result invoke(string_view name, std::span<const blob_result> args) override
    {
        return ctx_.view_model::invoke(name, args).detach();
    }
};

smart_blob lua_view_model::invoke(string_view methodname, span<const blob_result> args)
{
    smart_blob result;
    if (!try_invoke(methodname, args, result)) {
        vm_lua_resolver rslv{ *this };
        result = as_cstring<32>(methodname, [args, &rslv, this](cstring_view methodname_cstr) {
            return lua::language::eval_inplace(methodname_cstr, args, &rslv);
        });
    }
    return result;
}

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
    if (!try_set_property(propname, val)) {
        as_cstring<32>(propname, [&val, this](cstring_view propname_cstr) {
            lua::language::set_global_property(propname_cstr, val);
        });
    }
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
