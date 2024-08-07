//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

//#include "sonia/concurrency.hpp"
//#include "sonia/utility/concurrency/rw_fiber_mutex.hpp"
#include "sonia/utility/mvvm/view_model.hpp"
#include "sonia/utility/lua/lua.hpp"

namespace sonia {

class lua_view_model
    : public virtual view_model
    , private sonia::lua::language
    , public invocation::registrar<lua_view_model, view_model>
{
    using registrar_type = invocation::registrar<lua_view_model, view_model>::registrar_type;
    friend class invocation::registrar<lua_view_model, view_model>;

public:
    using view_model::view_model;

    // scripting

    void load_file(string_view pathstr);
    void load_lua(std::string code);
    cstring_view append_inplace(string_view code, bool no_return);

    smart_blob eval_lua(string_view code);
    smart_blob eval_lua(string_view code, resolver* r = nullptr);

    bool try_invoke(string_view methodname, span<const blob_result> args, smart_blob& result) noexcept override;
    bool try_get_property(string_view propname, smart_blob& result) const override;
    bool try_set_property(string_view propname, blob_result const& val) override;
    //smart_blob invoke(string_view methodname, span<const blob_result> args) override;
    //smart_blob get_property(string_view propname) const override;
    //void set_property(string_view propname, blob_result const& val) override;

protected:
    // methods routine
    static void do_registration(registrar_type& mr);
};

}
