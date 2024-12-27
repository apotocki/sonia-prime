//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "assign_expression_visitor.hpp"
#include "fn_compiler_context.hpp"
#include "expression_visitor.hpp"
//#include "../entities/variable_entity.hpp"
//#include "../entities/type_entity.hpp"

namespace sonia::lang::bang {

inline unit& assign_expression_visitor::u() const noexcept { return ctx_.u(); }

assign_expression_visitor::result_type assign_expression_visitor::operator()(variable_identifier const& v) const
{
    functional const* pfn = ctx_.lookup_functional(v.name.value);
    if (!pfn) return make_error<undeclared_identifier_error>(v.name);
    if (!pfn->default_entity()) return make_error<assign_error>(assign_location_, syntax_expression_t{ v });
    
    entity const& ent = u().eregistry().get(pfn->default_entity());
    if (variable_entity const* ve = dynamic_cast<variable_entity const*>(&ent); ve) {
        expression_visitor rvis{ ctx_, annotated_entity_identifier{ ve->get_type(), assign_location_ } };
        auto res = apply_visitor(rvis, rhs_);
        if (!res) return std::move(res.error());
        if (ve->is_weak()) {
            THROW_NOT_IMPLEMENTED_ERROR("expression_visitor binary_operator_type::ASSIGN");
            //ctx.append_expression(semantic::invoke_function{ ctx.u().get_builtin_function(unit::builtin_fn::weak_create) });
        }
        ctx_.append_expression(semantic::set_variable{ ve });
        if (ve->is_weak()) {
            ctx_.append_expression(semantic::truncate_values(1, false));
        }
        return {};
    }
    return make_error<assign_error>(assign_location_, syntax_expression_t{ v });
}

assign_expression_visitor::result_type assign_expression_visitor::operator()(member_expression_t const& me) const
{
    //if (auto res = apply_visitor(expression_visitor{ ctx_ }, me.object); !res) {
    //    return std::move(res.error());
    //}

    functional const& fn = u().fregistry().resolve(u().get_set_qname_identifier());
    pure_call_t set_call{ assign_location_ };
    set_call.emplace_back(annotated_identifier{ u().get_self_parameter_identifier() }, me.object);
    set_call.emplace_back(annotated_identifier{ u().get_property_parameter_identifier() }, me.property );
    set_call.emplace_back(rhs_);

    auto match = fn.find(ctx_, set_call);
    if (!match) {
        return append_cause(
            make_error<basic_general_error>(assign_location_, "can't assign"sv, syntax_expression_t{ me }),
            std::move(match.error())
        );
    }

    return match->apply(ctx_);
    
    /*
    if (auto* uotype = ctx.context_type.as<bang_union_t>(); me.is_object_optional && uotype && uotype->has(bang_tuple_t{})) {
        ctx.append_expression(std::move(semantic::not_empty_condition_t{}));
        ctx.push_chain(get<semantic::not_empty_condition_t>(ctx.expressions().back()).branch);
        ctx.context_type = *uotype - bang_tuple_t{};
    }
    */
    THROW_NOT_IMPLEMENTED_ERROR("lvalue_expression_visitor::handle_property_set");
    //return handle_property_set(me.property);
}

#if 0
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
#endif

}
