//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "lvalue_expression_visitor.hpp"
#include "fn_compiler_context.hpp"
#include "expression_visitor.hpp"
#include "../entities/variable_entity.hpp"
#include "../entities/type_entity.hpp"

namespace sonia::lang::bang {

//lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(annotated_qname const&) const
//{
//    THROW_NOT_IMPLEMENTED_ERROR("lvalue_expression_visitor annotated_qname");
//}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(variable_identifier const& v) const
{
    functional const* pfn = ctx.lookup_functional(v.name.value);
    if (pfn && pfn->default_entity()) {
        return &ctx.u().eregistry().get(pfn->default_entity());
    }
    return std::unexpected(make_error<undeclared_identifier_error>(v.name));
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(context_value const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("lvalue_expression_visitor context_value");
}

std::expected<entity const*, error_storage> lvalue_expression_visitor::handle_property_set(annotated_identifier id) const
{
    THROW_NOT_IMPLEMENTED_ERROR("lvalue_expression_visitor::handle_property_set");
#if 0
    if (auto const* po = sonia::get<bang_object_t>(&ctx.context_type); po) {
        if (auto const& pte = dynamic_cast<type_entity const*>(po->value); pte) {
            return pte->find_field_setter(ctx, id);
        } else {
            THROW_NOT_IMPLEMENTED_ERROR();
        }
    }
    
    return std::unexpected(make_error<left_not_an_object_error>(id.location, id.value, ctx.context_type));
#endif
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(bang_parameter_pack_t const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(not_empty_expression_t const& ne) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
    /*
    if (auto opterr = apply_visitor(expression_visitor{ ctx, nullptr }, me.value); opterr) {
        return std::unexpected(std::move(opterr));
    }
    if (auto* uotype = ctx.context_type.as<bang_union_t>(); uotype && uotype->has(bang_tuple_t{})) {
        ctx.append_expression(std::move(semantic::not_empty_condition_t{}));
        ctx.push_chain(get<semantic::not_empty_condition_t>(ctx.expressions().back()).branch);
        ctx.context_type = *uotype - bang_tuple_t{};
    }
    return std::unexpected(make_error<basic_general_error>(get_start_location(me), "the expression type is not optional"sv, me));
    */
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(member_expression_t const& me) const
{
    if (auto res = apply_visitor(expression_visitor{ ctx, nullptr }, me.object); !res) {
        return std::unexpected(std::move(res.error()));
    }
    /*
    if (auto* uotype = ctx.context_type.as<bang_union_t>(); me.is_object_optional && uotype && uotype->has(bang_tuple_t{})) {
        ctx.append_expression(std::move(semantic::not_empty_condition_t{}));
        ctx.push_chain(get<semantic::not_empty_condition_t>(ctx.expressions().back()).branch);
        ctx.context_type = *uotype - bang_tuple_t{};
    }
    */
    return handle_property_set(me.name);
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(property_expression const& pe) const
{
    return handle_property_set(pe.name);
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

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(annotated_integer const& ai) const
{
    return std::unexpected(make_error<wrong_lvalue_error>(ai));
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(annotated_decimal const& ad) const
{
    return std::unexpected(make_error<wrong_lvalue_error>(ad));
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(annotated_string const& as) const
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

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(unary_expression_t const& be) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(binary_expression_t const& be) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
    //return bang_binary_switcher(be, *this);
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(binary_operator_t<binary_operator_type::ASSIGN>, binary_expression_t const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(binary_operator_t<binary_operator_type::LOGIC_AND>, binary_expression_t const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(binary_operator_t<binary_operator_type::LOGIC_OR>, binary_expression_t const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(binary_operator_t<binary_operator_type::CONCAT>, binary_expression_t const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(binary_operator_t<binary_operator_type::PLUS>, binary_expression_t const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(binary_operator_t<binary_operator_type::EQ>, binary_expression_t const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(binary_operator_t<binary_operator_type::NE>, binary_expression_t const&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(chained_expression_t&) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(annotated_entity_identifier const& ee) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

lvalue_expression_visitor::result_type lvalue_expression_visitor::operator()(opt_named_syntax_expression_list_t const& el) const
{
    THROW_NOT_IMPLEMENTED_ERROR();
}

}
