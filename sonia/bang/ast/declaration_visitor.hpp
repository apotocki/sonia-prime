//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include "sonia/small_vector.hpp"

#include "sonia/bang/semantic.hpp"
#include "sonia/bang/errors.hpp"

namespace sonia::lang::bang {

class functional;
class internal_function_entity;

class declaration_visitor : public static_visitor<error_storage>
{
    fn_compiler_context& ctx;
    internal_function_entity& fnent;
    mutable small_vector<statement_span, 4> decl_stack_;

public:
    inline explicit declaration_visitor(fn_compiler_context& c, internal_function_entity& fne) noexcept
        : ctx{ c }, fnent{ fne }
    {}

    [[nodiscard]] error_storage apply(statement_span) const;

    //void operator()(empty_t const&) const {}

    [[nodiscard]] result_type operator()(include_decl const&) const;

    [[nodiscard]] result_type operator()(extern_var const&) const;

    [[nodiscard]] result_type operator()(using_decl const&) const;

    [[nodiscard]] result_type operator()(struct_decl const&) const;
    [[nodiscard]] result_type operator()(enum_decl const&) const;

    [[nodiscard]] result_type operator()(fn_pure_t const&) const;

    [[nodiscard]] result_type operator()(fn_decl_t const&) const;

    [[nodiscard]] result_type operator()(if_decl const&) const;
    [[nodiscard]] result_type operator()(while_decl const&) const;
    [[nodiscard]] result_type operator()(for_decl const&) const;
    [[nodiscard]] result_type operator()(continue_statement_t const&) const;
    [[nodiscard]] result_type operator()(break_statement_t const&) const;

    [[nodiscard]] result_type operator()(let_statement const&) const;
    //void operator()(assign_decl_t const&) const;

    [[nodiscard]] result_type operator()(expression_statement_t const&) const;

    [[nodiscard]] result_type operator()(return_decl_t const&) const;

    template <typename T>
    result_type operator()(T const& d) const {
        THROW_NOT_IMPLEMENTED_ERROR("declaration_visitor");
    }

    void append_fnsig(fn_pure_t& /*in*/, functional** ppf = nullptr) const;
    
    //function_entity& append_fnent(fn_pure&, function_signature& sig, span<infunction_declaration_t>) const;

    //void operator()(type_decl const&) const { THROW_INTERNAL_ERROR(); }
    //void operator()(include_decl const&) const { THROW_INTERNAL_ERROR(); }

private:
    unit& u() const noexcept;

    // append_result is used to append the result of an expression evaluation to the current function context
    // returns the number of expressions appended
    [[nodiscard]] size_t append_result(semantic::expression_list_t&, syntax_expression_result_t&) const;

    // for the case when condition is a runtime evaluated expression
    [[nodiscard]] error_storage do_rt_if_decl(if_decl const&) const;
};

}
