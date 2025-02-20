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

struct_init_pattern::struct_init_pattern(functional const& fnl, variant<field_list_t, statement_span> const& body)
    : basic_fn_pattern{ fnl }
{
    apply_visitor(make_functional_visitor<void>([this](auto const& body) {
        if constexpr (std::is_same_v<std::decay_t<decltype(body)>, field_list_t>) {
            field_list_t const& fl = body;
            for (field_t const& f : fl) {
                body_parameters_.emplace_back(
                    parameter_t{ named_parameter_name{ f.name }, f.modifier, parameter_constraint_set_t{ f.type } },
                    f.value
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

error_storage struct_init_pattern::init(fn_compiler_context& ctx, annotated_qname name, parameter_woa_list_t const& fparameters)
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

std::expected<functional_match_descriptor_ptr, error_storage> struct_init_pattern::try_match(fn_compiler_context& ctx, pure_call_t const& call, annotated_entity_identifier const& exp) const
{
    auto res = basic_fn_pattern::try_match(ctx, call, exp);
    //if (!res) {
    //    GLOBAL_LOG_WARN() << ctx.u().print(*res.error());
    //}
    return res;
}

error_storage struct_init_pattern::apply(fn_compiler_context& ctx, qname_identifier fid, functional_match_descriptor& md) const
{
    // create tuple instance
    unit& u = ctx.u();
    size_t argcount = apply_arguments(ctx, md);

    if (argcount > 1) {
        u.push_back_expression(ctx.expressions(), semantic::push_value{ mp::integer{ argcount } });
        u.push_back_expression(ctx.expressions(), semantic::invoke_function(u.get(builtin_eid::arrayify)));
    }

    BOOST_ASSERT(md.result);
    ctx.context_type = md.result;
    return {};
}

std::expected<entity_identifier, error_storage> struct_init_pattern::const_apply(fn_compiler_context&, qname_identifier, functional_match_descriptor&) const
{
    THROW_NOT_IMPLEMENTED_ERROR("struct_init_pattern::const_apply");
}

}
