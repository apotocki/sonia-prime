//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/string.hpp"
#include "sonia/utility/invokation/invokation.hpp"
#include "sonia/utility/mvvm/lua_view_model.hpp"

#include "beng/unit.hpp"
#include "sonia/utility/lang/vm.hpp"

#include "builder_vm.hpp"

namespace sonia {
 
namespace lang::beng { class parser_context; }

class builder_view_model 
    : public lua_view_model
    , vm::builder_context
{
    using base_t = lua_view_model;

public:
    using lua_view_model::lua_view_model;

    void build(fs::path const&);
    void build(string_view code);

    //virtual void create(string_view type, string_view id) = 0;
    //virtual void set_property(string_view id, string_view propname, blob_result const& value) = 0;

    lang::beng::unit& unit() { return unit_; };
    vm::builder_virtual_stack_machine& vm() override { return bvm_; }

protected:
    void compile(lang::beng::parser_context const& ctx);

private:
    lang::beng::unit unit_;
    vm::builder_virtual_stack_machine bvm_;
};

}
