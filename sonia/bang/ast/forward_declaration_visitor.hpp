//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "../ast_terms.hpp"

namespace sonia::lang::bang {

class type_entity;
class parser_context;
class fn_compiler_context;

struct forward_declaration_visitor : static_visitor<void>
{
    fn_compiler_context& ctx;
    parser_context& pctx;
    std::vector<infunction_statement> decls;
    std::vector<type_entity*> types;

    explicit forward_declaration_visitor(fn_compiler_context& c, lang::bang::parser_context& p) : ctx{ c }, pctx{ p } {}

    void operator()(include_decl&);

    void operator()(enum_decl&);

    void operator()(external_struct_decl&);

    void operator()(type_decl&);

    void operator()(extern_var&) { THROW_NOT_IMPLEMENTED_ERROR("extern_var"); }

    template <typename T>
    void operator()(T & d)
    {
        //infunction_declaration_adopt_visitor vis{};
        //decls.emplace_back(apply_visitor(vis, d));
        decls.emplace_back(std::move(d));
    }
};

}
