//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "bang_view_model.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia {

void bang_view_model::do_registration(registrar_type& mr)
{
    using load_bang_t = void(bang_view_model::*)(string_view);
    mr.register_method<(load_bang_t)&bang_view_model::build>("load_bang"sv);
    mr.register_method<(load_bang_t)&bang_view_model::build>("load_code"sv);
    mr.register_method<&bang_view_model::load_file>("load_file"sv);
    using load_bang_t = void(bang_view_model::*)(string_view);
    //using eval_bang_t = smart_blob(bang_view_model::*)(string_view);
    //mr.register_method<(eval_lua_t)&bang_view_model::eval_bang>("eval_bang"sv);
    //mr.register_method<&bang_view_model::append_inplace>("append_inplace"sv);
}

void bang_view_model::load_file(string_view pathstr)
{
    fs::path path{ u8string_view{reinterpret_cast<char8_t const*>(pathstr.data()), pathstr.size()} };
    build(path);
}

}
