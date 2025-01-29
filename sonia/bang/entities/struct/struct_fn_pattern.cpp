//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "struct_fn_pattern.hpp"
#include "struct_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/errors/circular_dependency_error.hpp"

namespace sonia::lang::bang {

struct_fn_pattern::struct_fn_pattern(fn_compiler_context& ctx, functional const& fnl, fn_pure const& fnd, field_list_t const& fds)
    : basic_fn_pattern{ ctx, fnl, fnd }
    , fields_{ fds }
{ }

#if 0
std::expected<entity_identifier, error_storage> struct_fn_pattern::get_underlying_tuple_eid(fn_compiler_context& callee_ctx, qname_identifier fid) const
{
    {
        lock_guard lock{ mtx_ };
        if (underlying_tuple_eid_) return underlying_tuple_eid_;
    }

    if (auto err = get_underlying(callee_ctx, fid); err) return std::unexpected(std::move(err));

    return underlying_tuple_eid_;
}


std::expected<entity_identifier, error_storage> struct_fn_pattern::get_underlying_tuple_constructor_eid(fn_compiler_context& callee_ctx, qname_identifier fid) const
{
    {
        lock_guard lock{ mtx_ };
        if (underlying_tuple_constructor_eid_) return underlying_tuple_constructor_eid_;
    }

    if (auto err = get_underlying(callee_ctx, fid); err) return std::unexpected(std::move(err));

    return underlying_tuple_constructor_eid_;
}
#endif

#if 0
error_storage struct_fn_pattern::get_underlying(fn_compiler_context& callee_ctx, qname_identifier fid) const
{
    {
        lock_guard lock{ mtx_ };
        if (underlying_tuple_eid_) return {};
    }

    compiler_task_tracer::task_guard tg = callee_ctx.try_lock_task(qname_task_id{ fid });
    if (!tg) {
        throw circular_dependency_error({ make_error<basic_general_error>(location(), "resolving underlying tuple type"sv, fid) });
    }

    {
        // double check
        lock_guard lock{ mtx_ };
        if (underlying_tuple_eid_) return {};
    }

    unit& u = callee_ctx.u();

    // build underlying tuple type
    fn_compiler_context ctx{ callee_ctx.u() };
    auto tpleid = struct_utility::build_tuple(ctx, fields_);
    if (!tpleid) return std::move(tpleid.error());

    {
        lock_guard lock{ mtx_ };
        underlying_tuple_eid_ = *tpleid;
    }
    return {};
}
#endif

std::expected<entity_identifier, error_storage> struct_fn_pattern::const_apply(fn_compiler_context& ctx, qname_identifier fid, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    entity_signature sig = md.build_signature(u, fid);
    indirect_signatured_entity smpl{ sig };

    return u.eregistry_find_or_create(smpl, [this, &ctx, &sig]() {
        return make_shared<struct_entity>(ctx.u().get(builtin_eid::typename_), std::move(sig), fields_);
    }).id();

    //struct_entity const& se = static_cast<struct_entity const&>(u.eregistry_find_or_create(smpl, [this, &ctx, &sig]() {
    //    return make_shared<struct_entity>(ctx.u().get(builtin_eid::typename_), std::move(sig), fields_);
    //}));

    //se.build(ctx);
    //return se.id();
}

//void struct_fn_pattern::build(fn_compiler_context& callee_ctx, qname_identifier fid)
//{
//    
//}

}
