//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "signature_matcher.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

namespace sonia::lang::bang {

signature_matcher_visitor::signature_matcher_visitor(fn_compiler_context & ctx, functional_binding& bs, entity_identifier eid)
    : ctx_{ ctx }
    , binding_{ bs }
    , eid_ { eid }
{

}

signature_matcher_visitor::result_type signature_matcher_visitor::operator()(variable_identifier const& var) const
{
    auto opteid = ctx_.lookup_entity(var.name);
    if (!opteid) return std::move(opteid.error());
    if (!*opteid) {
        if (var.implicit) { // var.name.value.is_relative() && var.name.value.size() == 1) {
            // bind variable
            BOOST_ASSERT(var.name.value.is_relative() && var.name.value.size() == 1);
            identifier varid = *var.name.value.begin();
            binding_.emplace_back(annotated_identifier{ varid, var.name.location }, eid_);
            return {};
        }
        return make_error<undeclared_identifier_error>(var.name);
    }

    if (*opteid != eid_) {
        return make_error<basic_general_error>(var.name.location, "argument mismatch"sv, var);
    }
    return {};
}

#if 0
signature_matcher_visitor::result_type signature_matcher_visitor::operator()(function_call_t const& sigp) const
{
    unit& u = ctx_.u();
    ct_expression_visitor sv{ ctx_, annotated_entity_identifier{ u.get(builtin_eid::qname) } };
    auto qn_ent_id = apply_visitor(sv, sigp.fn_object);
    if (!qn_ent_id) return std::unexpected(std::move(qn_ent_id.error()));
    qname_identifier_entity qname_ent = static_cast<qname_identifier_entity const&>(u.eregistry_get(*qn_ent_id));
    functional const& fn_fl = u.fregistry().resolve(qname_ent.value());

    // check if can evaluate signature_pattern_
    auto match = fn_fl.find(ctx_, sigp);
    if (match) {
        if (auto result = match->const_apply(ctx_); result) {
            expression_visitor evis{ ctx_, annotated_entity_identifier{ *result, sigp.location() } };
            return apply_visitor(evis, expr_);
        }
    }

    // can't evaluate signature_pattern_ as a function, consider as a pattern
    expression_visitor evis{ ctx_ };
    auto res = apply_visitor(evis, expr_);
    if (!res) return std::unexpected(std::move(res.error()));

    entity const& type_ent = u.eregistry_get(ctx_.context_type);
    entity_signature const* psig = type_ent.signature();
    if (!psig) {
        return std::unexpected(make_error<basic_general_error>(sigp.location(), "argument mismatch"sv, expr_));
    }
    if (qname_ent.value() != psig->name()) {
        return std::unexpected(make_error<basic_general_error>(sigp.location(), "argument mismatch"sv, expr_));
    }

    size_t arg_index = 0;
    for (auto const& arg : sigp.args()) {
        annotated_identifier const* argname = arg.name();
        field_descriptor const* pfd = argname ? psig->find_field(argname->value) : psig->find_field(arg_index++);
        if (!pfd) {
            return std::unexpected(make_error<basic_general_error>(sigp.location(), "argument pattern mismatch"sv, expr_));
        }
    }

    //if (auto eid = apply_visitor(ct_expression_visitor{ ctx }, signature_pattern_.fn_object);

    THROW_NOT_IMPLEMENTED_ERROR("value_type_constraint_match_visitor(function_call_t) not implemented expression");
}
#endif
//std::expected<bool, error_storage> signature_matcher_visitor::try_match(fn_compiler_context& ctx, syntax_expression_t const& expr) const
//{
//        
//}

}
