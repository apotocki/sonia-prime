//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "struct_init_pattern.hpp"
#include "struct_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/errors/circular_dependency_error.hpp"

namespace sonia::lang::bang {

std::ostream& struct_init_pattern::print(unit const&, std::ostream& s) const
{
    return s << "init(...) -> @structure"sv;
}

struct_init_pattern::struct_init_pattern(functional const& fnl, variant<field_list_t, statement_span> const& body)
    : basic_fn_pattern{ fnl }
{
    apply_visitor(make_functional_visitor<void>([this](auto const& body) {
        if constexpr (std::is_same_v<std::decay_t<decltype(body)>, field_list_t>) {
            field_list_t const& fl = body;
            for (field_t const& f : fl) {
                // to do: fix it for const/exact
                parameter_constraint_modifier_t pcm = [](field_modifier_t fm) -> parameter_constraint_modifier_t {
                    switch (fm)
                    {
                    case sonia::lang::bang::field_modifier_t::value:
                        return parameter_constraint_modifier_t::mutable_value_type;
                    case sonia::lang::bang::field_modifier_t::const_value:
                        return parameter_constraint_modifier_t::const_value_type;
                    default:
                        THROW_INTERNAL_ERROR("struct_init_pattern::struct_init_pattern(field_list_t)");
                    }
                }(f.modifier);
                body_parameters_.emplace_back(
                     named_parameter_name{ f.name }, pcm, parameter_constraint_set_t{ f.type }, f.value
                );
            }
        } else {
            THROW_NOT_IMPLEMENTED_ERROR("struct_init_pattern::struct_init_pattern(statement_span)");
        }
    }), body);
}

error_storage struct_init_pattern::init(fn_compiler_context& ctx, annotated_entity_identifier result)
{
    fn_pure_t init_fn{ annotated_qname{ {}, result.location } };
    init_fn.parameters = std::move(body_parameters_);
    init_fn.result = result;
    return basic_fn_pattern::init(ctx, init_fn);
}

error_storage struct_init_pattern::init(fn_compiler_context& ctx, annotated_qname name, parameter_list_t const& fparameters)
{
    function_call_t rpattern{ lex::resource_location{ name.location }, syntax_expression_t{ name } };
    for (parameter_t const& param : fparameters) {
        auto [external_name, internal_name, varname] = apply_visitor(param_name_retriever{}, param.name);
        if (external_name) {
            if (internal_name) {
                rpattern.emplace_back(*external_name, variable_identifier{ annotated_qname{ qname{ internal_name->value, false }, internal_name->location }, true } );
            } else {
                // to do: prepare and use replacement
                rpattern.emplace_back(*external_name, variable_identifier{ annotated_qname{ qname{ external_name->value, false }, external_name->location }, true } );
            }
        } else if (internal_name) {
            rpattern.emplace_back(variable_identifier{ annotated_qname{ qname{ internal_name->value, false }, internal_name->location }, true });
        } else {
            rpattern.emplace_back(placeholder{});
        }
    }

    fn_pure_t init_fn{ annotated_qname{ {}, name.location } };
    init_fn.parameters = std::move(body_parameters_);
    init_fn.result = std::move(rpattern);
    
    return basic_fn_pattern::init(ctx, init_fn);
}

std::expected<functional_match_descriptor_ptr, error_storage> struct_init_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, annotated_entity_identifier const& exp) const
{
    auto res = basic_fn_pattern::try_match(ctx, call, exp);
    //if (!res) {
    //    GLOBAL_LOG_WARN() << ctx.u().print(*res.error());
    //}
    return res;
}

std::expected<functional::pattern::application_result_t, error_storage> struct_init_pattern::apply(fn_compiler_context& ctx, functional_match_descriptor& md) const
{
    // create tuple instance
    unit& u = ctx.u();
    
    auto [el, argcount] = apply_arguments(ctx, md);

    if (argcount > 1) {
        u.push_back_expression(el, semantic::push_value{ mp::integer{ argcount } });
        u.push_back_expression(el, semantic::invoke_function(u.get(builtin_eid::arrayify)));
    }

    BOOST_ASSERT(md.result.entity_id());
    ctx.context_type = md.result.entity_id();
    return std::move(el);
}

}
