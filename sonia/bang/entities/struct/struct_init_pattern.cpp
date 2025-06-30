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

struct_init_pattern::struct_init_pattern(variant<field_list_t, statement_span> const& body)
{
    apply_visitor(make_functional_visitor<void>([this](auto const& body) {
        if constexpr (std::is_same_v<std::decay_t<decltype(body)>, field_list_t>) {
            field_list_t const& fl = body;
            for (field_t const& f : fl) {
                body_parameters_.emplace_back(
                     named_parameter_name{ f.name }, f.type_or_value, f.value, f.modifier
                );
            }
        } else {
            THROW_NOT_IMPLEMENTED_ERROR("struct_init_pattern::struct_init_pattern(statement_span)");
        }
    }), body);
}

error_storage struct_init_pattern::init(fn_compiler_context& ctx, annotated_entity_identifier result)
{
#if 0
    fn_pure_t init_fn{ annotated_qname{ {}, result.location } };
    init_fn.parameters = std::move(body_parameters_);
    init_fn.result = result;
    return basic_fn_pattern::init(ctx, init_fn);
#endif
    THROW_NOT_IMPLEMENTED_ERROR("struct_init_pattern::init(fn_compiler_context&, annotated_entity_identifier)");
}

error_storage struct_init_pattern::init(fn_compiler_context& ctx, annotated_qname name, parameter_list_t const& fparameters)
{
    pattern_t::signature_descriptor sigdesc{ .name = name };
    for (parameter_t const& param : fparameters) {
        auto [external_name, internal_name] = apply_visitor(param_name_retriever{}, param.name);
        if (external_name) {
            if (internal_name) {
                sigdesc.fields.emplace_back(*external_name, pattern_t{ .descriptor = variable_reference{ annotated_qname{ qname{ internal_name->value, false }, internal_name->location }, true } } );
            } else {
                // to do: prepare and use replacement
                sigdesc.fields.emplace_back(*external_name, pattern_t{ .descriptor = variable_reference{ annotated_qname{ qname{ external_name->value, false }, external_name->location }, true } } );
            }
        } else if (internal_name) {
            BOOST_ASSERT(internal_name);
            sigdesc.fields.emplace_back(nullptr, pattern_t{ .descriptor = variable_reference{ annotated_qname{ qname{ internal_name->value, false }, internal_name->location }, true } });
        } else {
            sigdesc.fields.emplace_back(nullptr, pattern_t{ .descriptor = placeholder{} });
        }
    }

    fn_pure_t init_fn{ annotated_qname{ {}, name.location } };
    init_fn.parameters = std::move(body_parameters_);
    init_fn.result = pattern_t{ .descriptor = std::move(sigdesc) };
    
    return basic_fn_pattern::init(ctx, init_fn);
}

std::expected<functional_match_descriptor_ptr, error_storage> struct_init_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const& exp) const
{
    auto res = basic_fn_pattern::try_match(ctx, call, exp);
    //if (!res) {
    //    GLOBAL_LOG_WARN() << ctx.u().print(*res.error());
    //}
    return res;
}

std::expected<syntax_expression_result_t, error_storage> struct_init_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    // create tuple instance
    unit& u = ctx.u();
    
#if 0
    auto [exprs, argcount] = apply_arguments(ctx, md, el);

    if (argcount > 1) {
        u.push_back_expression(el, exprs, semantic::push_value{ mp::integer{ argcount } });
        u.push_back_expression(el, exprs, semantic::invoke_function(u.get(builtin_eid::arrayify)));
    }

    BOOST_ASSERT(md.result.entity_id());
    
    return syntax_expression_result_t{
        .expressions = std::move(el),
        .value_or_type = md.result.entity_id(),
        .is_const_result = false
    };
#endif
    THROW_NOT_IMPLEMENTED_ERROR("struct_init_pattern::apply(fn_compiler_context&, semantic::expression_list_t&, functional_match_descriptor&)");
}

}
