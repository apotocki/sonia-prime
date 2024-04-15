//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "lvalue_expression_visitor.hpp"
#include "fn_compiler_context.hpp"
#include "expression_visitor.hpp"
#include "../entities/variable_entity.hpp"
#include "../entities/type_entity.hpp"

namespace sonia::lang::beng {

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(variable_identifier const& v) const
{
    if (auto optvar = ctx.resolve_variable(v.name); optvar) {
        return &*optvar;
    }
    return std::unexpected(make_error<undeclared_identifier_error>(v.location, v.name));
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(member_expression_t & me) const
{
    auto otype = apply_visitor(expression_visitor{ ctx, nullptr }, me.object);
    if (!otype.has_value()) return std::unexpected(std::move(otype.error()));
    if (auto* uotype = otype.value().as<beng_union_t>(); me.is_object_optional && uotype && uotype->has(beng_tuple_t{})) {
        ctx.append_expression(std::move(semantic::not_empty_condition_t{}));
        ctx.push_chain(get<semantic::not_empty_condition_t>(ctx.expressions().back()).branch);
        otype = *uotype - beng_tuple_t{};
    }
    if (auto const* po = sonia::get<beng_object_t>(&*otype); po) {
        if (auto const& pte = dynamic_cast<type_entity const*>(po->value); pte) {
            return pte->find_field_setter(ctx, me.name);
        } else {
            THROW_NOT_IMPLEMENTED_ERROR();
        }
    } else {
        return std::unexpected(make_error<left_not_an_object_error>(me.name.location, me.name.value, otype.value()));
    }
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(lambda_t const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(case_expression const& ce) const
{
    return std::unexpected(make_error<wrong_lvalue_error>(ce));
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(annotated_bool const& ab) const
{
    return std::unexpected(make_error<wrong_lvalue_error>(ab));
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(annotated_decimal const& ad) const
{
    return std::unexpected(make_error<wrong_lvalue_error>(ad));
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(annotated_u32string const& as) const
{
    return std::unexpected(make_error<wrong_lvalue_error>(as));
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(expression_vector_t const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(function_call_t const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(unary_expression_t<unary_operator_type::NEGATE> const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(binary_expression_t<binary_operator_type::ASSIGN> const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(binary_expression_t<binary_operator_type::LOGIC_AND> const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(binary_expression_t<binary_operator_type::LOGIC_OR> const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(binary_expression_t<binary_operator_type::CONCAT>&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(chained_expression_t&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

}
