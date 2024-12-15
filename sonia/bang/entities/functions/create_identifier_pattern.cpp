//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "create_identifier_pattern.hpp"

#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

//#include "pack_entity.hpp"

namespace sonia::lang::bang {

// __id(const string)
std::expected<functional_match_descriptor_ptr, error_storage> create_identifier_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const&) const
{
    if (call.args().empty()) {
        return std::unexpected(make_error<basic_general_error>(call.location(), "an argument is expected"sv));
    }
    auto const& opt_named_expr = call.args().front();
    if (annotated_identifier const* optname = opt_named_expr.name(); optname) {
        return std::unexpected(make_error<basic_general_error>(optname->location, "unexpected named argument"sv, optname->value));
    }

    if (call.args().size() != 1) {
        return std::unexpected(make_error<basic_general_error>(get_start_location(call.args()[1].value()), "unexpected argument"sv));
    }
    
    auto const& expr = opt_named_expr.value();

    ct_expression_visitor evis{ ctx, annotated_entity_identifier{ ctx.u().get_string_entity_identifier(), get_start_location(expr) } };

    auto res = apply_visitor(evis, expr);
    if (!res) return std::unexpected(std::move(res.error()));

    auto pmd = make_shared<functional_match_descriptor>(ctx.u());
    pmd->get_match_result(0).append_result(false, *res);
    return pmd;
}

std::expected<entity_identifier, error_storage> create_identifier_pattern::const_apply(fn_compiler_context& ctx, qname_identifier fid, functional_match_descriptor& md) const
{
    entity_identifier strid = md.get_match_result(0).result.front();
    string_literal_entity const& str_ent = static_cast<string_literal_entity const&>(ctx.u().eregistry().get(strid));
    identifier id = ctx.u().slregistry().resolve(str_ent.value());
    identifier_entity id_ent{ id };

    return ctx.u().eregistry().find_or_create(id_ent, [&ctx, id]() {
        auto result = make_shared<identifier_entity>(id);
        result->set_type(ctx.u().get_identifier_entity_identifier());
        return result;
    }).id();
}

}
