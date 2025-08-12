//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "tuple_project_size_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/errors/type_mismatch_error.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

std::expected<functional_match_descriptor_ptr, error_storage> tuple_project_size_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();
    auto call_session = call.new_session(ctx);
    prepared_call::argument_descriptor_t arg_descr;
    auto arg = call_session.use_next_positioned_argument(&arg_descr);
    if (!arg) {
        if (!arg.error()) {
            return std::unexpected(make_error<basic_general_error>(call.location, "missing required argument"sv));
        }
        return std::unexpected(std::move(arg.error()));
    }

    if (auto argterm = call_session.unused_argument(); argterm) {
        return std::unexpected(make_error<basic_general_error>(argterm.location(), "argument mismatch"sv, std::move(argterm.value())));
    }

    entity_identifier slftype;
    
    syntax_expression_result_t& arg_er = arg->first;
    if (arg_er.is_const_result) {
        entity const& slf_entity = get_entity(u, arg_er.value());
        slftype = slf_entity.get_type();
    } else {
        slftype = arg_er.type();
    }

    entity const& tpl_prj_entity = get_entity(u, slftype);
    auto psig = tpl_prj_entity.signature();
    if (!psig || psig->name != u.get(builtin_qnid::tuple_project)) {
        return std::unexpected(make_error<type_mismatch_error>(get_start_location(*get<0>(arg_descr)), arg_er.value_or_type, "a tuple_project"sv));
    }
        
    // Extract project name and original tuple from signature
    if (psig->field_count() != 2 || !psig->result || psig->result->entity_id() != u.get(builtin_eid::typename_)) { // tuple_project, (id, orig_tuple)
        return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(arg_descr)), "invalid tuple_project signature"sv, tpl_prj_entity.id));
    }
        
    // Get project name from signature (first field)
    identifier project_name;
    entity const& name_entity = get_entity(u, psig->field(0).entity_id());
    if (identifier_entity const* pident = dynamic_cast<const identifier_entity*>(&name_entity)) {
        project_name = pident->value();
    } else {
        return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(arg_descr)), "invalid tuple_project name"sv, name_entity.id));
    }
        
    // Get original tuple entity (second field)
    entity const& orig_tuple = get_entity(u, psig->field(1).entity_id());
    auto orig_sig = orig_tuple.signature();
    if (!orig_sig || orig_sig->name != u.get(builtin_qnid::tuple)) {
        return std::unexpected(make_error<basic_general_error>(get_start_location(*get<0>(arg_descr)), "invalid tuple_project origin tuple"sv, orig_tuple.id));
    }
    
    // Count the number of fields with the projection name
    size_t project_size = 0;
    auto frng = orig_sig->find_fields(project_name);
    project_size = std::distance(frng.first, frng.second);
        
    functional_match_descriptor_ptr pmd = make_shared<functional_match_descriptor>(call);
    pmd->signature.result.emplace(ctx.u().make_integer_entity(project_size).id, true);
    pmd->emplace_back(0, arg_er);
    return pmd;
}

std::expected<syntax_expression_result_t, error_storage> tuple_project_size_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    syntax_expression_result_t & arg = get<1>(md.matches.front());

    return syntax_expression_result_t{
        .temporaries = std::move(arg.temporaries),
        .value_or_type = md.signature.result->entity_id(),
        .is_const_result = true
    };
}

} // namespace sonia::lang::bang