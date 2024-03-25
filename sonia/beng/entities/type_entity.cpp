//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "type_entity.hpp"

#include "../ast/fn_compiler_context.hpp"
#include "../ast/expression_visitor.hpp"
#include "../ast/preliminary_type_visitor.hpp"

#include "sonia/utility/scope_exit.hpp"

namespace sonia::lang::beng {

void type_entity::treat(fn_compiler_context& ctx)
{
    if (handled_) return;
    if (handling_) {
        throw exception("%1%(%2%,%3%): `%4%`: recursive dependency"_fmt %
            location().resource % location().line % location().column %
            ctx.u().print(name()));
    }
    handling_ = true;
    signatures.emplace_back();
    auto& sig = signatures.back();

    sig.fn_type.result = beng_object_t{ this };
    sig.setup(ctx, direct_parameters);
    
    // to do: merge types for the same named arguments
    for (auto const& base : direct_bases) {
        shared_ptr<entity> e = ctx.resolve_entity(base.name);
        if (!e) [[unlikely]] {
            ctx.throw_undeclared_identifier(base.name, base.location);
        }
        shared_ptr<type_entity> type_ent = dynamic_pointer_cast<type_entity>(e);
        if (!type_ent) [[unlikely]] {
            throw exception("%1%(%2%,%3%): `%4%`: not a type identifier"_fmt %
                base.location.resource % base.location.line % base.location.column %
                ctx.u().print(base.name));
        }
        type_ent->treat(ctx);
        BOOST_ASSERT(1 == type_ent->signatures.size());
        auto const& basesig = type_ent->signatures.back();
        sig.position_parameters().insert(sig.position_parameters().end(), basesig.position_parameters().begin(), basesig.position_parameters().end());
        sig.named_parameters().insert(sig.named_parameters().end(), basesig.named_parameters().begin(), basesig.named_parameters().end());
        
        bases.emplace_back(type_ent.get());
        bases.insert(bases.end(), type_ent->bases.begin(), type_ent->bases.end());
    }

    sig.normilize(ctx);

    // to do: handle duplicated bases

    handling_ = false;
    handled_ = true;
}

bool type_entity::try_cast(fn_compiler_context& ctx,
    std::vector<semantic_expression_type>& result,
    beng_type const& rtype) const
{
    beng_object_t const* pot = get<beng_object_t>(&rtype);
    if (!pot) return false;
    if (pot->value == this) return true;
    return bases.end() != std::find(bases.begin(), bases.end(), pot->value);
}

bool type_entity::find(fn_compiler_context& ctx,
    span<const expression_t> positioned_args,
    span<const std::tuple<annotated_identifier, expression_t>> named_args,
    std::vector<semantic_expression_type>& result,
    beng_type & rtype) const
{
    if (!positioned_args.empty()) {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    
    size_t initial_result_sz = result.size();
    EXCEPTIONAL_SCOPE_EXIT([&result, initial_result_sz]() { result.resize(initial_result_sz); });

    function_signature const& sig = signatures.back();
    size_t posargpos = 0;

    for (auto const& narg : named_args) {
        auto const& argname = std::get<0>(narg);
        auto it = std::ranges::lower_bound(sig.named_parameters(), argname.id, [](auto const& l, auto const& r) { return l < r; }, [](auto const& v) { return std::get<0>(v).id; });
        if (it == sig.named_parameters().end() || std::get<0>(*it).id != argname.id) {
            throw exception("%1%(%2%,%3%): parameter `%4%` of `%5%` is not found"_fmt %
                argname.location.resource % argname.location.line % argname.location.column %
                ctx.u().print(argname.id) % ctx.u().print(name()));
        }
        expression_visitor evis{ ctx, result, &std::get<1>(*it) };
        apply_visitor(evis, std::get<1>(narg));
        result.emplace_back(semantic::push_value{ ctx.u().as_u32string(argname.id) });
    }
    result.emplace_back(semantic::push_value{ decimal{ named_args.size() } });
    result.emplace_back(semantic::push_value{ ctx.u().as_u32string(name()) });
    result.emplace_back(semantic::invoke_function{ name_ /*, 2 * ((uint32_t)named_args.size()) + 1*/ });
    rtype = beng_object_t{ this };
    return true;

    // ids: check uniqueness?
    //std::ranges::sort(std::views::zip(ids, args_exprs), {}, [](auto const& pair) { return std::get<0>(pair); });
    //uint32_t argcount = (uint32_t)ids.size();
    /*
    auto it = funcions_.find(span{ ids }, hasher{}, std::equal_to<>{});
    if (it == funcions_.end()) {
        auto fn = make_shared<implemented_function>();
        for (auto argit = ids.rbegin(), argeit = ids.rend(); argit != argeit; ++argit) {
            
        }
        for (identifier argid : ids) {
            fn->body.emplace_back(semantic::push_value{ ctx.u().as_u32string() });
        }

        ctx.u().put_function(fn);
        function_descriptor fd;
        fd.param_names = std::move(ids);
        fd.fn = std::move(fn);
        it = funcions_.insert(it, std::move(fd));
    }

    semantic::invoke_function inv_fn{ it->fn.get(), argcount };
    args_exprs.emplace_back(std::move(inv_fn));
    result.emplace_back(std::move(args_exprs), beng_object_t{ name() });
    */
}

}
