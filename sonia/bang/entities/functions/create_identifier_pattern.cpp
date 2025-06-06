//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "create_identifier_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

// __id(const string)
std::expected<functional_match_descriptor_ptr, error_storage> create_identifier_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    if (call.args.empty()) {
        return std::unexpected(make_error<basic_general_error>(call.location, "an argument is expected"sv));
    }
    auto const& opt_named_expr = call.args.front();
    if (annotated_identifier const* optname = opt_named_expr.name(); optname) {
        return std::unexpected(make_error<basic_general_error>(optname->location, "unexpected named argument"sv, optname->value));
    }

    if (call.args.size() != 1) {
        return std::unexpected(make_error<basic_general_error>(get_start_location(call.args[1].value()), "unexpected argument"sv));
    }
    
    auto const& expr = opt_named_expr.value();

    ct_expression_visitor evis{ ctx, call.expressions, expected_result_t{ ctx.u().get(builtin_eid::string), get_start_location(expr) } };

    auto res = apply_visitor(evis, expr);
    if (!res) return std::unexpected(std::move(res.error()));

    auto pmd = make_shared<functional_match_descriptor>();
    pmd->get_match_result(0).append_result(*res);
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> create_identifier_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    unit& u = ctx.u();
    auto& ser = md.get_match_result(0).results.front();
    entity_identifier strid = ser.value();
    string_literal_entity const& str_ent = static_cast<string_literal_entity const&>(get_entity(u, ser.value()));
    identifier id = u.slregistry().resolve(str_ent.value());
    semantic::managed_expression_list rel{ u };
    rel.splice_back(el, ser.expressions);
        
    return syntax_expression_result_t{
        .expressions = std::move(rel),
        .value_or_type = u.make_identifier_entity(id).id,
        .is_const_result = true
    };
}

}
