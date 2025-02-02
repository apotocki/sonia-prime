//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "new_struct_pattern.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/errors.hpp"

#include "struct_entity.hpp"


//#include "sonia/bang/entities/functions/function_entity.hpp"

namespace sonia::lang::bang {

class new_struct_match_descriptor : public functional_match_descriptor
{
    struct_entity const& se_;
    small_vector<std::tuple<annotated_identifier, functional::match>, 16> arguments_;

public:
    using functional_match_descriptor::functional_match_descriptor;

    new_struct_match_descriptor(unit& u, struct_entity const& se)
        : functional_match_descriptor{ u }
        , se_{ se }
    {}

    void push_back(annotated_identifier const* name, functional::match&& m)
    {
        arguments_.emplace_back(name ? *name : annotated_identifier{}, std::move(m));
    }

    inline struct_entity const& get_struct_entity() const noexcept { return se_; }
    inline span<std::tuple<annotated_identifier, functional::match>> arguments() noexcept { return arguments_; }
};

std::expected<functional_match_descriptor_ptr, error_storage> new_struct_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    unit& u = ctx.u();

    identifier tpid = u.get(builtin_id::type);

    struct_entity const* pse = nullptr;
    lex::resource_location typeloc;
    // looking for '__type' parameter
    for (auto const& arg : call.args()) {
        annotated_identifier const* pargname = arg.name();
        if (pargname && pargname->value == tpid) {
            syntax_expression_t const& arg_expr = arg.value();
            auto res = apply_visitor(ct_expression_visitor{ ctx }, arg_expr);
            if (!res) return std::unexpected(std::move(res.error()));
            entity const& some_entity = u.eregistry_get(*res);
            pse = dynamic_cast<struct_entity const*>(&some_entity);
            if (!pse) return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, expected a structure"sv, pargname->value));
            typeloc = get_start_location(arg_expr);
            break;
        }
    }
    if (!pse) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "unmatched parameter"sv, tpid));
    }

    auto pmd = make_shared<new_struct_match_descriptor>(u, *pse);
    auto estate = ctx.expressions_state();
    ctx.push_chain(pmd->call_expressions);
    ctx.context_type = pse->id(); // for context_value

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
            return std::unexpected(append_cause(
                make_error<basic_general_error>(set_call.location(), "argument mismatch"sv, arg_expr),
                std::move(match.error())
            ));
        }
        pmd->push_back(pargname, std::move(*match));
        /*
        auto last_expr_it = ctx.expressions().last();
        if (auto err = match->apply(ctx); err) return std::unexpected(err);
        parameter_match_result* pmr = pargname ? &pmd->get_match_result(pargname->value) : &pmd->get_match_result(pos_arg_num++);
        pmr->append_result(false, ctx.context_type, last_expr_it, ctx.expressions());
        */
    }

    return pmd;
}

error_storage new_struct_pattern::apply(fn_compiler_context& ctx, qname_identifier fid, functional_match_descriptor& md) const
{
    // create tuple instance
    unit& u = ctx.u();

    BOOST_ASSERT(dynamic_cast<new_struct_match_descriptor*>(&md));
    new_struct_match_descriptor& nsmd = static_cast<new_struct_match_descriptor&>(md);

    size_t pos_arg_num = 0;
    for (auto & [argname, match] : nsmd.arguments()) {
        auto last_expr_it = ctx.expressions().last();
        if (auto err = match.apply(ctx); err) return err;
        parameter_match_result* pmr = argname ? &md.get_match_result(argname.value) : &md.get_match_result(pos_arg_num++);
        pmr->append_result(false, ctx.context_type, last_expr_it, ctx.expressions());
    }

    struct_entity const& se = nsmd.get_struct_entity();
    auto uteid = se.underlying_tuple_eid(ctx);
    if (!uteid) return std::move(uteid.error());
    entity const& tplent = u.eregistry_get(*uteid);
    entity_signature const* tplsig = tplent.signature();
    BOOST_ASSERT(tplsig && tplsig->name() == u.get(builtin_qnid::tuple));

    ctx.append_expression(semantic::expression_list_t{});
    semantic::expression_list_t& args = get<semantic::expression_list_t&>(ctx.expressions().back());

    // create underlying tuple instance
    auto ctor = se.underlying_tuple_constructor_eid(ctx);
    if (!ctor) return std::move(ctor.error());
    u.push_back_expression(args, semantic::invoke_function(*ctor));


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
    THROW_NOT_IMPLEMENTED_ERROR("new_struct_pattern::apply");
}

std::expected<entity_identifier, error_storage> new_struct_pattern::const_apply(fn_compiler_context&, qname_identifier, functional_match_descriptor&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("new_struct_pattern::const_apply");
}

std::ostream& new_struct_pattern::print(unit const&, std::ostream& s) const
{
    return s << "new structure";
}

}
