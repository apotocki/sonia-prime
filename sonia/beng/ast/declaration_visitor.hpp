//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "terms.hpp"
#include "../ast.hpp"

namespace sonia::lang::beng {

struct declaration_visitor : static_visitor<void>
{
    compiler_context& ctx;

    explicit declaration_visitor(compiler_context& c) : ctx{ c } {}

    void operator()(empty_t const&) const {}

    void operator()(exten_var const& td) const;

    void operator()(type_decl const& td) const;
    
    void operator()(enum_decl const& ed) const;
    
    void operator()(fn_pure_decl const& ed) const;

    void operator()(fn_decl_t const& ed) const;

    void operator()(let_statement_decl const& ld) const;
    
    void operator()(expression_decl const& ed) const;

    void operator()(return_decl const& rd) const;

    void check_name(qname const& name) const;

private:
    function_signature& append_fnsig(fn_pure_decl const& fd) const;
};

}
