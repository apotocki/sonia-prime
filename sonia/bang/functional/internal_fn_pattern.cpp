//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "internal_fn_pattern.hpp"

#include "sonia/bang/unit.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/entities/functions/internal_function_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/errors/circular_dependency_error.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

error_storage internal_fn_pattern::init(fn_compiler_context& ctx, fn_decl_t const& fnd)
{
    body_ = fnd.body;
    kind_ = fnd.kind;
    return basic_fn_pattern::init(ctx, fnd);
    // if (auto err = init(ctx, static_cast<fn_pure_t const&>(fnd))) return err;
}

shared_ptr<entity> internal_fn_pattern::build(fn_compiler_context& ctx, functional_match_descriptor& md, entity_signature&& signature) const
{
    unit& u = ctx.u();

    qname_view fnqn = u.fregistry_resolve(signature.name).name();
    qname fn_ns = fnqn / u.new_identifier();

    auto pife = make_shared<internal_function_entity>(
        std::move(fn_ns),
        std::move(signature),
        /*std::move(md.bindings),*/ body_);

    pife->location = location();
    build_scope(u, md, *pife);

    pife->set_inline(kind_ == fn_kind::INLINE);

    return pife;
}

void internal_fn_pattern::build_scope(unit& u, functional_match_descriptor& md, internal_function_entity& fent) const
{
    // bind variables (rt arguments)
    for (parameter_descriptor const& pd : parameters_) {
        functional_binding::value_type const* bsp = md.bindings.lookup(pd.inames.front().value);
        BOOST_ASSERT(bsp);
        
        if (!has(pd.modifier, parameter_constraint_modifier_t::ellipsis)) {
            if (local_variable const* plv = get<local_variable>(bsp); plv) {
                fent.push_argument(plv->varid);
            } // else arg is constant
            continue;
        }

        entity_identifier const* peid = get<entity_identifier>(bsp);
        if (!peid) {
            THROW_INTERNAL_ERROR("internal_fn_pattern::build_scope: expected entity_identifier in functional_binding::value_type for ellipsis");
        }

        entity const& ellipsis_unit = get_entity(u, *peid);
        entity const& ellipsis_unit_type = get_entity(u, ellipsis_unit.get_type());
        entity_signature const* pellipsis_sig = ellipsis_unit_type.signature();
        BOOST_ASSERT(pellipsis_sig && pellipsis_sig->name == u.get(builtin_qnid::tuple));
        for (field_descriptor const& fd : pellipsis_sig->fields()) {
            BOOST_ASSERT(fd.is_const());
            entity const& fd_ent = get_entity(u, fd.entity_id());
            if (qname_entity const* pqent = dynamic_cast<qname_entity const*>(&fd_ent); pqent) {
                qname const& qn = pqent->value();
                BOOST_ASSERT(qn.size() == 1);
                identifier varname = qn.parts().front();
                functional_binding::value_type const* bvar = md.bindings.lookup(varname);
                BOOST_ASSERT(bvar);
                local_variable const* plv = get<local_variable>(bvar);
                BOOST_ASSERT(plv);
                fent.push_argument(plv->varid);
            }
        }
    }
    //for (auto& [argindex, mr] : md.matches) {
    //    parameter_descriptor const& pd = parameters_[argindex];
    //    functional_binding::value_type const* bsp = md.bindings.lookup(pd.inames.front().value);
    //    BOOST_ASSERT(bsp);
    //    if (local_variable const* plv = get<local_variable>(bsp); plv) {
    //        fent.push_argument(plv->varid);
    //    } else {
    //        THROW_INTERNAL_ERROR("internal_fn_pattern::build_scope: expected local_variable in functional_binding::value_type");
    //    }
    //}
    
    // ? bind constants ?
#if 0
    // bind consts
    md.bindings.for_each([&u, &fent](identifier name, lex::resource_location const& loc, functional_binding::value_type & value) {
        entity_identifier eid = apply_visitor(make_functional_visitor<entity_identifier>([&u](auto const& e) {
            if constexpr (std::is_same_v<std::decay_t<decltype(e)>, entity_identifier>) {
                return e;
            } else if constexpr (std::is_same_v<std::decay_t<decltype(e)>, entity_ptr>) {
                if (e->id) return e->id;
                //if (e->get_type() == u.get(builtin_eid::metaobject)) {
                //    return u.eregistry_find_or_create(*e, [&e]() { return std::move(e); }).id();
                //}
                return u.eregistry_find_or_create(*e, [&e]() { return std::move(e); }).id;
            } else { // else skip variables
                return entity_identifier{};
            }
        }), value);

        if (eid) {
            qname infn_name = fent.name() / name;
            functional& fnl = u.fregistry_resolve(infn_name);
            fnl.set_default_entity(annotated_entity_identifier{ eid, loc });
        }
    });
#endif
    fent.bindings = std::move(md.bindings);
}

std::expected<syntax_expression_result_t, error_storage> internal_fn_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto [result, mut_arg_cnt] = apply_arguments(ctx, el, md);

    indirect_signatured_entity smpl{ md.signature };

    entity& e = u.eregistry_find_or_create(smpl, [this, &ctx, &md]() {
        return build(ctx, md, std::move(md.signature));
    });

    BOOST_ASSERT(dynamic_cast<internal_function_entity*>(&e));
    internal_function_entity& fne = static_cast<internal_function_entity&>(e);

    if (!fne.result) { // we need building to resolve result type
        compiler_task_tracer::task_guard tg = ctx.try_lock_task(entity_task_id{ e });
        if (!tg) return std::unexpected(
            make_error<circular_dependency_error>(make_error<basic_general_error>(location_, "resolving function result type"sv, e.id))
        );
        if (!fne.result) {
            if (auto err = fne.build(u)) {
                return std::unexpected(std::move(err));
            }
        }

        BOOST_ASSERT(fne.result);
    }

    result.value_or_type = fne.result.entity_id();
    result.is_const_result = fne.result.is_const();

    if (mut_arg_cnt || !fne.is_const_eval(u)) {
        u.push_back_expression(el, result.expressions, semantic::invoke_function(e.id));
        return result;
    }

    return result;
}

}
