//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "../ast_terms.hpp"

namespace sonia::lang::beng {

class type_entity;

struct forward_declaration_visitor : static_visitor<void>
{
    fn_compiler_context& ctx;
    std::vector<type_entity*> types;

    explicit forward_declaration_visitor(fn_compiler_context& c) : ctx{ c } {}

    /*
    void operator()(empty_t const&) const {}

    void operator()(exten_var const& td) const;

    void operator()(expression_decl const& ed) const;

    void operator()(return_decl const& rd) const;

    void operator()(fn_pure_decl const& ed) const;

    void operator()(fn_decl_t const& ed) const;

    void operator()(let_statement_decl const& ld) const;
    */

    void operator()(enum_decl const& ed) const;

    void operator()(type_decl & td);

    template <typename T>
    void operator()(T const& d) const
    {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
};

}
