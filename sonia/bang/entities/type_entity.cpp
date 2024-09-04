//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "type_entity.hpp"

#include "../ast/fn_compiler_context.hpp"
#include "../ast/expression_visitor.hpp"
#include "../ast/preliminary_type_visitor.hpp"

namespace sonia::lang::bang {

#if 0
void type_entity2::treat(fn_compiler_context& ctx)
{
    THROW_NOT_IMPLEMENTED_ERROR("ype_entity::treat");
#if 0
    if (handled_) return;
    if (handling_) {
        throw exception(ctx.u().print(basic_general_error{location(), "recursive dependency"sv, name()}));
    }
    handling_ = true;
    signatures.emplace_back();
    auto& sig = signatures.back();

    sig.fn_type.result = bang_object_t{ this };
    sig.setup(ctx, direct_parameters);
    
    // to do: merge types for the same named arguments
    for (auto const& base : direct_bases) {
        shared_ptr<entity> e = ctx.resolve_entity(base.value);
        if (!e) [[unlikely]] {
            throw exception(ctx.u().print(undeclared_identifier_error{base.location, base.value}));
        }
        shared_ptr<type_entity> type_ent = dynamic_pointer_cast<type_entity>(e);
        if (!type_ent) [[unlikely]] {
            throw exception(ctx.u().print(basic_general_error{base.location, "not a type identifier"sv, base.value}));
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
#endif
}

bool type_entity2::try_cast(fn_compiler_context& ctx, bang_type const& rtype) const
{
    THROW_NOT_IMPLEMENTED_ERROR("type_entity::try_cast");
#if 0
    bang_object_t const* pot = get<bang_object_t>(&rtype);
    if (pot && (pot->value == this || bases.end() != std::find(bases.begin(), bases.end(), pot->value))) {
        ctx.context_type = rtype;
        return true;
    }
    return false;
#endif
}

std::expected<function_signature const*, error_storage> type_entity2::find(fn_compiler_context& ctx, pure_call_t& call) const
{
    THROW_NOT_IMPLEMENTED_ERROR("type_entity find");
#if 0
    if (!call.positioned_args.empty()) {
        THROW_NOT_IMPLEMENTED_ERROR();
    }
    
    auto estate = ctx.expressions_state();

    ctx.append_expression(semantic::push_value{ ctx.u().as_string(name()) }); // type
    ctx.append_expression(ctx.u().get_builtin_function(unit::builtin_fn::extern_object_create));

    function_signature const& sig = signatures.back();
    size_t posargpos = 0;

    for (auto & narg : call.named_args) {
        ctx.append_expression(semantic::push_by_offset{ 0 }); // duplicate object on stack (because 'set property' absorbs object )
        auto const& argname = std::get<0>(narg);
        auto argparts = argname.value.parts();
        // build property assignment
        ctx.context_type = bang_object_t{ this };
        expression_t left_expr = property_expression{ annotated_identifier{ argparts[0], argname.location } };
        for (size_t partidx = 1; partidx < argparts.size(); ++partidx) {
            left_expr = member_expression_t{ std::move(left_expr), annotated_identifier{ argparts[partidx], argname.location } };
        }
        binary_expression_t aex{ binary_operator_type::ASSIGN, std::move(left_expr), std::get<1>(narg), std::get<2>(narg) };
        expression_visitor evis{ ctx, nullptr };
        if (auto opterr = evis(aex); opterr) return std::unexpected(std::move(opterr));
        
        ctx.append_expression(semantic::truncate_values(1, false)); // remove property value from stack
#if 0
        if (argparts.size() > 1) {
            assign_expression_t aex{left, std::get<1>(narg) }
            identifier id = argparts[0];
            auto expgetter = find_field_getter(ctx, annotated_identifier{ id, argname.location });
            if (!expgetter.has_value()) return std::unexpected(std::move(expgetter.error()));
            function_entity const* getter = expgetter.value();
            ctx.append_expression(semantic::invoke_function{ getter->name() });
            bang_type proptype = getter->result_type();

            for (size_t partidx = 1; partidx < argparts.size() - 1; ++partidx) {
                identifier id = argparts[partidx];
                expression_visitor evis{ ctx, nullptr };
                expgetter = evis.handle_property_get(proptype, annotated_identifier{ id, argname.location });
                if (!expgetter.has_value()) return std::unexpected(std::move(expgetter.error()));
                function_entity const* getter = expgetter.value();
                proptype = getter->result_type();
            }
            
        }

        auto it = std::ranges::lower_bound(sig.named_parameters(), argname.value, {} /*[](auto const& l, auto const& r) { return l < r; }*/, [](auto const& v) { return qname{std::get<0>(v).value}; });
        if (it == sig.named_parameters().end() || qname{std::get<0>(*it).value} != argname.value) {
            annotated_qname_identifier aqnid{ ctx.u().qnregistry().resolve(argname.value), argname.location };
            return std::unexpected(make_error<parameter_not_found_error>(name(), aqnid));
        }
        expression_visitor evis{ ctx, expected_result_t{ std::get<1>(*it), std::get<0>(*it).location } };
        if (auto rtype = apply_visitor(evis, std::get<1>(narg)); !rtype.has_value()) return rtype;
        ctx.append_expression(semantic::push_value{ ctx.u().as_string(argname.value) });
#endif
    }
    //ctx.append_expression(semantic::push_value{ decimal{ call.named_args.size() } });
    //ctx.append_expression(semantic::push_value{ ctx.u().as_string(name()) });
    //ctx.append_expression(semantic::invoke_function{ name_ });
    estate.detach();
    ctx.context_type = bang_object_t{ this };
    return &sig;

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
            fn->body.emplace_back(semantic::push_value{ ctx.u().as_string() });
        }

        ctx.u().put_function(fn);
        function_descriptor fd;
        fd.param_names = std::move(ids);
        fd.fn = std::move(fn);
        it = funcions_.insert(it, std::move(fd));
    }

    semantic::invoke_function inv_fn{ it->fn.get(), argcount };
    args_exprs.emplace_back(std::move(inv_fn));
    result.emplace_back(std::move(args_exprs), bang_object_t{ name() });
    */
#endif
}

std::expected<function_entity const*, error_storage> type_entity2::find_field_getter(fn_compiler_context& ctx, annotated_identifier const& f) const
{
    THROW_NOT_IMPLEMENTED_ERROR("type_entity find_field_getter");
#if 0
    qname fnname = ctx.u().qnregistry().resolve(name_) / f.value / ctx.u().slregistry().resolve("g"sv);
    qname_identifier fnnameid = ctx.u().qnregistry().resolve(fnname);
    if (auto pfn = dynamic_pointer_cast<function_entity>(ctx.u().eregistry().find(fnnameid)); pfn) {
        return pfn.get();
    }
    function_signature const& sig = signatures.back();
    auto it = std::ranges::lower_bound(sig.named_parameters(), f.value,
        [](auto const& l, auto const& r) { return l < r; },
        [](auto const& v) { return std::get<0>(v).value; });
    if (it == sig.named_parameters().end() || std::get<0>(*it) != f) {
        annotated_qname_identifier aqnid{ ctx.u().qnregistry().resolve(qname{f.value}), f.location };
        return std::unexpected(make_error<parameter_not_found_error>(name(), aqnid));
    }

    function_signature fn_getter_sig;
    fn_getter_sig.position_parameters().emplace_back(bang_object_t{ this });
    fn_getter_sig.fn_type.result = std::get<1>(*it);
    fn_getter_sig.normilize(ctx);
    fn_getter_sig.build_mangled_id(ctx.u());
    auto fnent = sonia::make_shared<function_entity>(fnnameid, std::move(fn_getter_sig));
    fnent->set_inline();
    fnent->body.emplace_back(semantic::push_value{ ctx.u().as_string(f.value) });
    fnent->body.emplace_back(ctx.u().get_builtin_function(unit::builtin_fn::extern_object_get_property));
    ctx.u().eregistry().insert(fnent);
    return fnent.get();
#endif
}

std::expected<function_entity const*, error_storage> type_entity2::find_field_setter(fn_compiler_context& ctx, annotated_identifier const& f) const
{
    THROW_NOT_IMPLEMENTED_ERROR("type_entity find_field_setter");
#if 0
    qname fnname = ctx.u().qnregistry().resolve(name_) / f.value / ctx.u().slregistry().resolve("s"sv);
    qname_identifier fnnameid = ctx.u().qnregistry().resolve(fnname);
    if (auto pfn = dynamic_pointer_cast<function_entity>(ctx.u().eregistry().find(fnnameid)); pfn) {
        return pfn.get();
    }
    function_signature const& sig = signatures.back();
    auto it = std::ranges::lower_bound(sig.named_parameters(), f.value,
        [](auto const& l, auto const& r) { return l < r; },
        [](auto const& v) { return std::get<0>(v).value; });
    if (it == sig.named_parameters().end() || std::get<0>(*it) != f) {
        annotated_qname_identifier aqnid{ ctx.u().qnregistry().resolve(qname{f.value}), f.location };
        return std::unexpected(make_error<parameter_not_found_error>(name(), aqnid));
    }

    function_signature fn_setter_sig;
    fn_setter_sig.position_parameters().emplace_back(bang_object_t{this});
    fn_setter_sig.position_parameters().emplace_back(std::get<1>(*it));
    fn_setter_sig.fn_type.result = std::get<1>(*it);
    fn_setter_sig.normilize(ctx);
    //fn_setter_sig.build_mangled_id(ctx.u());
    auto fnent = sonia::make_shared<function_entity>(fnnameid, std::move(fn_setter_sig));
    fnent->set_inline();
    fnent->body.emplace_back(semantic::push_value{ ctx.u().as_string(f.value) });
    fnent->body.emplace_back(ctx.u().get_builtin_function(unit::builtin_fn::extern_object_set_property));
    fnent->body.emplace_back(semantic::truncate_values(1, true)); // remove object reference, preserve value
    ctx.u().eregistry().insert(fnent);
    return fnent.get();
#endif
}
#endif
}
