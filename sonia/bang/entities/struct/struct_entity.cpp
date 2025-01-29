//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "struct_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/errors/circular_dependency_error.hpp"

namespace sonia::lang::bang {

error_storage struct_entity::build(fn_compiler_context& extctx) const
{
    compiler_task_tracer::task_guard tg = extctx.try_lock_task(entity_task_id{ *this });
    if (!tg) {
        return make_error<circular_dependency_error>(make_error<basic_general_error>(location(), "build struct"sv, id()));
    }
    if (built_.load()) return {};

    unit& u = extctx.u();

    // prepare context

    // get parent qname of sig_.name
    qname_identifier cur_qn = sig_.name();
    qname_view parent_qn = u.fregistry().resolve(cur_qn).name().parent();

    fn_compiler_context ctx{ extctx, parent_qn };
    
    entity_signature tuple_signature{ u.get(builtin_qnid::tuple) };
    for (field_t const& f : fields_) {
        ct_expression_visitor evis{ ctx };
        auto res = apply_visitor(evis, f.type);
        if (!res) return std::move(res.error());
        bool is_const = f.modifier != parameter_constraint_modifier_t::value_type_constraint;
        if (f.name) {
            tuple_signature.set(f.name.value, field_descriptor{ *res, is_const });
        } else {
            tuple_signature.push_back(field_descriptor{ *res, is_const });
        }
    }

    indirect_signatured_entity smplsig{ tuple_signature };

    entity const& e = u.eregistry_find_or_create(smplsig, [&u, &tuple_signature]() {
        return make_shared<basic_signatured_entity>(u.get(builtin_eid::typename_), std::move(tuple_signature));
    });

    underlying_tuple_eid_ = e.id();
    
    //for (auto& f : fields_) {
        //auto& fent = ctx.u().resolve_functional(f.entity_id());
        //if (f.is_const()) {
        //    fent.set_const();
        //}
    //}
    built_.store(true);
    return {};
}

std::expected<entity_identifier, error_storage> struct_entity::underlying_tuple_eid(fn_compiler_context& ctx) const
{
    if (!built_.load()) {
        if (auto err = build(ctx); err) return std::unexpected(std::move(err));
    }
    return underlying_tuple_eid_;
}

std::expected<entity_identifier, error_storage> struct_entity::underlying_tuple_constructor_eid(fn_compiler_context& ctx) const
{
    if (!built_.load()) {
        if (auto err = build(ctx); err) return std::unexpected(std::move(err));
    }
    THROW_NOT_IMPLEMENTED_ERROR("struct_entity::underlying_tuple_constructor_eid");
    return underlying_tuple_constructor_eid_;
}

}
