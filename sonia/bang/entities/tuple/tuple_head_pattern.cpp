//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_head_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/expression_visitor.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/functional/generic_pattern_base.ipp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

// head( tuple(name0: value0, name1: value1, ...) ) => metaobject(name0, value0)
// head( tuple(const value0, name1: value1, ...) ) => const value0
// head( tuple(value0, name1: value1, ...) ) => value0

class tuple_head_match_descriptor : public functional_match_descriptor
{
public:
    inline explicit tuple_head_match_descriptor(unit& u, entity_signature const& sig, lex::resource_location loc) noexcept
        : result_sig{ u.get(builtin_qnid::tuple) }
    {
        field_descriptor const& head_field = sig.fields().front();
        if (head_field.name()) {
            result_sig.emplace_back(u.make_identifier_entity(head_field.name()).id, true);
        }
        result_sig.push_back(head_field);
        result_sig.result.emplace(u.get(builtin_eid::typename_));
        location = std::move(loc);
    }

    size_t src_size;
    entity_signature result_sig;
};

error_storage tuple_head_pattern::accept_argument(std::nullptr_t, functional_match_descriptor_ptr& pmd, arg_context_type & argctx, syntax_expression_result_t& er) const
{
    if (pmd || argctx.pargname) return argctx.make_argument_mismatch_error();

    fn_compiler_context& ctx = argctx.ctx;
    prepared_call const& call = argctx.call;
    unit& u = ctx.u();
    
    entity_identifier argtype;

    if (er.is_const_result) {
        entity const& arg_entity = get_entity(u, er.value());
        if (auto psig = arg_entity.signature(); psig && psig->name == u.get(builtin_qnid::tuple)) {
            // argument is typename tuple
            pmd = make_shared<tuple_head_match_descriptor>(u, *psig, call.location);
            return {};
        } else {
            argtype = arg_entity.get_type();
        }
    } else {
        argtype = er.type();
    }
            
    entity const& tpl_entity = get_entity(u, argtype);
    if (auto psig = tpl_entity.signature(); psig && psig->name == u.get(builtin_qnid::tuple)) {
        pmd = make_shared<tuple_head_match_descriptor>(u, *psig, call.location);
        auto& md = static_cast<tuple_head_match_descriptor&>(*pmd);

        field_descriptor const& head_field = psig->fields().front();
        if (head_field.is_const()) {
            return {};
        }

        // get non const fields count and index
        size_t fields_count = psig->fields().size();
        field_descriptor const* b_fd = &head_field, * e_fd = b_fd + fields_count;
        for (++b_fd; b_fd != e_fd; ++b_fd) { // first is not const, so skip it
            if (b_fd->is_const()) { --fields_count; }
        }
        md.src_size = fields_count;

        auto& mr = md.get_match_result(0);

        if (er.is_const_result) {
            mr.append_const_result(argtype, er.expressions);
        } else {
            mr.append_result(argtype, er.expressions);
        }
        call.splice_back(er.expressions);
        return {};
    }
    return argctx.make_argument_mismatch_error();
}

//std::expected<functional_match_descriptor_ptr, error_storage> tuple_head_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const& exp) const
//{
//    unit& u = ctx.u();
//    shared_ptr<tuple_head_match_descriptor> pmd;
//    for (auto const& arg : call.args()) {
//        annotated_identifier const* pargname = arg.name();
//        auto const& argexpr = arg.value();
//
//        auto res = apply_visitor(base_expression_visitor{ ctx }, argexpr);
//        if (!res) return std::unexpected(std::move(res.error()));
//        auto err = apply_visitor(make_functional_visitor<error_storage>([&ctx, &pmd, &argexpr, &call, pargname](auto& v) -> error_storage {
//            entity_identifier argtype;
//            if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
//                if (v == ctx.u().get(builtin_eid::void_)) {
//                    return make_error<type_mismatch_error>(get_start_location(argexpr), v, "not void"sv);
//                }
//            } else {
//                if (ctx.context_type == ctx.u().get(builtin_eid::void_)) return {}; // skip
//                argtype = ctx.context_type;
//            }
//
//            if (!pmd && !pargname) {
//                if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
//                    entity const& arg_entity = ctx.u().eregistry_get(v);
//                    if (auto psig = arg_entity.signature(); psig && psig->name == ctx.u().get(builtin_qnid::tuple)) {
//                        // argument is typename tuple
//                        pmd = make_shared<tuple_head_match_descriptor>(ctx.u(), *psig, call.location());
//                        return {};
//                    } else {
//                        argtype = arg_entity.get_type();
//                    }
//                }
//            
//                entity const& tpl_entity = ctx.u().eregistry_get(argtype);
//                if (auto psig = tpl_entity.signature(); psig && psig->name == ctx.u().get(builtin_qnid::tuple)) {
//                    pmd = make_shared<tuple_head_match_descriptor>(ctx.u(), *psig, call.location());
//                    field_descriptor const& head_field = psig->fields().front();
//                    if (head_field.is_const()) {
//                        return {};
//                    }
//
//                    // get non const fields count and index
//                    size_t fields_count = psig->fields().size();
//                    field_descriptor const* b_fd = &head_field, * e_fd = b_fd + fields_count;
//                    for (++b_fd; b_fd != e_fd; ++b_fd) { // first is not const, so skip it
//                        if (b_fd->is_const()) { --fields_count; }
//                    }
//                    pmd->src_size = fields_count;
//
//                    auto& mr = pmd->get_match_result(0);
//                    if constexpr (std::is_same_v<entity_identifier, std::decay_t<decltype(v)>>) {
//                        mr.append_result(argtype);
//                    } else {
//                        mr.append_result(argtype, v.end(), v);
//                        pmd->call_expressions.splice_back(v);
//                    }
//                    return {};
//                }
//            }
//            return make_error<basic_general_error>(pargname ? pargname->location : get_start_location(argexpr), "argument mismatch"sv, argexpr);
//            
//        }), res->first);
//        if (err) return std::unexpected(std::move(err));
//    }
//    if (!pmd) {
//        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched parameter"sv));
//    }
//    return pmd;
//}

std::expected<syntax_expression_result_t, error_storage> tuple_head_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& tmd = static_cast<tuple_head_match_descriptor&>(md);
    entity_identifier rtype;
    if (tmd.result_sig.fields().size() > 1) { // 'named' front field case
        indirect_signatured_entity smpl{ tmd.result_sig };
        entity& tplent = ctx.u().eregistry_find_or_create(smpl, [&u, &tmd]() {
            return make_shared<basic_signatured_entity>(std::move(tmd.result_sig));
        });

        entity_signature const& res_sig = *tplent.signature();
        if (std::ranges::find(res_sig.fields(), false, &field_descriptor::is_const) == res_sig.fields().end()) {
            empty_entity valueref{ tplent.id };
            entity& value_ent = ctx.u().eregistry_find_or_create(valueref, [&tplent]() {
                return make_shared<empty_entity>(tplent.id);
            });
            return make_result(u, value_ent.id);
        }
        rtype = tplent.id;
    } else if (tmd.result_sig.fields().front().is_const()) {
        return make_result(u, tmd.result_sig.fields().front().entity_id());
    } else {
        rtype = tmd.result_sig.fields().front().entity_id();
    }

    semantic::managed_expression_list exprs{ u };
    tmd.for_each_positional_match([&exprs, &el](parameter_match_result const& mr) {
        for (auto const& ser : mr.results) {
            exprs.splice_back(el, ser.expressions);
        }
    });

    if (tmd.src_size > 1) {
        u.push_back_expression(exprs, semantic::push_value{ mp::integer{ 0 } });
        u.push_back_expression(exprs, semantic::invoke_function(u.get(builtin_eid::array_at)));
    }

    return syntax_expression_result_t{ std::move(exprs), rtype };
}

template generic_pattern_base<tuple_head_pattern>;

//std::expected<entity_identifier, error_storage> tuple_head_pattern::const_apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
//{
//    auto res = apply(ctx, md);
//    if (!res) return std::unexpected(std::move(res.error()));
//    using result_t = std::expected<entity_identifier, error_storage>;
//    return apply_visitor(make_functional_visitor<result_t>([&ctx, &md](auto && eid_or_el) -> result_t {
//        if constexpr (std::is_same_v<std::decay_t<decltype(eid_or_el)>, entity_identifier>) {
//            return eid_or_el;
//        } else {
//            return std::unexpected(make_shared<basic_general_error>(md.location, "can't evaluate as a const expression"sv));
//        }
//    }), *res);
//}

}
