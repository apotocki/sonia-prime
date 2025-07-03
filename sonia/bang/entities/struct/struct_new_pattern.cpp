//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "struct_new_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"
#include "sonia/bang/entities/prepared_call.hpp"

#include "sonia/bang/auxiliary.hpp"
#include "sonia/bang/errors.hpp"

#include "struct_entity.hpp"


//#include "sonia/bang/entities/functions/function_entity.hpp"

namespace sonia::lang::bang {

class new_struct_match_descriptor : public functional_match_descriptor
{
public:
    struct_entity const& se;
    functional::match init_match;

    new_struct_match_descriptor(struct_entity const& se, functional::match m)
        : se{ se }
        , init_match{ std::move(m) }
    {}
};

std::expected<functional_match_descriptor_ptr, error_storage> struct_new_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();

    identifier tpid = u.get(builtin_id::type);

    struct_entity const* pse = nullptr;
    lex::resource_location typeloc;
    // looking for '__type' parameter
    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        if (pargname && pargname->value == tpid) {
            syntax_expression_t const& arg_expr = arg.value();
            auto res = apply_visitor(ct_expression_visitor{ ctx, call.expressions }, arg_expr);
            if (!res) return std::unexpected(std::move(res.error()));
            if (res->expressions) THROW_NOT_IMPLEMENTED_ERROR("struct_new_pattern::try_match, const value expressions"sv);
            entity const& some_entity = get_entity(u, res->value);
            pse = dynamic_cast<struct_entity const*>(&some_entity);
            if (!pse) return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, expected a structure"sv, pargname->value));
            typeloc = get_start_location(arg_expr);
            break;
        }
    }
    if (!pse) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter"sv, tpid));
    }

    pure_call_t init_call{ call.location };
    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        if (pargname && pargname->value == tpid) continue; // skip '__type' parameter
        syntax_expression_t const& arg_expr = arg.value();
        if (pargname) {
            init_call.emplace_back(*pargname, arg_expr);
        } else {
            init_call.emplace_back(arg_expr);
        }
    }
    
    auto init_match = ctx.find(builtin_qnid::init, init_call, call.expressions, expected_result_t{
        .type = pse->id, .location = pse->location, .modifier = value_modifier_t::runtime_value });
    if (!init_match) {
        return std::unexpected(append_cause(
            make_error<basic_general_error>(call.location, "no constructuctor found"sv, u.get(builtin_qnid::new_)),
            std::move(init_match.error())
        ));
    }

    return make_shared<new_struct_match_descriptor>(*pse, std::move(*init_match));
    //ctx.context_type = pse->id(); // for context_value

#if 0
    functional const& fn = u.fregistry().resolve(u.get(builtin_qnid::set));
    
    size_t pos_arg_num = 0;
    for (auto const& arg : call.args()) {
        annotated_identifier const* pargname = arg.name();
        if (pargname && pargname->value == tpid) continue; // skip '__type' parameter
        syntax_expression_t const& arg_expr = arg.value();
        pure_call_t set_call{ pargname ? pargname->location : get_start_location(arg_expr) };
        set_call.emplace_back(annotated_identifier{ u.get(builtin_id::self) }, context_value{ pse->id(), typeloc });
        set_call.emplace_back(annotated_identifier{ u.get(builtin_id::property) }, pargname ? syntax_expression_t{ *pargname } : annotated_integer{ pos_arg_num++ });
        set_call.emplace_back(arg_expr);

        auto match = fn.find(ctx, set_call);
        if (!match) {
            auto err = pargname ?
                make_error<basic_general_error>(pargname->location, "argument mismatch"sv, *pargname) :
                make_error<basic_general_error>(get_start_location(arg_expr), "argument mismatch"sv, arg_expr);
            return std::unexpected(append_cause(err, std::move(match.error())));
        }
        pmd->push_back(pargname, std::move(*match));
        /*
        auto last_expr_it = ctx.expressions().last();
        if (auto err = match->apply(ctx); err) return std::unexpected(err);
        parameter_match_result* pmr = pargname ? &pmd->get_match_result(pargname->value) : &pmd->get_match_result(pos_arg_num++);
        pmr->append_result(false, ctx.context_type, last_expr_it, ctx.expressions());
        */
    }
#endif
}

std::expected<syntax_expression_result_t, error_storage> struct_new_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t&, functional_match_descriptor& md) const
{
    // create tuple instance
    unit& u = ctx.u();

    BOOST_ASSERT(dynamic_cast<new_struct_match_descriptor*>(&md));
    new_struct_match_descriptor& nsmd = static_cast<new_struct_match_descriptor&>(md);

    return nsmd.init_match.apply(ctx);
#if 0
    struct_entity const& se = nsmd.get_struct_entity();
    auto uteid = se.underlying_tuple_eid(ctx);
    if (!uteid) return std::move(uteid.error());
    entity const& tplent = u.eregistry_get(*uteid);
    entity_signature const* tplsig = tplent.signature();
    BOOST_ASSERT(tplsig && tplsig->name() == u.get(builtin_qnid::tuple));

    {
        auto estate = ctx.expressions_state();
        ctx.push_chain(md.call_expressions);

        size_t pos_arg_num = 0;
        for (auto & [argname, match] : nsmd.arguments()) {
            auto last_expr_it = ctx.expressions().last();
            if (auto err = match.apply(ctx); err) return err;
            parameter_match_result* pmr = argname ? &md.get_match_result(argname.value) : &md.get_match_result(pos_arg_num++);
            pmr->append_result(false, ctx.context_type, last_expr_it, ctx.expressions());
        }
    }

    // create underlying tuple instance
    auto tin = se.underlying_tuple_initializer(ctx);
    if (!tin) return std::move(tin.error());
    functional_match_descriptor & tmd = const_cast<functional_match_descriptor&>(**tin);

    size_t argcount = 0;
    // push call expressions in the right order
    ctx.append_expression(semantic::expression_list_t{});
    semantic::expression_list_t& args = get<semantic::expression_list_t&>(ctx.expressions().back());

    tmd.for_each_match([&argcount, &args, &tmd](auto matchid, parameter_match_result const& mr) {
        BOOST_ASSERT(!mr.expressions.empty());
        for (auto rng : mr.expressions) {
            ++rng.second;
            args.splice_back(tmd.call_expressions, rng.first, rng.second);
        }
        ++argcount;
    });

    BOOST_ASSERT(!tmd.call_expressions); // all arguments were transfered
    if (argcount > 1) {
        u.push_back_expression(args, semantic::push_value{ mp::integer{ argcount } });
        u.push_back_expression(args, semantic::invoke_function(u.get(builtin_eid::arrayify)));
    }
    ctx.context_type = se.id();
    return {};
#endif

    // set fields
    //for (auto const& nf : tplsig->named_fields()) {
    //    if (nf.second.is_const()) continue;
    //    auto it = std::find_if(nsmd.arguments_.begin(), nsmd.arguments_.end(), [&nf](auto const& a) {
    //        return std::get<0>(a).value == nf.first;
    //        });
    //    if (it == nsmd.arguments_.end()) {
    //        return make_error<basic_general_error>(md.location(), "field not found"sv, nf.first);
    //    }
    //    auto& [name, match] = *it;
    //    auto last_expr_it = ctx.expressions().last();
    //    if (auto err = match.apply(ctx); err) return err;
    //    parameter_match_result* pmr = &nsmd.get_match_result(name.value);
    //    pmr->append_result(false, ctx.context_type, last_expr_it, ctx.expressions());
    //}

    //struct_utility::create_tuple_instance(ctx, uteid);

    // assign fields
    //THROW_NOT_IMPLEMENTED_ERROR("struct_new_pattern::apply");
}



std::ostream& struct_new_pattern::print(unit const&, std::ostream& s) const
{
    return s << "new structure"sv;
}

}
