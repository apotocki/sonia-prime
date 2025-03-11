//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "struct_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/ast/expression_visitor.hpp"

#include "sonia/bang/ast/declaration_visitor.hpp"

#include "sonia/bang/errors/circular_dependency_error.hpp"

namespace sonia::lang::bang {

struct_entity::struct_entity(unit& u, functional& fn, variant<field_list_t, statement_span> const& body)
    : name_{ fn.name() }
    , body_{ body }
{
    sig_.name = fn.id();
    sig_.result.emplace(u.get(builtin_eid::typename_));
}

error_storage struct_entity::build(fn_compiler_context& extctx) const
{
    compiler_task_tracer::task_guard tg = extctx.try_lock_task(entity_task_id{ *this });
    if (!tg) {
        return make_error<circular_dependency_error>(make_error<basic_general_error>(location(), "build struct"sv, id()));
    }
    if (built_.load() == build_state::underlying_tuple_built) return {}; // double check

    unit& u = extctx.u();

    // prepare context
    fn_compiler_context ctx{ extctx, name_ };
    auto err = apply_visitor(make_functional_visitor<error_storage>([&ctx, this](auto const& body) -> error_storage {
        return build(ctx, body);
    }), body_);
    if (!err) {
        built_.store(build_state::underlying_tuple_built);
    }
    return err;
}

error_storage struct_entity::build(fn_compiler_context& ctx, field_list_t const& fl) const
{
    unit& u = ctx.u();

    entity_signature tuple_signature{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };
    for (field_t const& f : fl) {
        auto res = apply_visitor(ct_expression_visitor{ ctx }, f.type);
        if (!res) return std::move(res.error());
        bool is_const = f.modifier != field_modifier_t::value;
        if (f.name) {
            tuple_signature.push_back(f.name.value, field_descriptor{ *res, is_const });
        } else {
            tuple_signature.push_back(field_descriptor{ *res, is_const });
        }
    }

    indirect_signatured_entity smplsig{ tuple_signature };

    entity const& e = u.eregistry_find_or_create(smplsig, [&u, &tuple_signature]() {
        return make_shared<basic_signatured_entity>(std::move(tuple_signature));
    });

    underlying_tuple_eid_ = e.id();
    return {};
}

error_storage struct_entity::build(fn_compiler_context& ctx, statement_span const& sts) const
{
    declaration_visitor dvis{ ctx };
    if (auto err = dvis.apply(sts); err) return std::move(err);

    THROW_NOT_IMPLEMENTED_ERROR("struct_entity::build(statement_set_t)");
}

//error_storage struct_entity::build(fn_compiler_context& ctx, field_list_t const& fl) const
//{
//    unit& u = ctx.u();
//
//    pmd_ = make_shared<functional_match_descriptor>(u);
//    // no need to store the context expression state here
//    ctx.push_chain(pmd_->call_expressions);
//
//    //small_vector<std::tuple<field_t const&, syntax_expression_t>, 16> initializers; // [field, field name expression]
//    size_t pos_arg_num = 0;
//    entity_signature tuple_signature{ u.get(builtin_qnid::tuple) };
//    for (field_t const& f : fl) {
//        ct_expression_visitor evis{ ctx };
//        auto res = apply_visitor(evis, f.type);
//        if (!res) return std::move(res.error());
//        bool is_const = f.modifier != field_modifier_t::value;
//        if (f.name) {
//            tuple_signature.push_back(f.name.value, field_descriptor{ *res, is_const });
//        } else {
//            tuple_signature.push_back(field_descriptor{ *res, is_const });
//            if (!is_const) ++pos_arg_num;
//        }
//        if (!is_const) {
//            parameter_match_result* pmr = f.name ? &pmd_->get_match_result(f.name.value) : &pmd_->get_match_result(pos_arg_num - 1);
//            if (f.value) {
//                auto last_expr_it = ctx.expressions().last();
//                expression_visitor evis{ ctx, annotated_entity_identifier{ *res, get_start_location(f.type) } };
//                auto res = apply_visitor(evis, *f.value);
//                if (!res) return std::move(res.error());
//                pmr->append_result(false, ctx.context_type, last_expr_it, ctx.expressions());
//                // annotated_entity_identifier{ *res, get_start_location(f.type) }
//            }
//        }
//        else {
//            BOOST_ASSERT(!f.value);
//        }
//    }
//
//    indirect_signatured_entity smplsig{ tuple_signature };
//
//    entity const& e = u.eregistry_find_or_create(smplsig, [&u, &tuple_signature]() {
//        return make_shared<basic_signatured_entity>(u.get(builtin_eid::typename_), std::move(tuple_signature));
//    });
//
//    underlying_tuple_eid_ = e.id();
//    return {};
//}



//std::expected<functional::match, error_storage> struct_entity::find_init(fn_compiler_context& ctx, pure_call_t const& call) const
//{
//    if (built_.load() == build_state::not_built) {
//        if (auto err = build(ctx); err) return std::unexpected(std::move(err));
//    }
//    functional const* ifn = ctx.u().fregistry().find(name() / ctx.u().get(builtin_id::init));
//    if (!ifn) return std::unexpected(make_error<basic_general_error>(location(), "no constructuctor"sv, id()));
//    return ifn->find(ctx, call, annotated_entity_identifier{ id(), location() });
//}

std::expected<entity_identifier, error_storage> struct_entity::underlying_tuple_eid(fn_compiler_context& ctx) const
{
    if (built_.load() == build_state::not_built) {
        if (auto err = build(ctx); err) return std::unexpected(std::move(err));
    }
    return underlying_tuple_eid_;
}

//std::expected<functional_match_descriptor const*, error_storage> struct_entity::underlying_tuple_initializer(fn_compiler_context& ctx) const
//{
//    if (built_.load() == build_state::not_built) {
//        if (auto err = build(ctx); err) return std::unexpected(std::move(err));
//    }
//    return pmd_.get();
//}

}
