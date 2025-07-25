//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#include "sonia/config.hpp"
#include "ellipsis_pattern.hpp"

#include "sonia/bang/entities/prepared_call.hpp"
#include "sonia/bang/entities/signatured_entity.hpp"
#include "sonia/bang/entities/literals/literal_entity.hpp"

#include "sonia/bang/ast/fn_compiler_context.hpp"
#include "sonia/bang/ast/ct_expression_visitor.hpp"

#include "sonia/bang/auxiliary.hpp"

namespace sonia::lang::bang {

class ellipsis_match_descriptor : public functional_match_descriptor
{
    variant<identifier_entity const*, basic_signatured_entity const*> arg_;

public:
    template <typename EntityT>
    inline ellipsis_match_descriptor(qname_identifier fname, EntityT const* ent, lex::resource_location const& loc) noexcept
        : functional_match_descriptor{ std::move(fname), loc }
        , arg_{ ent }
    {}

    auto const& argument() const noexcept { return arg_; }
};

// postfix operator ...(__identifier|metaobject(__identifier...))
std::expected<functional_match_descriptor_ptr, error_storage> ellipsis_pattern::try_match(fn_compiler_context& ctx, prepared_call const& call, expected_result_t const&) const
{
    unit& u = ctx.u();
    syntax_expression_t const* object_arg = nullptr;

    for (auto const& arg : call.args) {
        annotated_identifier const* pargname = arg.name();
        if (pargname) {
            return std::unexpected(make_error<basic_general_error>(pargname->location, "argument mismatch, unexpected argument"sv, *pargname));
        }
        syntax_expression_t const& e = arg.value();
        if (object_arg) {
            return std::unexpected(make_error<basic_general_error>(get_start_location(e), "argument mismatch, unexpected argument"sv, e));
        }
        object_arg = &e;
    }

    if (!object_arg) {
        return std::unexpected(make_error<basic_general_error>(call.location, "unmatched parameter"sv));
    }

    ct_expression_visitor eobjvis{ ctx, call.expressions };
    auto obj = apply_visitor(eobjvis, *object_arg);
    if (!obj) return std::unexpected(std::move(obj.error()));
    BOOST_ASSERT(!obj->expressions); // not impelemented const value expressions
    entity const& metaobject_ent = get_entity(u, obj->value);
    if (identifier_entity const* pie = dynamic_cast<identifier_entity const*>(&metaobject_ent); pie) {
        return make_shared<ellipsis_match_descriptor>(call.functional_id(), pie, get_start_location(*object_arg));
    }
    else if (basic_signatured_entity const* bse = dynamic_cast<basic_signatured_entity const*>(&metaobject_ent); bse) {
        entity_signature const& signature = *bse->signature();
        if (signature.name == u.get(builtin_qnid::metaobject)) {
            // to do: check if the signature is a comopatible to the call metaobject
            return make_shared<ellipsis_match_descriptor>(call.functional_id(), bse, get_start_location(*object_arg));
        }
    }
    
    return std::unexpected(make_error<basic_general_error>(call.location, "argument mismatch"sv, *object_arg));
}

std::expected<field_descriptor, error_storage> push_by_name(fn_compiler_context& ctx, semantic::expression_list_t& el, annotated_qname const& name, semantic::expression_span & result)
{
    auto optent = ctx.lookup_entity(name);
    using result_t = std::expected<field_descriptor, error_storage>;
    return apply_visitor(make_functional_visitor<result_t>([&ctx, &el, &name, &result](auto& eid_or_var) -> result_t
    {
        if constexpr (std::is_same_v<std::decay_t<decltype(eid_or_var)>, entity_identifier>) {
            if (!eid_or_var) return std::unexpected(make_error<undeclared_identifier_error>(std::move(name)));
            return field_descriptor{ eid_or_var, true };
        } else {
            ctx.u().push_back_expression(el, result, semantic::push_local_variable{ eid_or_var });
            return field_descriptor{ eid_or_var.type, false };
        }
    }), optent);
}

std::expected<syntax_expression_result_t, error_storage> ellipsis_pattern::apply(fn_compiler_context& ctx, semantic::expression_list_t& el, functional_match_descriptor& md) const
{
    BOOST_ASSERT(dynamic_cast<ellipsis_match_descriptor*>(&md));

    ellipsis_match_descriptor& nsmd = static_cast<ellipsis_match_descriptor&>(md);
    using result_t = std::expected<syntax_expression_result_t, error_storage>;
    return apply_visitor(make_functional_visitor<result_t>([&ctx, &nsmd, &el](auto const* pe) -> result_t {
        unit& u = ctx.u();
        
        semantic::expression_span l = nsmd.merge_void_spans(el);
        if constexpr (std::is_same_v<decltype(pe), identifier_entity const*>) {
            annotated_qname varname{ qname{ pe->value(), false }, nsmd.call_location };
            auto res = push_by_name(ctx, el, varname, l);
            if (!res) return std::unexpected(std::move(res.error()));
            return syntax_expression_result_t{
                .expressions = std::move(l),
                .value_or_type = res->entity_id(),
                .is_const_result = res->is_const()
            };
        } else {
            basic_signatured_entity const* bse = pe;
            // make tuple
            size_t argcount = 0; // runtime arguments
            entity_signature sig{ u.get(builtin_qnid::tuple), u.get(builtin_eid::typename_) };
            for (auto const& field : bse->signature()->fields()) {
                entity const& metaobject_ent = get_entity(u, field.entity_id());
                identifier_entity const* pie = dynamic_cast<identifier_entity const*>(&metaobject_ent);
                if (!pie) {
                    return std::unexpected(make_error<basic_general_error>(nsmd.call_location, "identifier is expected"sv, metaobject_ent.id));
                }
                annotated_qname varname{ qname{ pie->value(), false }, nsmd.call_location };
                auto res = push_by_name(ctx, el, varname, l);
                if (!res) return std::unexpected(std::move(res.error()));
                sig.emplace_back(pie->value(), *res);
                if (!res->is_const()) ++argcount;
            }
            
            if (argcount > 1) {
                u.push_back_expression(el, l, semantic::push_value{ smart_blob{ ui64_blob_result(argcount) } });
                u.push_back_expression(el, l, semantic::invoke_function(u.get(builtin_eid::arrayify)));
            }

            return syntax_expression_result_t{
                .expressions = std::move(l),
                .value_or_type = u.make_basic_signatured_entity(std::move(sig)).id,
                .is_const_result = !argcount
            };
        }
    }), nsmd.argument());
}

}
