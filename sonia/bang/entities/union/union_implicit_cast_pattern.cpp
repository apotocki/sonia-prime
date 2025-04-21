//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "union_implicit_cast_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/base_expression_visitor.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

namespace sonia::lang::bang {

class union_cast_match_descriptor : public functional_match_descriptor
{
public:
    union_cast_match_descriptor(functional::match m, size_t i, entity_identifier rt)
        : submatch{ std::move(m) }
        , field_index{ i }
        , result_type{ rt }
    {}

    functional::match submatch;
    size_t field_index;
    entity_identifier result_type;
};

std::expected<functional_match_descriptor_ptr, error_storage> union_implicit_cast_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& e) const
{
    unit& u = ctx.u();
    if (!e) {
        return std::unexpected(make_error<basic_general_error>(call.location, "expected a vector result"sv));
    }
    entity const& ent = u.eregistry_get(e.value);
    entity_signature const* pusig = ent.signature();
    if (!pusig || pusig->name != u.get(builtin_qnid::union_)) {
        return std::unexpected(make_error<type_mismatch_error>(e.location, e.value, "a union"sv));
    }

    shared_ptr<union_cast_match_descriptor> pmd;

    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        auto const& argexpr = arg.value();
        
        if (pargname) { // should be void to proceed
            return std::unexpected(make_error<basic_general_error>(pargname ? pargname->location : get_start_location(argexpr), "argument mismatch"sv, argexpr));
            //auto res = apply_visitor(base_expression_visitor{ ctx }, argexpr);
            //if (!res) return std::unexpected(std::move(res.error()));
            //auto err = apply_visitor(make_functional_visitor<error_storage>([this, &ctx, pargname, &argexpr](auto & v) -> error_storage {
            //    if constexpr (std::is_same_v<semantic::managed_expression_list, std::decay_t<decltype(v)>>) {
            //        if (ctx.context_type == ctx.u().get(builtin_eid::void_)) return {}; // skip
            //    }
            //    return make_error<basic_general_error>(pargname ? pargname->location : get_start_location(argexpr), "argument mismatch"sv, argexpr);
            //}), res->first);
            //if (err) return std::unexpected(std::move(err));
            //continue; // skip void argument
        }
        
        if (pmd) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "argument mismatch"sv, argexpr));
        }

        pure_call_t cast_call{ e.location };
        cast_call.emplace_back(argexpr);
        optional<std::pair<functional::match, size_t>> rmatch;
        for (auto const& f : pusig->fields()) {
            auto match = ctx.find(builtin_qnid::implicit_cast, cast_call, annotated_entity_identifier{ f.entity_id(), e.location });
            if (!match) continue; // to do: collect errors?
            if (rmatch) {
                return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "ambiguous cast to union"sv, argexpr));
            }
            rmatch.emplace(std::move(*match), (size_t)(&f - &pusig->fields().front()));
        }
        if (!rmatch) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(argexpr), "no cast to union found"sv, argexpr));
        }
        pmd = make_shared<union_cast_match_descriptor>(std::move(rmatch->first), rmatch->second, e.value);
    }

    if (!pmd) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter(s)"sv));
    }
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> union_implicit_cast_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& ucmd = static_cast<union_cast_match_descriptor&>(md);
    auto res = ucmd.submatch.apply(ctx);
    if (!res) return std::unexpected(std::move(res.error()));
    auto& ser = *res;
    if (ser.is_const_result) {
        entity_signature usig{ u.get(builtin_qnid::metaobject), ucmd.result_type };
        usig.push_back(u.get(builtin_id::which), field_descriptor{ u.make_integer_entity(ucmd.field_index).id(), true });
        usig.push_back(field_descriptor{ ser.value(), true });
        indirect_signatured_entity smpl{ usig };
        return make_result(u, u.eregistry_find_or_create(smpl, [&u, &usig]() {
            return make_shared<basic_signatured_entity>(std::move(usig));
        }).id());
    } else {
        semantic::managed_expression_list rel{ u };
        rel.splice_back(el, (semantic::expression_span)ser.expressions);
        u.push_back_expression(rel, semantic::push_value{ ucmd.field_index }); // runtime which value
        u.push_back_expression(rel, semantic::push_value{ 2ull });
        u.push_back_expression(rel, semantic::invoke_function(u.get(builtin_eid::arrayify)));
        return syntax_expression_result_t{ std::move(rel), ucmd.result_type };
    }
}

}
