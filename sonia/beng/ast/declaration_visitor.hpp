//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "../semantic.hpp"

namespace sonia::lang::beng {

struct declaration_visitor : static_visitor<void>
{
    fn_compiler_context& ctx;

    explicit declaration_visitor(fn_compiler_context& c) : ctx{ c } {}

    //void operator()(empty_t const&) const {}

    void operator()(exten_var & td) const;

    void operator()(fn_pure_decl & ed) const;

    void operator()(fn_decl_t & ed) const;

    void operator()(let_statement_decl & ld) const;
    
    void operator()(expression_decl & ed) const;

    void operator()(return_decl & rd) const;

    function_signature& append_fnsig(fn_pure_decl & fd) const;
};

}
