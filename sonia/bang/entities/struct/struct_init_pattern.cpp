//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "struct_init_pattern.hpp"
#include "struct_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"

#include "sonia/bang/entities/literals/literal_entity.hpp"
//#include "sonia/bang/errors/circular_dependency_error.hpp"

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
    fn_pure_t init_fn{ .location = result.location };
    init_fn.parameters = std::move(body_parameters_);
    init_fn.result = result;
    return basic_fn_pattern::init(ctx, init_fn);
}

error_storage struct_init_pattern::init(fn_compiler_context& ctx, annotated_qname name, parameter_list_t const& fparameters)
{
    pattern_t::signature_descriptor sigdesc{ .name = name };
    for (parameter_t const& param : fparameters) {
        auto [external_name, internal_name] = apply_visitor(param_name_retriever{}, param.name);
        if (external_name) {
            if (internal_name) {
                sigdesc.fields.emplace_back(*external_name, pattern_t{ .descriptor = context_identifier{ annotated_identifier{ internal_name->value, internal_name->location } } });
            } else {
                // to do: prepare and use replacement
                sigdesc.fields.emplace_back(*external_name, pattern_t{ .descriptor = context_identifier{ annotated_identifier{ external_name->value, external_name->location } } });
            }
        } else if (internal_name) {
            BOOST_ASSERT(*internal_name);
            sigdesc.fields.emplace_back(nullptr, pattern_t{ .descriptor = context_identifier{ annotated_identifier{ internal_name->value, internal_name->location } } });
        } else {
            sigdesc.fields.emplace_back(nullptr, pattern_t{ .descriptor = placeholder{} });
        }
    }

    fn_pure_t init_fn{ .location = name.location };
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
    auto [r, argcount] = apply_arguments(ctx, el, md);

    if (argcount > 1) {
        u.push_back_expression(el, r.expressions, semantic::push_value{ smart_blob{ ui64_blob_result(argcount) } });
        u.push_back_expression(el, r.expressions, semantic::invoke_function(u.get(builtin_eid::arrayify)));
    }

    BOOST_ASSERT(md.signature.result && md.signature.result->entity_id());
    r.value_or_type = r.is_const_result ? u.make_empty_entity(md.signature.result->entity_id()).id : md.signature.result->entity_id();
    return r;
}

}
