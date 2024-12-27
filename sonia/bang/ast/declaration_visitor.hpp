//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "../semantic.hpp"

namespace sonia::lang::bang {

class functional;
class function_entity;

struct declaration_visitor : static_visitor<void>
{
    fn_compiler_context& ctx;

    inline explicit declaration_visitor(fn_compiler_context& c) noexcept : ctx{ c } {}

    //void operator()(empty_t const&) const {}

    void operator()(extern_var const&) const;

    void operator()(struct_decl const&) const;

    void operator()(fn_pure_t const&) const;

    void operator()(fn_decl_t const&) const;

    void operator()(if_decl_t const&) const;
    void operator()(while_decl_t const&) const;
    void operator()(continue_statement_t const&) const;
    void operator()(break_statement_t const&) const;

    void operator()(let_statement const&) const;
    //void operator()(assign_decl_t const&) const;

    void operator()(expression_statement_t const&) const;

    void operator()(return_decl_t const&) const;

    void append_fnsig(fn_pure_t& /*in*/, functional** ppf = nullptr) const;
    
    //function_entity& append_fnent(fn_pure_t&, function_signature& sig, span<infunction_declaration_t>) const;

    //void operator()(type_decl const&) const { THROW_INTERNAL_ERROR(); }
    //void operator()(include_decl const&) const { THROW_INTERNAL_ERROR(); }

    unit& u() const noexcept;
};

}
